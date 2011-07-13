
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
    data = new std::map<std::string, std::string>;

    regSeparator = "\\s+";
    regDigit = "\\d+(\\.\\d+)?";
    regPunct = "[\\.,;:!\\?<>&\\-\\(\\)\\[\\]\"`'\\+\\*\\|~]+";
    regInterpSeparator = "; ";
//    regEscAmp = "(&amp;)";
//    regEscLt = "(&lt;)";
//    regEscGt = "(&gt;)";
    regAmp = "&";
    regLt = "<";
    regGt = ">";
    regLPar = "\\(";
    regRPar = "\\)";
    regAlt = "\\|";
    regPlus = "\\+";
    regAsterisk = "\\*";
    regOpt = "\\?";

    regSep = "\\|";
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

void Tagger::loadDictionary(std::string &filename)
{
    std::ifstream input(filename.c_str());
    boost::archive::text_iarchive ia(input);
    ia >> *data;
    input.close();
}

void Tagger::setTagset(TagsetPtr aTagset)
{
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

std::string Tagger::tagSentence(std::string sentence, Entities &entities, Edges &edges)
{
    std::stringstream cs;
    std::stringstream ss;   //pomocniczy one
    int tokenCount = 0;

    regSeparator = "\\s+";

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

    boost::regex_token_iterator<std::string::iterator> u(sentence.begin(), sentence.end(), regSeparator, -1);
    boost::regex_token_iterator<std::string::iterator> v;
    while (u != v)
    {
        std::string token = *u;

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

        if (boost::regex_match(token, regPunct))
        {
            token = boost::regex_replace(token, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regLt, "\\&lt;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regGt, "\\&gt;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);

            compiledToken += "<" + token;
            tok->setOrth(token);
            ti->setLabel(token);

            std::string pos = "";
            if (!boost::regex_match(token, regDigit))
            {
                pos = "interp";
            }
            else
            {
                pos = "ign";
            }
            std::string mapped = tagset->mapMorphology(pos);
            if (mapped == "")
            {
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
        }
        else
        {
            bool orthSet = false;

            token = boost::regex_replace(token, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regLt, "\\&lt;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regGt, "\\&gt;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
            token = boost::regex_replace(token, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);

//            *compiledToken += "<" + token;
//            tok->setOrth(token);
//            ti->setLabel(token);

            std::string lowcase = token;
            UnicodeString valL = icuConverter::fromUtf(token);
            valL.toLower();
            StringCharacterIterator itL(valL);
            std::stringstream ss;
            while (itL.hasNext())
            {
                UnicodeString tl = itL.current();
                ss << icuConverter::toUtf(tl);
                itL.next();
            }
            lowcase = ss.str();
            std::string titlecase = token;
            UnicodeString valUf = icuConverter::fromUtf(token);
            valUf.toTitle(0);
            StringCharacterIterator itUf(valUf);
            std::stringstream ss2;
            while (itUf.hasNext())
            {
                UnicodeString tuf = itUf.current();
                ss2 << icuConverter::toUtf(tuf);
                itUf.next();
            }
            titlecase = ss2.str();

////zmiana przez Państwo, Parlament, 20.07.2009
//            if (lowcase != token)
//            {
//                if (data->find(lowcase) != data->end())
//                    token = lowcase;
//            }

            //std::transform(token.begin(), token.end(), lowcase.begin(), ::tolower);

            std::map<std::string, std::string>::iterator it = data->find(token);
            if (it != data->end())
            {
                orthSet = true;
                compiledToken += "<" + token;
                tok->setOrth(token);
                ti->setLabel(token);

                std::string interps = it->second;
                boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
                boost::regex_token_iterator<std::string::iterator> j;
                while (i != j)
                {
                    std::string interp = *i;
                    boost::algorithm::trim(interp);
                    std::string base = interp.substr(0, interp.find(":", 1));
                    std::string ctag = interp.substr(interp.find(":", 1) + 1, std::string::npos);
                    std::string mapped = tagset->mapMorphology(ctag);
                    if (mapped == "")
                    {
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

                    i ++;
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
                    if (mapped == "")
                    {
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

            if (token != lowcase)
            {
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
                    boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
                    boost::regex_token_iterator<std::string::iterator> j;
                    while (i != j)
                    {
                        std::string interp = *i;
                        boost::algorithm::trim(interp);
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
                        i ++;
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
                    boost::regex_token_iterator<std::string::iterator> i(interps.begin(), interps.end(), regInterpSeparator, -1);
                    boost::regex_token_iterator<std::string::iterator> j;
                    while (i != j)
                    {
                        std::string interp = *i;
                        boost::algorithm::trim(interp);
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

                        PosInfo posInfo(base, ctag, 1);
                        ti->addMorphology(posInfo);
                        i ++;
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

        u ++;
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

std::string Tagger::tagSentenceReadable(std::string sentence, Entities &entities, Edges &edges)
{
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

std::string Tagger::processInput(std::string &sentence, Entities &entities, Edges &edges)
{
    std::stringstream cs;
    std::stringstream ss;   //pomocniczy one
    int tokenCount = 0;

    regSeparator = "\\s+";

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

    boost::regex_token_iterator<std::string::iterator> u(sentence.begin(), sentence.end(), regSeparator, -1);
    boost::regex_token_iterator<std::string::iterator> v;
    while (u != v)
    {
        std::string token = *u;

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
            u ++;
            continue;
        }


        std::string grapheme = token.substr(0, pos);
        std::string info = token.substr(pos + 3, std::string::npos);

        grapheme = boost::regex_replace(grapheme, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regLt, "\\&lt;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regGt, "\\&gt;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);
        grapheme = boost::regex_replace(grapheme, regAlt, "\\&bar;", boost::match_default | boost::format_sed);

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

        compiledToken += "<" + grapheme;
        tok->setOrth(grapheme);
        ti->setLabel(grapheme);

        if (info.size() > 1 && info[0] == '|')
        {
            info = "&bar;" + info.substr(1, std::string::npos);
        }

        boost::regex_token_iterator<std::string::iterator> x(info.begin(), info.end(), regSep, -1);
        boost::regex_token_iterator<std::string::iterator> y;
        bool first_interp = true;
        while (x != y)
        {
            std::string interp = *x;
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
            base = boost::regex_replace(base, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regLt, "\\&lt;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regGt, "\\&gt;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);
            base = boost::regex_replace(base, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
            std::string ctag = interp.substr(interp.find("#", 1) + 1, std::string::npos);
            std::string mapped = tagset->mapMorphology(ctag);
            if (mapped == "")
            {
                mapped = emptyMorphology;
                //std::cerr << "Nie ma skompilowanej interpretacji morfologicznej: " << ctag << " dla " << grapheme << "!!!!!!" << std::endl;
            }
            std::string morpho = base + ":" + ctag;
            std::string compiled = mapped + base;
            tok->addInterpretation(morpho, compiled);
            compiledToken += "<" + mapped + base;
            //delete morpho;
            //delete compiled;
            //delete mapped;

            PosInfo posInfo(base, ctag, 1);
            //delete ctag;
            //delete base;
            ti->addMorphology(posInfo);
            x ++;
        }

        compiledToken += ">";

        tok->setCompiled(compiledToken);
        cs << compiledToken;

        //delete id;
        //delete compiledToken;

        entities.push_back(tok);
        edges.push_back(ti);

        tokenCount ++;

        u ++;
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

}

}

}
