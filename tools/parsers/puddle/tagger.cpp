
#include "tagger.hpp"

#include <cstdlib>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Tagger::Tagger()
{
    //@todo: przeniesc to do jakiejs funkcji init_szatan
    data = new std::map<std::string, std::string>;

    //regSeparator = PatternPtr(new RE2("\\s+"));
    regDigit = PatternPtr(new RE2("\\d+(\\.\\d+)?"));
    regPunct = PatternPtr(new RE2("[\\.,;:!\\?<>&\\-\\(\\)\\[\\]\"`'\\+\\*\\|~]+"));
    interpSeparator = ";";
//    regEscAmp = "(&amp;)";
//    regEscLt = "(&lt;)";
//    regEscGt = "(&gt;)";
    regAmp = PatternPtr(new RE2("&"));
    regLt = PatternPtr(new RE2("<"));
    regGt = PatternPtr(new RE2(">"));
    regLPar = PatternPtr(new RE2("\\("));
    regRPar = PatternPtr(new RE2("\\)"));
    regAlt = PatternPtr(new RE2("\\|"));
    regPlus = PatternPtr(new RE2("\\+"));
    regAsterisk = PatternPtr(new RE2("\\*"));
    regOpt = PatternPtr(new RE2("\\?"));

//    regEscLPar = "&lpar;";
//    regEscRPar = "&rpar;";

    //tagset = new Tagset();
    emptyMorphology = "";
}

Tagger::~Tagger()
{
    delete data;
    //delete emptyMorphology;
//    delete tagset;
}

void Tagger::loadDictionary(std::string &filename) {
    std::ifstream input(filename.c_str());
    boost::archive::text_iarchive ia(input);
    ia >> *data;
    input.close();
}

void Tagger::setTagset(TagsetPtr aTagset) {
    //delete tagset;
    tagset = aTagset;
    emptyMorphology = "";
    int n = tagset->getNumberOfAttributes();
    int i = 0;
    while (i < n)
    {
        emptyMorphology += "0";
        i ++;
    }
}

std::string Tagger::tagSentence(std::string sentence, Entities &entities,
        Edges &edges) {
    std::stringstream cs;
    std::stringstream ss;   //pomocniczy one
    int tokenCount = 0;

    //regSeparator = "\\s+";

    SpecialToken *sb = new SpecialToken();
    std::string id = "0";
    sb->setId(id);
    //delete id;
    std::string tokType = "sb";
    sb->setTokenType(tokType);
    //delete tokType;
    std::string comp = "<<s<0<sb<>";
    sb->setCompiled(comp);
    //delete comp;
    entities.push_back(sb);

    cs << "<<s<0<sb<>";
    tokenCount ++;

    std::string tokSeparator = " ";
    std::vector<std::string> tokensVector;
    boost::split(tokensVector, sentence, boost::is_any_of(tokSeparator));
    //boost::regex_token_iterator<std::string::iterator> u(sentence.begin(), sentence.end(), regSeparator, -1);
    //boost::regex_token_iterator<std::string::iterator> v;
    //while (u != v)
    for (std::vector<std::string>::iterator tok_it = tokensVector.begin();
            tok_it != tokensVector.end(); tok_it ++) {
        std::string token = boost::trim_copy(*tok_it);

        std::string compiledToken = "<<t";
        std::stringstream sss;// = new std::stringstream;
        sss << std::hex << tokenCount;
        std::string id = sss.str();
        //delete sss;
        compiledToken += "<" + id;

        Token *tok = new Token();
        tok->setId(id);

        tok->clearMorphology(); // a to, to konieczne?
        tok->clearCompiledInterpretations(); //jw

        TransitionInfo *ti = new TransitionInfo("token");
        ti->setStart(tokenCount - 1);
        ti->setEnd(tokenCount);
        ti->setDepth(0);
        ti->setId(id);

        //if (boost::regex_match(token, regPunct))
        if (RE2::FullMatch(token, *regPunct)) {
//            token = boost::regex_replace(token, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regLt, "\\&lt;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regGt, "\\&gt;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
//            token = boost::regex_replace(token, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);
            //@todo: przeniesc takie rzeczy do jakiejs ogolnej metody
            RE2::GlobalReplace(&token, *regAmp, "\\&amp;");
            RE2::GlobalReplace(&token, *regLt, "\\&lt;");
            RE2::GlobalReplace(&token, *regGt, "\\&gt;");
            RE2::GlobalReplace(&token, *regLPar, "\\&lpar;");
            RE2::GlobalReplace(&token, *regRPar, "\\&rpar;");
            RE2::GlobalReplace(&token, *regAlt, "\\&bar;");
            RE2::GlobalReplace(&token, *regPlus, "\\&plus;");
            RE2::GlobalReplace(&token, *regAsterisk, "\\&astr;");
            RE2::GlobalReplace(&token, *regOpt, "\\&qmark;");

            compiledToken += "<" + token;
            tok->setOrth(token);
            ti->setLabel(token);

            std::string pos = "";
            if (!RE2::FullMatch(token, *regDigit)) {
                pos = "interp";
            } else {
                pos = "ign";
            }
            std::string mapped = tagset->mapMorphology(pos);
            if (mapped == "") {
                mapped = emptyMorphology;
                //                        std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << orth << "!!!!!!" << std::endl;
            }
            std::string morpho = token + ":" + pos;
            std::string compiled = mapped + token;
            tok->addInterpretation(morpho, compiled);
            compiledToken += "<" + mapped + token;
            //delete morpho;
            //delete compiled;
            //delete mapped;

            PosInfo posInfo(token, pos, 1);

//            if (tagset->containsDesc())
//            {
//                std::vector<int> desc = tagset->getDesc(*pos);
//                ti->setDesc(desc);
//            }

            //delete pos;
            ti->addMorphology(posInfo);
        } else {
            bool orthSet = false;

            RE2::GlobalReplace(&token, *regAmp, "\\&amp;");
            RE2::GlobalReplace(&token, *regLt, "\\&lt;");
            RE2::GlobalReplace(&token, *regGt, "\\&gt;");
            RE2::GlobalReplace(&token, *regLPar, "\\&lpar;");
            RE2::GlobalReplace(&token, *regRPar, "\\&rpar;");
            RE2::GlobalReplace(&token, *regAlt, "\\&bar;");
            RE2::GlobalReplace(&token, *regPlus, "\\&plus;");
            RE2::GlobalReplace(&token, *regAsterisk, "\\&astr;");
            RE2::GlobalReplace(&token, *regOpt, "\\&qmark;");

//            *compiledToken += "<" + token;
//            tok->setOrth(token);
//            ti->setLabel(token);

            std::string lowcase = token;
            boost::to_lower(lowcase);
//            UnicodeString valL = icuConverter::fromUtf(token);
//            valL.toLower();
//            StringCharacterIterator itL(valL);
//            std::stringstream ss;
//            while (itL.hasNext())
//            {
//                UnicodeString tl = itL.current();
//                ss << icuConverter::toUtf(tl);
//                itL.next();
//            }
//            lowcase = ss.str();
            //std::string titlecase = token;
            std::string titlecase = token.substr(0,1);
            boost::to_upper(titlecase);
            if (token.size() > 1)
                titlecase += token.substr(1, std::string::npos);
//            UnicodeString valUf = icuConverter::fromUtf(token);
//            valUf.toTitle(0);
//            StringCharacterIterator itUf(valUf);
//            std::stringstream ss2;
//            while (itUf.hasNext())
//            {
//                UnicodeString tuf = itUf.current();
//                ss2 << icuConverter::toUtf(tuf);
//                itUf.next();
//            }
//            titlecase = ss2.str();

////zmiana przez Państwo, Parlament, 20.07.2009
//            if (lowcase != token)
//            {
//                if (data->find(lowcase) != data->end())
//                    token = lowcase;
//            }

            //std::transform(token.begin(), token.end(), lowcase.begin(), ::tolower);

            std::map<std::string, std::string>::iterator it = data->find(token);
            if (it != data->end()) {
                orthSet = true;
                compiledToken += "<" + token;
                tok->setOrth(token);
                ti->setLabel(token);

                std::string interps = it->second;
                std::vector<std::string> interpsVector;
                boost::split(interpsVector, interps, boost::is_any_of(interpSeparator));
                //boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
                //boost::regex_token_iterator<std::string::iterator> j;
                //while (i != j)
                for (std::vector<std::string>::iterator interp_it = interpsVector.begin();
                        interp_it != interpsVector.end(); interp_it ++ ) {
                    std::string interp = boost::algorithm::trim_copy(*interp_it);
                    //boost::algorithm::trim(interp);
                    std::string base = interp.substr(0, interp.find(":", 1));
                    std::string ctag = interp.substr(interp.find(":", 1) + 1,
                                                     std::string::npos);
                    std::string mapped = tagset->mapMorphology(ctag);
                    if (mapped == "") {
                        mapped = emptyMorphology;
//                        std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << token << "!!!!!!" << std::endl;
                    }
                    std::string morpho = base;
                    morpho += ":" + ctag;
                    std::string compiled = mapped;
                    compiled += base;
                    tok->addInterpretation(morpho, compiled);
                    compiledToken += "<" + mapped + base;
                    //delete morpho;
                    //delete compiled;
                    //delete mapped;

//                    std::string pos = ctag->substr(0, ctag->find(":"));
//                    if (tagset->containsDesc())
//                    {
//                        std::vector<int> desc = tagset->getDesc(pos);
//                        ti->setDesc(desc);
//                    }

                    PosInfo posInfo(base, ctag, 1);
                    //delete base;
                    //delete ctag;
                    ti->addMorphology(posInfo);

                    //i ++;
                }
            }
            else
            {
                if ((data->find(lowcase) == data->end()) && (data->find(titlecase) == data->end()))
                {
                    compiledToken += "<" + token;
                    tok->setOrth(token);
                    ti->setLabel(token);
                    orthSet = true;

                    std::string pos = "ign";
                    std::string mapped = tagset->mapMorphology(pos);
                    if (mapped == "") {
                        mapped = emptyMorphology;
//                                                std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << orth << "!!!!!!" << std::endl;
                    }
                    std::string morpho = token + ":" + pos;
                    std::string compiled = mapped + token;
                    tok->addInterpretation(morpho, compiled);
                    compiledToken += "<" + mapped + token;
                    //delete morpho;
                    //delete compiled;
                    //delete mapped;

                    PosInfo posInfo(token, pos, 1);

//                    if (tagset->containsDesc())
//                    {
//                        std::vector<int> desc = tagset->getDesc(*pos);
//                        ti->setDesc(desc);
//                    }

                    //delete pos;
                    ti->addMorphology(posInfo);
                }
            }

            if (token != lowcase) {
                it = data->find(lowcase);
                if (it != data->end())
                {
                    if (!orthSet)
                    {
//                        UnicodeString val = icuConverter::fromUtf(token);
//                        val.toLower();
//                        token = icuConverter::toUtf(val);
                        compiledToken += "<" + token;
                        tok->setOrth(token);
                        ti->setLabel(token);
                        orthSet = true;
                    }

                    std::string interps = it->second;
                    std::vector<std::string> interpsVector;
                    boost::split(interpsVector, interps, boost::is_any_of(interpSeparator));
//                    boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
//                    boost::regex_token_iterator<std::string::iterator> j;
//                    while (i != j)
                    for (std::vector<std::string>::iterator interp_it = interpsVector.begin();
                            interp_it != interpsVector.end(); interp_it ++) {
                        std::string interp = boost::algorithm::trim_copy(*interp_it);
                        //boost::algorithm::trim(interp);
                        std::string base = interp.substr(0, interp.find(":", 1));
                        std::string ctag = interp.substr(interp.find(":", 1) + 1, std::string::npos);
                        std::string mapped = tagset->mapMorphology(ctag);
                        if (mapped == "")
                        {
                            mapped = emptyMorphology;
                            //std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << orth << "!!!!!!" << std::endl;
                        }
                        std::string morpho = base + ":" + ctag;
                        std::string compiled = mapped + base;
                        tok->addInterpretation(morpho, compiled);
                        compiledToken += "<" + mapped + base;
                        //delete morpho;
                        //delete compiled;
                        //delete mapped;

//                        std::string pos = ctag->substr(0, ctag->find(":"));
//                        if (tagset->containsDesc())
//                        {
//                            std::vector<int> desc = tagset->getDesc(pos);
//                            ti->setDesc(desc);
//                        }

                        PosInfo posInfo(base, ctag, 1);
                        //delete ctag;
                        //delete base;
                        ti->addMorphology(posInfo);
                        //i ++;
                    }
                }
            }
            if (token != titlecase)
            {
                it = data->find(titlecase);
                if (it != data->end())
                {
                    if (!orthSet)
                    {
                        compiledToken += "<" + token;
                        tok->setOrth(token);
                        ti->setLabel(token);
                        orthSet = true;
                    }

                    std::string interps = it->second;
                    std::vector<std::string> interpsVector;
                    boost::split(interpsVector, interps, boost::is_any_of(interpSeparator));
                    //boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
                    //boost::regex_token_iterator<std::string::iterator> j;
                    //while (i != j)
                    for (std::vector<std::string>::iterator interp_it = interpsVector.begin();
                            interp_it != interpsVector.end(); interp_it ++) {
                        std::string interp = boost::algorithm::trim_copy(*interp_it);;
                        //boost::algorithm::trim(interp);
                        std::string base = interp.substr(0, interp.find(":", 1));
                        std::string ctag = interp.substr(interp.find(":", 1) + 1, std::string::npos);
                        std::string mapped = tagset->mapMorphology(ctag);
                        if (mapped == "") {
                            mapped = emptyMorphology;
                            //std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << orth << "!!!!!!" << std::endl;
                        }
                        std::string morpho = base + ":" + ctag;
                        std::string compiled = mapped + base;
                        tok->addInterpretation(morpho, compiled);
                        compiledToken += "<" + mapped + base;

                        PosInfo posInfo(base, ctag, 1);
                        ti->addMorphology(posInfo);
       //                 i ++;
                    }
                }
            }

        }

        compiledToken += ">";

        tok->setCompiled(compiledToken);
        cs << compiledToken;

        //delete id;
        //delete compiledToken;

        entities.push_back(tok);
        edges.push_back(ti);

        tokenCount ++;

     //   u ++;
    }

    SpecialToken *se = new SpecialToken();
    ss << std::hex << tokenCount;  //no to jest rozwiazane w sposob zenujacy!
    id = ss.str();
    se->setId(id);
    tokType = "se";
    se->setTokenType(tokType);
    //delete tokType;
    comp = "<<s<" + id + "<se<>";
    //delete id;
    se->setCompiled(comp);
    entities.push_back(se);

    cs << comp;
    //delete comp;
    tokenCount ++;

    Group::groupId = tokenCount - 1;

    return cs.str();
}

std::string Tagger::tagSentenceReadable(std::string sentence,
        Entities &entities, Edges &edges) {
    std::string tagged = tagSentence(sentence, entities, edges);
    std::string readable = getReadable(entities);
    return readable;
}

std::string Tagger::getReadable(Entities entities)
{
    std::stringstream ss;
    bool first = true;
    for (Entities::iterator e = entities.begin(); e != entities.end(); e++)
    {
        if ((*e)->getType() == "token")
        {
            if (! first)
                ss << " ";
            ss << ((Token*)(*e))->getReadable();
            first = false;
        }
    }
    return ss.str();
}

//@todo: przepisac funkcje 'tagujace' na uzywanie stringstream a nie jakies dziwactwa ze stringami i wskaznikami
std::string Tagger::processInput(std::string &sentence, Entities &entities, Edges &edges)
{
    std::stringstream cs;
    std::stringstream ss;   //pomocniczy one
    int tokenCount = 0;

    //regSeparator = "\\s+";

    SpecialToken *sb = new SpecialToken();
    std::string id = "0";
    sb->setId(id);
    //delete id;
    std::string tokType = "sb";
    sb->setTokenType(tokType);
    //delete tokType;
    std::string comp = "<<s<0<sb<>";
    sb->setCompiled(comp);
    //delete comp;
    entities.push_back(sb);

    cs << "<<s<0<sb<>";
    tokenCount ++;

    std::string tokSeparator = " ";
    std::vector<std::string> tokensVector;
    boost::split(tokensVector, sentence, boost::is_any_of(tokSeparator));
    //boost::regex_token_iterator<std::string::iterator> u(sentence.begin(), sentence.end(), regSeparator, -1);
    //boost::regex_token_iterator<std::string::iterator> v;
    //while (u != v)
    for (std::vector<std::string>::iterator tok_it = tokensVector.begin();
            tok_it != tokensVector.end(); tok_it ++) {
        std::string token = boost::algorithm::trim_copy(*tok_it);

        std::stringstream sst;

        int pos = token.find("|||");
        if (pos == 0)
        {
            if (token.find("|||", 1) == 1)
            {
                pos = 1; //Zabezpieczenie przed |||||#interp
            }
        }
        if (pos == std::string::npos)
        {
            std::cerr << "Invalid token: " << token << "." << std::endl;
            //u ++;
            continue;
        }


        std::string grapheme = token.substr(0, pos);
        std::string info = token.substr(pos + 3, std::string::npos);

        RE2::GlobalReplace(&grapheme, *regAmp, "\\&amp;");
        RE2::GlobalReplace(&grapheme, *regLt, "\\&lt;");
        RE2::GlobalReplace(&grapheme, *regGt, "\\&gt;");
        RE2::GlobalReplace(&grapheme, *regLPar, "\\&lpar;");
        RE2::GlobalReplace(&grapheme, *regRPar, "\\&rpar;");
        RE2::GlobalReplace(&grapheme, *regPlus, "\\&plus;");
        RE2::GlobalReplace(&grapheme, *regAsterisk, "\\&astr;");
        RE2::GlobalReplace(&grapheme, *regOpt, "\\&qmark;");
        RE2::GlobalReplace(&grapheme, *regAlt, "\\&bar;");

        //std::string compiledToken = "<<t";
        sst << "<<t";
        std::stringstream sss;// = new std::stringstream;
        sss << std::hex << tokenCount;
        std::string id = sss.str();
        //delete sss;
        /*compiledToken += "<" + id;*/
        //@todo: tu robimy nowy format skompilowanego tokenu. zamiast id jest: from, to, type (TOKEN)
        sst << "<" << (tokenCount - 1); // from  @todo: (przerobic na zwykle getStart, getEnd itd
        sst << "<" << tokenCount;       // to
        sst << "<" << "TOKEN";          //type


        Token *tok = new Token();
        tok->setId(id);

        tok->clearMorphology(); // a to, to konieczne?
        tok->clearCompiledInterpretations(); //jw

        TransitionInfo *ti = new TransitionInfo("token");
        ti->setStart(tokenCount - 1);
        ti->setEnd(tokenCount);
        ti->setDepth(0);
        ti->setId(id);

        //compiledToken += "<" + grapheme;
        sst << "<" << grapheme;
        tok->setOrth(grapheme);
        ti->setLabel(grapheme);

        if (info.size() > 1 && info[0] == '|')
        {
            info = "&bar;" + info.substr(1, std::string::npos);
        }

        std::string infoSeparator = "|";
        std::vector<std::string> infosVector;
        boost::split(infosVector, info, boost::is_any_of(infoSeparator));
        //boost::regex_token_iterator<std::string::iterator> x(info.begin(), info.end(), regSep, -1);
        //boost::regex_token_iterator<std::string::iterator> y;
        bool first_interp = true;
        //while (x != y)
        for (std::vector<std::string>::iterator interp_it = infosVector.begin();
                interp_it != infosVector.end(); interp_it ++) {
            std::string interp = boost::algorithm::trim_copy(*interp_it);
            if (first_interp)
            {
                if (interp.find("&bar;") == 0)
                {
                    interp = "|" + interp.substr(5, std::string::npos);
                }
            }
            first_interp = false;

            if ((interp.find("#", 1) == interp.length() - 1) || (interp.find("#", 1) == std::string::npos))
            {
                std::cerr << "Invalid interpretation: " << interp << "." << std::endl;
            }
            std::string base = interp.substr(0, interp.find("#", 1));
            RE2::GlobalReplace(&base, *regAmp, "\\&amp;");
            RE2::GlobalReplace(&base, *regLt, "\\&lt;");
            RE2::GlobalReplace(&base, *regGt, "\\&gt;");
            RE2::GlobalReplace(&base, *regLPar, "\\&lpar;");
            RE2::GlobalReplace(&base, *regRPar, "\\&rpar;");
            RE2::GlobalReplace(&base, *regPlus, "\\&plus;");
            RE2::GlobalReplace(&base, *regAsterisk, "\\&astr;");
            RE2::GlobalReplace(&base, *regOpt, "\\&qmark;");
            RE2::GlobalReplace(&base, *regAlt, "\\&bar;");
            std::string ctag = interp.substr(interp.find("#", 1) + 1, std::string::npos);
            std::string mapped = tagset->mapMorphology(ctag);
            if (mapped == "") {
                mapped = emptyMorphology;
                //std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << grapheme << "!!!!!!" << std::endl;
            }
            std::string morpho = base + ":" + ctag;
            std::string compiled = mapped + base;
            tok->addInterpretation(morpho, compiled);
            //compiledToken += "<" + mapped + base;
            sst << "<" << mapped << base;
            //delete morpho;
            //delete compiled;
            //delete mapped;

            PosInfo posInfo(base, ctag, 1);
            //delete ctag;
            //delete base;
            ti->addMorphology(posInfo);
            //x ++;
        }

        //compiledToken += ">";
        sst << ">";
        std::string compiledToken = sst.str();

        tok->setCompiled(compiledToken);
        cs << compiledToken;

        //delete id;
        //delete compiledToken;

        entities.push_back(tok);
        edges.push_back(ti);

        tokenCount ++;

     //   u ++;
    }

    SpecialToken *se = new SpecialToken();
    ss << std::hex << tokenCount;  //@todo: no to jest rozwiazane w sposob zenujacy! przeniesc to do jakiejs metody puddle::util
    id = ss.str();
    se->setId(id);
    tokType = "se";
    se->setTokenType(tokType);
    //delete tokType;
    comp = "<<s<" + id + "<se<>";
    //delete id;
    se->setCompiled(comp);
    entities.push_back(se);

    cs << comp;
    //delete comp;
    tokenCount ++;

    Group::groupId = tokenCount - 1;

    return cs.str();
}

}

}

}
