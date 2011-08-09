
#include "puddle.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

Puddle::Puddle()
{
//    ruleCompiler = new RuleCompiler;
    ruleMatcher = new RuleMatcher;
//    xmlParser = new XmlParser;
    graphWriter = new GraphWriter;
//    xmlWriter = new XmlWriter;
//    tagger = new Tagger;
//    newtagger = new NewTagger;
    parseConverter = new ParseConverter;
//    rules = new Rules;

//    ruleMatcher->setSyntok();

    syntok = true;
    verbose = false;
    norepeats = false;

    describe = false;

    ruleModifier = new RuleModifier;

    tagsetLogFilename = "";
    rulesLogFilename = "";
}

Puddle::~Puddle()
{
//    delete tagger;
//    delete ruleMatcher;
//    delete ruleCompiler;
//    delete xmlWriter;
    delete graphWriter;
//    delete xmlParser;

//    delete newtagger;
    delete ruleMatcher;
    delete parseConverter;

    delete ruleModifier;
    delete latticeWrapper;
//    delete rules;
}

void Puddle::setTagset(bonsai::puddle::TagsetPtr tagset_)
{
    if (tagset_->size() == 0)
    {
//        std::cerr << "Could not load tagset." << std::endl;
        return;
    }
    this->tagset = tagset_;
    describe = tagset_->containsDesc();
    //tagset->writeToFile(tagsetLogFilename);
//    ruleCompiler->setTagset(tagset);
//    newtagger->setTagset(tagset);
    parseConverter->setTagset(tagset);

    ruleMatcher->setTagset(tagset_);
    latticeWrapper = new LatticeWrapper(tagset_);
}

void Puddle::setRules(bonsai::puddle::RulesPtr rules_)
{
    this->rules = rules_;
    ruleMatcher->setRules(rules_);
    //ruleModifier->setRules(rules_);
    //std::cerr << "ustawilem reguly dla modyfikatora" << std::endl;
}

//void Puddle::setTagger(bonsai::puddle::MeTaggerPtr tagger_)
void Puddle::setTagger(bonsai::puddle::TaggerPtr tagger_)
{
    this->tagger = tagger_;
}

ParseGraphPtr Puddle::parseString(std::string line)
{
    /*Entities entities;
    Edges edges;*/
    ParseGraphPtr inputGraph = ParseGraphPtr(new ParseGraph()); //@todo: zmienic koniecznie nazwe tego

    boost::algorithm::trim(line);

//    std::cerr << "zdanie: " << line << std::endl;
    //std::string sentence = tagger->tagSentence(line, entities, edges);
    std::string sentenceString = tagger->tagSentence(line, inputGraph); //@todo: a tu sentence_string a input_pg nie ppowinny byc raczej miejscami zamienione?
//    std::cerr << "otagowane: " << sentence << std::endl;
    ParseGraphPtr pg = ruleMatcher->applyRules(sentenceString, inputGraph);
//    std::cerr << "sparsowany" << std::endl;
    if (describe)
        parseConverter->addDescription(pg);

/*    for (Entities::iterator i = entities.begin(); i != entities.end(); i++)
    {
        if ((*i)->getType() == "token")
            delete ((Token*)(*i));
        else if ((*i)->getType() == "group")
//        {
//            std::cerr << "usuwam: " << ((Group*)(*i))->getGroupType() << std::endl;
            delete ((Group*)(*i));
//        }
        else if ((*i)->getType() == "syntok")
            delete ((Syntok*)(*i));
        else if ((*i)->getType() == "special")
            delete ((SpecialToken*)(*i));
        else
            delete *i;
    }

    for (Edges::iterator i = edges.begin(); i != edges.end(); i++)
    {
//        std::cerr << "usuwam: " << (*i)->getLabel() << ", " << (*i)->getType() << std::endl;
        delete *i;
    }*/

//    graph = pg;
//    std::cerr << "przed:" << pg->write_graphviz() << std::endl;
//      std::string serialized = pg->save();
//      std::cerr << "zserializowany: " << serialized << std::endl;
//      ParseGraphPtr graph = pg;
//      graph->load(serialized);
//      std::cerr << "po: " << graph->write_graphviz() << std::endl;
//    graphWriter->setGraph(pg);
    return pg;
}

ParseGraphPtr Puddle::parseTaggedString(std::string line)
{
    /*Entities entities;
    Edges edges;*/
    ParseGraphPtr inputGraph = ParseGraphPtr(new ParseGraph()); //@todo: zmienic koniecznie nazwe tego

    boost::algorithm::trim(line);

//    std::cerr << "zdanie: " << line << std::endl;
    //std::string sentence = tagger->processInput(line, entities, edges);
    std::string sentenceString = tagger->processInput(line, inputGraph); //@todo: a tu sentence_string a input_pg nie ppowinny byc raczej miejscami zamienione?
//    std::cerr << "otagowane: " << sentence << std::endl;
    //ParseGraphPtr pg = ruleMatcher->applyRules(sentence, entities, edges);
    ParseGraphPtr pg = ruleMatcher->applyRules(sentenceString, inputGraph);
//    std::cerr << "sparsowany" << std::endl;
    if (describe)
        parseConverter->addDescription(pg);

/*    for (Entities::iterator i = entities.begin(); i != entities.end(); i++)
    {
        if ((*i)->getType() == "token")
            delete ((Token*)(*i));
        else if ((*i)->getType() == "group")
            delete ((Group*)(*i));
        else if ((*i)->getType() == "syntok")
            delete ((Syntok*)(*i));
        else if ((*i)->getType() == "special")
            delete ((SpecialToken*)(*i));
        else
            delete *i;
    }

    for (Edges::iterator i = edges.begin(); i != edges.end(); i++)
    {
        delete *i;
    }*/

//    graph = pg;
//    graphWriter->setGraph(pg);
    return pg;
}

ParseGraphPtr Puddle::parse(Lattice &lattice) {
    std::string sentenceString;
    ParseGraphPtr inputGraph = latticeWrapper->readInputLattice(lattice, sentenceString);
    ParseGraphPtr pg = ruleMatcher->applyRules(sentenceString, inputGraph);
    return pg;
}

//ParseGraphPtr Parser::parseITF(poleng::t5::ITF &itf)
//{
//    Entities entities;
//    Edges edges;
//    std::string sentence;
//
//    itfconverter->to_puddle(itf, entities, edges, sentence);
//    ParseGraphPtr pg = ruleMatcher->applyRules(sentence, entities, edges);
//
//    if (describe)
//        parseConverter->addDescription(pg);
//
//    for (Entities::iterator i = entities.begin(); i != entities.end(); i++)
//    {
//        if (*((*i)->getType()) == "token")
//            delete ((Token*)(*i));
//        else if (*((*i)->getType()) == "group")
//            delete ((Group*)(*i));
//        else if (*((*i)->getType()) == "syntok")
//            delete ((Syntok*)(*i));
//        else if (*((*i)->getType()) == "special")
//            delete ((SpecialToken*)(*i));
//        else
//            delete *i;
//    }
//
//    for (Edges::iterator i = edges.begin(); i != edges.end(); i++)
//    {
//        delete *i;
//    }
//
////    graph = pg;
////    std::cerr << "graf:" << pg->write_graphviz() << std::endl;
////      std::string serialized = pg->save();
////      std::cerr << "zserializowany: " << serialized << std::endl;
////      ParseGraphPtr graph = pg;
////      graph->load(serialized);
////      std::cerr << "po: " << graph->write_graphviz() << std::endl;
////    graphWriter->setGraph(pg);
//    return pg;
//}
//

//bool Parser::parseFile(std::string filename)
//{
//    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
//    std::cerr << "Parsing..." << std::endl;
//
//    if (verbose)
//        xmlParser->setVerbose();
//
//    xmlWriter->setOutputFile(xmlOutputFilename);
//    ruleMatcher->setXmlWriter(xmlWriter);
//
//    xmlWriter->writeBeginning();
//
//    xmlParser->parseFile(filename);
//
//   // Entities entities = xmlParser->handler->getEntities();
//    Edges edges = xmlParser->handler->getEdges();
//    //std::string compiled = xmlParser->handler->getCompiledSentence();
//
//    xmlWriter->writeEnd();
//
//    int sentencesCount = 0;
//    //ruleMatcher->applyRules(compiled, entities, edges, sentencesCount);
//    std::cerr << "Parsing finished "; // << std::endl;
//
////    Graph g;
////    Edges::iterator e = edges.begin();
////    while (e != edges.end())
////    {
////        add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
////        e ++;
////    }
////
////    graphWriter->setGraph(g);
////
//    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
//    boost::posix_time::time_duration delta = pt_end - pt_start;
//    std::cerr << "(" << delta.total_milliseconds() << " ms)." << std::endl;
////    writeGraph("output.dot");
//
//    //xmlWriter->setEntities(entities);
//    //writeXmlOutput("morphSh.xml");
//    //output?
//}

void Puddle::parseStdin()
{
    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    if (verbose)
        std::cerr << "Parsing..." << std::endl;

//    ruleMatcher->setNoxml();

//    if (verbose)
//        xmlParser->setVerbose();

//    ruleMatcher->setXmlWriter(xmlWriter);

    std::string line;
    while (getline(std::cin, line))
    {
//        boost::algorithm::trim(line);
        ParseGraphPtr pg = this->parseString(line);

      //  if (pg->num_vertices() > 0)
      //  {
      //      std::cerr << pg->shallow_path()->str() << std::endl;
      //      std::cerr << pg->deepest_path()->str() << std::endl;
      //  }
    }



   // Entities entities = xmlParser->handler->getEntities();
    //std::string compiled = xmlParser->handler->getCompiledSentence();


    int sentencesCount = 0;
    //ruleMatcher->applyRules(compiled, entities, edges, sentencesCount);
    if (verbose)
        std::cerr << "Parsing finished." << std::endl;

    if (verbose)
    {
        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration delta = pt_end - pt_start;
        std::cerr << "(" << delta.total_milliseconds() << " ms)." << std::endl;
    }
//    writeGraph("output.dot");

    //xmlWriter->setEntities(entities);
    //writeXmlOutput("morphSh.xml");
    //output?
}

bool Puddle::writeGraph(std::string filename)
{
    if (verbose)
        std::cerr << "Writing graph..." << std::endl;
    graphWriter->writeToFile(filename);
    if (verbose)
        std::cerr << "Finished." << std::endl;
    return true;
}

void Puddle::setFlag (std::string flag)
{
    if (flag == "no-syntok")
    {
        syntok = false;
        ruleMatcher->setNoSyntok();
    }
    else if (flag == "verbose")
        verbose = true;
    else if (flag == "no-repeat")
    {
        norepeats = true;
        ruleMatcher->setNoRepeats();
    }
    else if (flag == "ambig")
    {
        disamb = true;
        ruleMatcher->setDisamb();
    }
    else
    {
        std::cerr << "Unknown flag: " << flag << "." << std::endl;
    }
}

bool Puddle::getFlag (std::string flag)
{
    if (flag == "syntok")
        return syntok;
    else if (flag == "verbose")
        return verbose;
    else if (flag == "no-repeat")
        return norepeats;
    else if (flag == "ambig")
        return disamb;
    else
    {
        std::cerr << "Unknown flag: " << flag << "." << std::endl;
        return false;
    }
}

void Puddle::setTagsetLogFile(std::string filename)
{
    tagsetLogFilename = filename;
}

void Puddle::setRulesLogFile(std::string filename)
{
    rulesLogFilename = filename;
}

void Puddle::logTagset()
{
    if (tagsetLogFilename != "")
    {
        std::ofstream logfile(tagsetLogFilename.c_str());
        logfile << tagset->log();
        logfile.close();
    }
}

void Puddle::logRules()
{
    if (rulesLogFilename != "")
    {
        std::ofstream logfile(rulesLogFilename.c_str());
        for (Rules::iterator ir = rules->begin(); ir != rules->end(); ir ++)
        {
            logfile << (*ir)->log();
            logfile << std::endl;
        }
        logfile.close();
    }
}

}

}

}

