#include "tp_basic_tokenizer_rule_set.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "logging.hpp"

#include "escaping.hpp"

std::string TPBasicTokenizerRuleSet::itoa(size_t n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
};

void TPBasicTokenizerRuleSet::returnToTheBuffer(std::stringstream& st,
                                                std::string& resBuffer) {
    std::stringbuf stdbuf;
    std::string res;
    st.get(stdbuf, '\0');
    res = stdbuf.str();
    st.str(resBuffer + (res.length() > 0 ? "@" : "") + res);
    st.clear();
}

void TPBasicTokenizerRuleSet::clearState(std::map<std::string, Definition>& definitions,
                                         std::map<std::string, bool> & opened) {

    opened.clear();
    definitions.clear();
}

void TPBasicTokenizerRuleSet::loadWholeElement(std::istream& input, std::string& buff) {

    std::string line;
    std::getline(input, line, '@');

    buff = line;

    std::string buffCopy = buff;

    while ((Escaping::isEscaped(line, line.length())
            || removeComments(buffCopy))
           && !input.eof()) {

        buff += "@";
        std::getline(input, line, '@');
        buff += line;

        buffCopy = buff;
    }
}

void TPBasicTokenizerRuleSet::loadWholeElement(std::istream& input,
                                               std::string& buff,
                                               size_t& pos) {

    loadWholeElement(input, buff);

    std::string line;
    std::string buffCpy = buff;
    std::stringstream lineCount(buffCpy);
    while (std::getline(lineCount, line))
        pos++;
}

bool TPBasicTokenizerRuleSet::removeComments(std::string& comm) {
    size_t pos = 0;
    size_t last = 0;

    bool whatFollowsWouldBeCommentedOut = false;

    while ((pos = comm.find_first_of('#',pos)) != comm.npos) {

        if (!Escaping::isEscaped(comm, pos)) {
            if ((last = comm.find("\n",pos)) != comm.npos)
                comm.erase(pos, last - pos + 1);
            else {
                comm.erase(pos);
                whatFollowsWouldBeCommentedOut = true;
            }
        }
        else
            pos++;
    }

    return whatFollowsWouldBeCommentedOut;
}

void TPBasicTokenizerRuleSet::trim(std::string& strg) {
    if (strg.empty())
        return;

    size_t pos = strg.find_last_not_of("\n\r\t ");

    if (pos == std::string::npos)
        strg = "";
    else if (pos != strg.length() - 1) {
        if (Escaping::isEscaped(strg, pos+1) && strg[pos+1] == ' ')
            ++pos;

        if (pos != strg.length() - 1)
            strg.erase(pos+1);
    }

    strg = strg.erase(0, strg.find_first_not_of("\n\r\t "));
}

bool TPBasicTokenizerRuleSet::changeHereDoc(std::string& strg) {

    PerlRegExp hereBegin("<<([A-Za-z_0-9]+)([^\\n]*)\\n");

    std::string marker;
    std::string remainingPart;
    PerlStringPiece rest(strg);

    if (PerlRegExp::FindAndConsume(&rest, hereBegin, &marker, &remainingPart)) {

        size_t pos =
            strg.length() - rest.size() - marker.length() - remainingPart.length() - 3;
            // there are 3 non-captured chars

        std::string restS = rest.as_string();
        size_t markerPos = restS.find(marker);
        if (markerPos == std::string::npos)
            throw new TPTokenizerException("wrong here doc for `" + marker + "'");

        std::string hereDocBody = restS.substr(0, markerPos);

        PerlRegExp hereCenter("(\\s+)([^$])");
        PerlRegExp::GlobalReplace(&hereDocBody, hereCenter, "|\\2");
        hereDocBody = "(?i:" + hereDocBody + ")\\.(?!\\p{L})";

        strg.erase(pos);

        strg += hereDocBody;
        strg += remainingPart;
        strg += restS.substr(markerPos + marker.length());

        return true;
    }

    return false;
}

std::string TPBasicTokenizerRuleSet::applyDefinitions(
    std::string& buffer,
    std::map<std::string, Definition>& defs,
    std::string& subst) {

    if (buffer.length() > 4 && buffer.substr(0,4).compare("def ") == 0)
        return "";

    size_t pos = 0;

    if ((buffer.length() > 6 && buffer.substr(0,6).compare("ifdef ") == 0)
        || (buffer.length() > 7 && buffer.substr(0,7).compare("ifndef ") == 0)) {

        size_t begin = buffer.find_first_not_of("\n\r\t ", 6);
        if (begin == buffer.npos)
            begin = 6;

        size_t end = buffer.find_first_of("\n\r\t ",begin);

        if(end == buffer.npos)
            pos = buffer.length();
        else
            pos = end;
    }

    std::list<stackElem> stack;
    std::string context;

    if (buffer.substr(0,7) != "include") {

        size_t inc = buffer.length();
        size_t incFake = 0;

        while (pos < buffer.length() && pos < inc && stack.size() < MAXSTACKSIZE) {

            size_t len = 0;
            std::string candidate;

            for(std::map<std::string, Definition>::iterator def = defs.begin();
                def != defs.end();
                ++def) {

                // finding the longest substition at the given position
                if (buffer.find(def->first, pos) == pos && def->first.length() > len) {
                    len = def->first.length();
                    candidate = def->first;
                }
            }

            if (len > 0) {

                subst += "\npodstawiono:" + candidate + ";";
                std::string change = defs.find(candidate)->second.getValue();

                stack.push_back(
                    stackElem(
                        pos+change.length(),
                        change.length() - candidate.length(),
                        candidate));


                buffer.replace(pos, len, change);

                // ???
                while ((inc = buffer.find("@include", incFake)) != buffer.npos
                       && inc > 0 && (incFake = buffer.find("\\@include",inc - 1) == inc -1))
                    incFake = inc + 1;
            }
            else {
                ++pos;

                while(!stack.empty() && stack.back().epos <= pos) {
                    stackElem sTemp = stack.back();
                    stack.pop_back();
                    if(!stack.empty()) {
                        stack.back().epos += sTemp.len;
                        stack.back().len += sTemp.len;
                    }
                }
            }
        }
    }

    // definition loop
    if (stack.size() >= MAXSTACKSIZE) {
        while (!stack.empty()) {
            context += " --> " + stack.back().name;
            stack.pop_back();
        }
    }

    return context;
}
std::string TPBasicTokenizerRuleSet::getContent(
    std::string & buffer,
    std::string pattern,
    std::string msg,
    std::string & context,
    size_t filePos) {

    std::string target;
    PerlRegExp ruleName(pattern);

    PerlStringPiece bufferPiece(buffer);
    if (!PerlRegExp::FindAndConsume(&bufferPiece, ruleName, &target))
        throw new TPTokenizerException(msg + context + " at line: " + itoa(filePos));

    buffer = bufferPiece.as_string();

    return target;
}

void TPBasicTokenizerRuleSet::isEmpty(
    std::string & buffer,
    std::string msg,
    std::string & context,
    size_t filePos) {

    PerlRegExp empty("^\\s+$");

    if(PerlRegExp::FullMatch(buffer, empty))
        throw new TPTokenizerException(std::string(msg) + "in: "+ context + "at: " + itoa(filePos));
}

void TPBasicTokenizerRuleSet::readRule(
    std::string & resbuffer,
    std::string context,
    size_t filePos) {

    std::string uline = resbuffer;

    std::string uname = getContent(uline, "^\\s*([a-zA-Z_][a-zA-Z_0-9]+)\\s*/",
                                   "no rule name: ",
                                   context, filePos);

    std::string ucat = getContent(uline, "([a-zA-Z_]+)\\s*:=",
                                  "no rule type:  ",
                                  context, filePos);

    std::string ureg = getContent(uline, "(?s)(.+)",
                                  "no rule regexp:  ",
                                  context, filePos);

    isEmpty(ureg, "empty rule is unacceptable", context, filePos);

    while(changeHereDoc(ureg));

    PerlRegExp white("((?:^|[^\\\\])(?:\\\\\\\\)*)\\s+");
    PerlRegExp::GlobalReplace(&ureg, white, "\\1");

    PerlRegExp whiteB("\\\\(\\s)");
    PerlRegExp::GlobalReplace(&ureg, whiteB, "\\1");

    PerlRegExp ats("((?:^|[^\\\\])(?:\\\\\\\\)*)\\\\(@|#)");
    PerlRegExp::GlobalReplace(&ureg, ats, "\\1\\2");

    _regex r;
    ureg = "^("+ureg+")";

    boost::shared_ptr<PerlRegExp> reg(new PerlRegExp(ureg));

    if(!reg->ok())
    	throw TPTokenizerException(std::string("regex cannot be compiled: \n|")
                                   + ureg
                                   + std::string("| \nw: ")+context);
    else {
        if(uname == MAIN_REGEX_NAME)
            mainRegex = reg;
        else
            addRegex(reg, ucat, uname);
    }
}

void TPBasicTokenizerRuleSet::readInclude(
    std::string& resbuffer,
    std::map<std::string, std::string>& paths,
    std::string& context,
    std::map<std::string, bool>& opened,
    size_t filePos,
    std::map<std::string, Definition>& defs) {

    std::string name;
    size_t pos = resbuffer.find_first_of('\"');
    size_t posEnd = resbuffer.find_last_of('\"');

    if (pos != resbuffer.npos) {
        if (pos < posEnd) {

            resbuffer.erase(0, pos+1);
            std::stringstream nameStream(resbuffer);
            std::getline(nameStream, name, '"');

            std::string res;
            nameStream >> res;
            if(!res.empty())
               throw TPTokenizerException(
                   std::string("no filename to include ")
                   +context
                   +std::string(":")
                   +itoa(filePos));
        }
        else
            throw TPTokenizerException(
                std::string("filename to include expected: ")
                +context
                +std::string(":")
                +itoa(filePos));
    }
    else {

        std::stringstream nameStream(resbuffer);
        nameStream >> name;
        if(name.empty())
            throw TPTokenizerException(
                std::string("no file name to include")
                +context
                +std::string(":")
                +itoa(filePos));

        std::string res;
        nameStream >> res;
        if(!res.empty())
            throw TPTokenizerException(
                std::string("multi-element name should be quoted")
                +context
                +std::string(":")
                +itoa(filePos));
    }

    subLoad(paths, defs, context, opened, name);
}

void TPBasicTokenizerRuleSet::readDefinition(
    std::string& resbuffer,
    std::string context, size_t filePos,
    std::map<std::string, Definition>& defs) {

    std::stringstream incl(resbuffer);

    std::string name;
    std::string val;

    incl >> name;
    if (name.empty())
        throw
            TPTokenizerException(std::string("definition name expected")
                                 +context+std::string(":")+itoa(filePos));

    std::stringbuf stdbuf;

    if(incl.eof())
        throw TPTokenizerException(std::string("definition body expected")
                                   +context+std::string(":")+itoa(filePos));

    incl.get(stdbuf,'\0');
    val = stdbuf.str();
    size_t pos = 0;

    while ((pos = val.find_first_of("#@", pos)) != val.npos) {
        size_t last = val.find_last_not_of('\\', pos - 1);
        if((last == val.npos) && (val.length() > 0))
            last = -1;

        // ???
        // \@ => @
        // \\\@ => \@
        // \\\\\@ => \\\@
        if ((pos - last) % 2 == 0) {
            if(pos - last > 2) {
                val.replace(pos-3, 3, 1, '\\');
                pos -= 2;
            }
            else
                val.erase(pos-1, 1);
        }
        ++pos;
    }

    if(defs.find(name) != defs.end())
        WARN(std::string("redefinition")
             << name
             << std::string(" in ")
             << context+std::string(":")
             << itoa(filePos));

    trim(val);

    defs.insert(std::pair<std::string, Definition>(name, Definition(val,context)));
}

void TPBasicTokenizerRuleSet::readIfDef(std::string& resbuffer, std::string context,
                                         size_t & filePos,
                                         std::map<std::string, Definition> & defs,
                                         std::list<IfElem> & ifstack) {

    readIf(resbuffer, context, filePos, defs, ifstack, true);
};


void TPBasicTokenizerRuleSet::readIfNDef(std::string& resbuffer, std::string context,
                                         size_t & filePos,
                                         std::map<std::string, Definition> & defs,
                                         std::list<IfElem> & ifstack) {

    readIf(resbuffer, context, filePos, defs, ifstack, false);
};

void TPBasicTokenizerRuleSet::readIf(
    std::string& resbuffer,
    std::string context,
    size_t & filePos,
    std::map<std::string, Definition> & defs,
    std::list<IfElem> & ifstack,
    bool expectation) {

    std::stringstream nameStream(resbuffer);
    std::string name;
    nameStream >> name;
    trim(name);

    if(name.empty())
       throw TPTokenizerException(std::string("ifdef without a condition")+context+std::string(":")+itoa(filePos));

    std::string res;
    nameStream >> res;

    if(!res.empty())
        throw TPTokenizerException(std::string("ifdef - unexpected quotes")+context+std::string(":")+itoa(filePos));

    if(openedIf && ((defs.find(name) == defs.end()) == expectation)) {
        ifstack.push_back(IfElem(true, context, filePos, TIF));
        openedIf = false;
    }
    else
        ifstack.push_back(IfElem(false, context, filePos, TIF));
};

void TPBasicTokenizerRuleSet::readElse(
    std::string context,size_t & filePos,
    std::list<IfElem> & ifstack) {

    if(!openedIf && ifstack.back().getValue()) {

        if(ifstack.back().getType() == TELSE)
            throw TPTokenizerException(std::string("too many else")
                                       +context+std::string(":")+itoa(filePos));
        openedIf = true;
        ifstack.back().setType(TELSE);
    }
    else
    {
        ifstack.back().setType(TELSE);
        ifstack.back().setValue(true);
        openedIf = false;
    }
};

void TPBasicTokenizerRuleSet::readEndIf(std::list<IfElem> & ifstack) {

    if(!openedIf && ifstack.back().getValue()) {
        openedIf = true;
    }

    ifstack.pop_back();
};

void TPBasicTokenizerRuleSet::parseLine(
    std::string& resbuffer,
    std::map<std::string, Definition>& defs,
    std::map<std::string, std::string>& paths,
    std::string context,
    std::map<std::string, bool>& opened,
    size_t& filePos,
    std::list<IfElem>& ifstack) {

    if (resbuffer.length() > 5 && resbuffer.substr(0,5).compare("rule ") == 0) {
        resbuffer = resbuffer.erase(0,5);
        if(openedIf)
            readRule(resbuffer, context, filePos);
    }
    else if (resbuffer.length() > 8 && resbuffer.substr(0,8).compare("include ") == 0) {
        resbuffer = resbuffer.erase(0,7);
        if(openedIf)
            readInclude(resbuffer, paths, context, opened, filePos, defs);
    }
    else if(resbuffer.length() > 4 && resbuffer.substr(0,4).compare("def ") == 0) {
        resbuffer = resbuffer.erase(0,4);
        if(openedIf)
            readDefinition(resbuffer, context, filePos, defs);
    }
    else if(resbuffer.length() > 6 && resbuffer.substr(0,6).compare("ifdef ") == 0) {
        resbuffer = resbuffer.erase(0,6);
        readIfDef(resbuffer, context, filePos, defs, ifstack);
    }
    else if(resbuffer.length() > 7 && resbuffer.substr(0,7).compare("ifndef ") == 0) {
        resbuffer = resbuffer.erase(0,7);
        readIfNDef(resbuffer, context, filePos, defs, ifstack);
    }
    else if(resbuffer.length() >= 4 && resbuffer.substr(0,4).compare("else") == 0) {
        resbuffer = resbuffer.erase(0,4);
        readElse(context, filePos, ifstack);
    }
    else if(resbuffer.length() >= 5 && resbuffer.substr(0,5).compare("endif") == 0) {
        resbuffer = resbuffer.erase(0,5);
        readEndIf(ifstack);
    }
    else
        throw TPTokenizerException("unknown command "+ resbuffer +" in file " + context + "\n");
}

void TPBasicTokenizerRuleSet::subLoad(
    std::map<std::string, std::string>& paths,
    std::map<std::string, Definition>& defs,
    std::string context,
    std::map<std::string, bool>& opened,
    std::string& toLoad) {

    std::string buffer;

    std::string contextB = context;
    std::map<std::string, std::string>::iterator i_path = paths.find(toLoad);

    bool firstReadEmpty = true;

    if (i_path != paths.end()) {
        if (opened.find(toLoad) == opened.end()) {

            opened.insert(std::pair<std::string, bool>(toLoad, true));

            std::string fileName = (*i_path).second;

            INFO("trying to open `" << fileName << "'");
            std::ifstream ins(fileName.c_str());

            if(ins) {
                std::list<IfElem> ifstack;
                size_t filePos = 1;

                while(!ins.eof()) {

                    size_t beginPos = filePos;
                    loadWholeElement(ins, buffer, filePos);

                    removeComments(buffer);

                    std::string tempNoLine = toLoad + "-->" + fileName +"\nw: "+ context;

                    trim(buffer);

                    if (buffer.empty()) {
                        if (firstReadEmpty)
                            firstReadEmpty = false;
                        else
                            WARN(std::string("empty @ in line:")
                                 << beginPos
                                 << fileName);
                    }
                    else {
                        std::string subst;
                        std::string substRes = applyDefinitions(buffer, defs, subst);

                        if(!substRes.empty())
                            throw TPTokenizerException(
                                std::string("definition loop: ")
                                + substRes
                                + std::string("in line: ") + itoa(beginPos)
                                + std::string(" file:") + fileName);

                        std::stringstream st(buffer);
                        std::string tempContext = toLoad + "-->" + fileName +":"+itoa(beginPos)+"\nw: "+ context + subst;
                        std::string resBuffer;

                        while (!st.eof()) {
                            loadWholeElement(st, resBuffer);
                            removeComments(resBuffer);

                            if (resBuffer.empty()) {
                                if (firstReadEmpty)
                                    firstReadEmpty = false;
                                else
                                    WARN("empty @ in "
                                         << beginPos << fileName);
                            }
                            else {
                                std::string subst;
                                std::string substRes = applyDefinitions(resBuffer, defs, subst);
                                if (!substRes.empty())
                                    throw TPTokenizerException(
                                        std::string("definition loop: ")
                                        + substRes
                                        + std::string("in line: ") + itoa(beginPos)
                                        + std::string(" file:") + fileName);

                                returnToTheBuffer(st,resBuffer);
                                resBuffer = "";
                                loadWholeElement(st,resBuffer);
                                tempContext +=subst;

                                if (firstReadEmpty)
                                    throw
                                        TPTokenizerException("text with no @rule/@include/@def: "
                                                             "::"+itoa(beginPos)+" file: "+context+"\n");

                                parseLine(resBuffer,
                                          defs,
                                          paths,
                                          tempContext,
                                          opened,
                                          filePos,
                                          ifstack);

                                tempContext += "(after definition substitution)";
                            }
                        }
                    }
                }

                ins.close();

                if (!ifstack.empty())
                    throw TPTokenizerException(
                        std::string("if not closed \" \" ")
                        +ifstack.back().getContext()
                        +std::string(":")+itoa(ifstack.back().getPos()));
            }
            else
                throw TPTokenizerException(
                    std::string("base file cannot be opened: `")
                    + fileName + "'");
        }
        else
            throw TPTokenizerException(std::string("include loop:\n")+contextB);
    }
    else
        throw TPTokenizerException(
            std::string("cannot be found in the register: `")
            + toLoad + "'" + contextB);
}

void TPBasicTokenizerRuleSet::load(
    std::map<std::string, std::string>& paths,
    std::list<std::string>& filesToLoad) {

    std::string buffer;
    std::map<std::string, Definition> definitions;
    std::map<std::string, bool> opened;

    try {

        for(std::list<std::string>::iterator i_toBeLoaded = filesToLoad.begin();
            i_toBeLoaded  != filesToLoad.end();
            ++i_toBeLoaded) {

            INFO("trying to load `" << (*i_toBeLoaded) << "'");

            clearState(definitions, opened);

            std::map<std::string, std::string>::iterator i_path = paths.find((*i_toBeLoaded));

            //potencjalny brak odwzorowania - warning ????
            std::string context = (*i_toBeLoaded);

            bool firstReadEmpty = true;

            if (i_path != paths.end()) {

                std::string fileName = (*i_path).second;

                context += "-->" + fileName;

                INFO("trying to open `" << fileName << "'");
                std::ifstream ins(fileName.c_str());

                if (ins) {
                    std::list<IfElem> ifstack;
                    size_t filePos = 1;

                    while(!ins.eof()) {

                        size_t beginPos = filePos;
                        loadWholeElement(ins, buffer, filePos);

                        removeComments(buffer);

                        trim(buffer);

                        if(buffer.empty()) {
                            if(firstReadEmpty)
                                firstReadEmpty = false;
                        }
                        else {
                            if(firstReadEmpty)
                                throw
                                    TPTokenizerException("text with no @rule/@include/@def: "
                                                         +itoa(beginPos)+" file: "+context+"\n");

                            std::string subst;;
                            std::string substRes = applyDefinitions(buffer, definitions, subst);
                            if (!substRes.empty())
                                throw TPTokenizerException(
                                    std::string("definition loop: ")
                                    + substRes
                                    + std::string("in line: ") + itoa(beginPos)
                                    + std::string(" file:") + fileName);

                            // there might be new @s
                            std::stringstream st(buffer);
                            std::string tempContext = context + ":"+itoa(beginPos) + subst;
                            std::string resBuffer;

                            while (!st.eof()) {
                                loadWholeElement(st, resBuffer);
                                removeComments(resBuffer);

                                if(resBuffer.empty()) {
                                    WARN("empty @ in "
                                         << beginPos << fileName);
                                }
                                else {
                                    std::string subst;
                                    std::string substRes = applyDefinitions(resBuffer,
                                                                            definitions,
                                                                            subst);

                                    if (!substRes.empty())
                                        throw TPTokenizerException(
                                            std::string("definition loop: ")+substRes+
                                            std::string("in line: ")+itoa(beginPos)
                                            +std::string(" file:")+fileName);


                                    returnToTheBuffer(st,resBuffer);
                                    resBuffer = "";
                                    loadWholeElement(st,resBuffer);
                                    tempContext += subst;

                                    parseLine(resBuffer,
                                              definitions,
                                              paths,
                                              tempContext,
                                              opened,
                                              beginPos,
                                              ifstack);
                                }
                            };
                        }
                    }

                    ins.close();

                    if (!ifstack.empty())
                        throw TPTokenizerException(
                            std::string("if not closed \" \" ")
                            +ifstack.back().getContext()
                            +std::string(":")+itoa(ifstack.back().getPos()));
                }
                else
                    throw TPTokenizerException(
                        std::string("base file cannot be opened: `")
                        + fileName + "'");
            }
            else
                throw TPTokenizerException(
                    std::string("cannot be found in the register: `")
                    + (*i_toBeLoaded) + "'");
        }

        if(!mainRegex)
            throw TPTokenizerException(std::string("no main_regex rule"));
    }
    catch (TPTokenizerException& ex) {
        FATAL(std::string(ex.what()));
        clearState(definitions, opened);
        throw;
    }

    clearState(definitions, opened);
    reverse(regs.begin(),regs.end());
}

bool TPBasicTokenizerRuleSet::load(std::string path) {
    std::map<std::string, std::string> paths;
    paths["main"] = path;

    std::list<std::string> filesToLoad;
    filesToLoad.push_back("main");

    load(paths,filesToLoad);

    return true;
}

TPBasicTokenizerRuleSet::TPBasicTokenizerRuleSet() {
    openedIf = true;
}

TPBasicTokenizerRuleSet::~TPBasicTokenizerRuleSet() {
}

PerlRegExp* TPBasicTokenizerRuleSet::getMainRegex() {
    return mainRegex.get();
}

PerlRegExp* TPBasicTokenizerRuleSet::getRegex(size_t i) {
    return regs[i].reg.get();
}

PerlRegExp* TPBasicTokenizerRuleSet::getRegex(std::string &name) {
    for (size_t i=0;i<regs.size();++i) {
        if (regs[i].name == name)
            return regs[i].reg.get();
    }
    return NULL;
}

std::string TPBasicTokenizerRuleSet::getRegexName(size_t i) const {
        return regs[i].name;
}

std::string TPBasicTokenizerRuleSet::getRegexCategory(size_t i) const {
        return regs[i].category;
}

std::string TPBasicTokenizerRuleSet::getRegexCategory(std::string &name) const {
    for(size_t i=0; i<regs.size(); ++i) {
        if(regs[i].name == name) return regs[i].category;
    }
    return std::string("");
}

void TPBasicTokenizerRuleSet::addRegex(
    boost::shared_ptr<PerlRegExp> regex, std::string &category, std::string &name) {
        _regex r;
        r.reg = regex;
        r.category = category;
        r.name = name;
        regs.push_back(r);
}

size_t TPBasicTokenizerRuleSet::getRegexCount() const {
        return regs.size();
}

const size_t TPBasicTokenizerRuleSet::MAXSTACKSIZE = 15;

const std::string TPBasicTokenizerRuleSet::MAIN_REGEX_NAME = "main_regex";

