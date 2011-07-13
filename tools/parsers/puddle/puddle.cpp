
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
}

//bool Parser::loadTagset(std::string filename)
//{
//    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
//    if (verbose)
//        std::cerr << "Loading tagset..." << std::endl;
//    Tagset *tagset = new Tagset;
//    tagset->readFromFile(filename);
//    describe = false;
//    ruleCompiler->setTagset(*tagset);
////    xmlParser->handler->setTagset(tagset);
//    //std::string tagsetLogFile = "tagset.log"; //TODO: to ma byc parametr
//    tagset->writeToFile(tagsetLogFilename);
//
//    newtagger->setTagset(tagset);
//
// //   delete tagset;
//    if (verbose)
//    {
//        std::cerr << "Tagset loaded " << std::endl;
//        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
//        boost::posix_time::time_duration delta = pt_end - pt_start;
//        std::cerr << "(" << delta.total_milliseconds() << " ms)." << std::endl;
//    }
//    return true;
//}

//bool Parser::loadTagset(std::string filename, std::string descFilename)
//{
//    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
//    if (verbose)
//        std::cerr << "Loading tagset..." << std::endl;
//    Tagset *tagset = new Tagset;
//    tagset->readFromFile(filename);
//    parseConverter->loadTagset(filename, descFilename);
//    describe = true;
//    ruleCompiler->setTagset(*tagset);
////    xmlParser->handler->setTagset(tagset);
//    //std::string tagsetLogFile = "tagset.log"; //TODO: to ma byc parametr
//    tagset->writeToFile(tagsetLogFilename);
//
//    newtagger->setTagset(tagset);
//
// //   delete tagset;
//    if (verbose)
//    {
//        std::cerr << "Tagset loaded ";
//        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
//        boost::posix_time::time_duration delta = pt_end - pt_start;
//        std::cerr << "(" << delta.total_milliseconds() << " ms)." << std::endl;
//    }
//    return true;
//}

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

//bool Parser::loadRules(std::string filename)
//{
//    if (verbose)
//        ruleCompiler->setVerbose();
//    if (syntok)
//        ruleCompiler->setSyntok();
//
//    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
//    if (verbose)
//        std::cerr << "Loading rules..." << std::endl;
////    Rules *rules = new Rules;
//    RulesPtr rules = ruleCompiler->readFromFile(filename);
//    //ruleMatcher->setRules(*rules);
//    ruleMatcher->setRules(rules);
//
//    ruleModifier->setRules(rules);
//    ruleModifier->setRuleCompiler(ruleCompiler);
//
//    //ruleModifier->addRule("Rule \"siedemnasta\"\nMatch: [type=A];\nEval: transform(1, X);\n");
//    //ruleModifier->deleteRule(0);
//    //ruleModifier->moveRule(2,0);
////    ruleModifier->addAction(0, "delete(pos!~\"subst\", 1)");
////    ruleModifier->setRepeat(1);
////    ruleModifier->unsetRepeat(2);
////    ruleModifier->setMatch(1, "[pos~\"adj|subst\"] [type=AP]* ([pos~\"det\"] | [base!~\"nic\"]+)?");
////    ruleModifier->setLeftContext(0, "[orth!~\"je\"]");
////    ruleModifier->setRightContext(2, "[pos!~\"p.*\"]");
//
//    ruleCompiler->logRulesToFile(rulesLogFilename);
//
////    delete rules;
////    xmlParser->handler->setRuleMatcher(ruleMatcher);
////    xmlParser->handler->setGraphWriter(graphWriter);
//
////    delete rules;
//
//  //  std::string rulesLogFile = "rules.log"; //TODO: te parametry gdzies tam przeniesc do jakiegos konfiguracyjnego pliku
//
//    if (verbose)
//    {
//        std::cerr << "Rules loaded " << std::endl;
//
//        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
//        boost::posix_time::time_duration delta = pt_end - pt_start;
//        std::cerr << "(" << delta.total_milliseconds() << " ms)." << std::endl;
//    }
//    return true;
//}

ParseGraphPtr Puddle::parseString(std::string line)
{
    Entities entities;
    Edges edges;

    boost::algorithm::trim(line);

//    std::cerr << "zdanie: " << line << std::endl;
    std::string sentence = tagger->tagSentence(line, entities, edges);
//    std::cerr << "otagowane: " << sentence << std::endl;
    ParseGraphPtr pg = ruleMatcher->applyRules(sentence, entities, edges);
//    std::cerr << "sparsowany" << std::endl;
    if (describe)
        parseConverter->addDescription(pg);
    
    for (Entities::iterator i = entities.begin(); i != entities.end(); i++)
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
    }

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
    Entities entities;
    Edges edges;

    boost::algorithm::trim(line);

//    std::cerr << "zdanie: " << line << std::endl;
    std::string sentence = tagger->processInput(line, entities, edges);
//    std::cerr << "otagowane: " << sentence << std::endl;
    ParseGraphPtr pg = ruleMatcher->applyRules(sentence, entities, edges);
//    std::cerr << "sparsowany" << std::endl;
    if (describe)
        parseConverter->addDescription(pg);
    
    for (Entities::iterator i = entities.begin(); i != entities.end(); i++)
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
    }

//    graph = pg;
//    graphWriter->setGraph(pg);
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
        //ParseGraphPtr pg = this->parseTaggedString(line);


      //  if (pg->num_vertices() > 0)
      //  {
      //      std::cerr << pg->shallow_path()->str() << std::endl;
      //      std::cerr << pg->deepest_path()->str() << std::endl;
      //  }
//        continue;
//
////        std::cerr << "Tagging..." << std::endl;
//        std::ofstream tmpout("tagged.tmp.txt", std::ios_base::out | std::ios_base::trunc);
//        //tmpout << utf8converter::ToUtf8(utf8converter::FromUtf8(tagger->tagSentence(line)));
//        tmpout << tagger->tagSentence(line);
//        tmpout.close();
//
//        //system("./taggerpl/tagger -d ./taggerpl/slownik.pl.dat -i sentence.tmp.txt -o tagged.tmp.txt");
////        std::cerr << "Tagging finished" << std::endl;
//        xmlWriter->writeBeginning();
//        xmlParser->parseFile("tagged.tmp.txt");
//        //Edges edges = xmlParser->handler->getEdges();
//        system("rm -f tagged.tmp.txt");
//        xmlWriter->writeEnd();
    }



   // Entities entities = xmlParser->handler->getEntities();
    //std::string compiled = xmlParser->handler->getCompiledSentence();


    int sentencesCount = 0;
    //ruleMatcher->applyRules(compiled, entities, edges, sentencesCount);
    if (verbose)
        std::cerr << "Parsing finished." << std::endl;

//    Graph g;
//    Edges::iterator e = edges.begin();
//    while (e != edges.end())
//    {
//        add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
//        e ++;
//    }
//
//    graphWriter->setGraph(g);
//
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

void Puddle::writeXml(std::string filename) {
    ;
}
//bool Parser::writeXmlOutput(std::string filename)
//{
//    //std::cerr << "Writing XML output..." << std::endl;
//    //xmlWriter->writeToFile(filename);
//    //std::cerr << "Finished." << std::endl;
//}

//bool Parser::initTagger(std::string filename)
//{
//    std::cerr << "Loading inflected forms dictionary..." << std::endl;
//    std::string dict_filename = filename;
//    tagger->loadDictionary(filename);
//    std::cerr << "Dictionary loaded." << std::endl;
//    return true;
//}

//bool Parser::initNewTagger(std::string filename)
//{
//    if (verbose)
//        std::cerr << "Loading inflected forms dictionary..." << std::endl;
//    std::string dict_filename = filename;
//    newtagger->loadDictionary(filename);
//    if (verbose)
//        std::cerr << "Dictionary loaded." << std::endl;
//    return true;
//}

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

//void Parser::setXmlOutputFile(std::string filename)
//{
//    xmlOutputFilename = filename;
//}

void Puddle::setDotOutputFile(std::string filename)
{
    dotOutputFilename = filename;
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

//std::string Parser::save()
//{
//    std::stringstream ss;
//    boost::archive::text_oarchive ofs(ss);
//    //ofs << *(graph->getBoostGraph());
//    ofs << graph;
//    return ss.str();
//}
//
//void Parser::load(std::string serializedGraph)
//{
//    std::stringstream ss;
//    ss << serializedGraph;
//    boost::archive::text_iarchive ifs(ss);
//
//    ParseGraphPtr g;
//    ifs >> g;
//    graph = g;
//}
//
//ParseGraphPtr Parser::getGraph()
//{
//    return graph;
//}


//std::vector<ParseGraphPtr>* Parser::getGraphs()
//{
//    return xmlParser->getGraphs();
//}

/*
int main(int argc, char **argv)
{
    boost::program_options::options_description desc("Options");
    desc.add_options()
        ("file,f", boost::program_options::value<std::string>(), "input XML file")
        ("xml-out,o", boost::program_options::value<std::string>(), "output XML file")
        ("graph-out,g", boost::program_options::value<std::string>(), "output Dot file")
        ("tagset,t", boost::program_options::value<std::string>(), "tagset file")
        ("rules,r", boost::program_options::value<std::string>(), "rules file")
        ("dictionary,d", boost::program_options::value<std::string>(), "inflected forms dictionary")
//        ("syntok", "include syntactic tokens in parse graph")
        ("verbose", "talks a lot on stderr")
        ("tagset-log", boost::program_options::value<std::string>(), "tagset log file path")
        ("rules-log", boost::program_options::value<std::string>(), "rules log file path")
        ("help", "prints this information")
        ;
    boost::program_options::variables_map vm;
    boost::program_options::variables_map vm2;
    std::ifstream plik("config.ini");
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::store(boost::program_options::parse_config_file(plik, desc), vm2);
    boost::program_options::notify(vm);
    boost::program_options::notify(vm2);
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    Parser parser;

    //TODO: zrobic zeby ten parametr syntok faktycznie dzialam tam, gdzie ma dzialac, bo na razie sie robi dramat!
//    if (vm.count("syntok"))
        parser.setFlag("syntok");

    if (vm.count("verbose"))
        parser.setFlag("verbose");

    if (vm2.count("tagset-log"))
    {
        std::string logpath = vm2["tagset-log"].as<std::string>();
        parser.setTagsetLogFile(logpath);
    }

    if (vm2.count("rules-log"))
    {
        std::string logpath = vm2["rules-log"].as<std::string>();
        parser.setRulesLogFile(logpath);
    }

    if (vm2.count("tagset"))
    {
        std::string tagsetFilename = vm2["tagset"].as<std::string>();
        parser.loadTagset(tagsetFilename);
    }
    else
    {
        std::cerr << "Tagset file not specified" << std::endl;
        return 1;
    }

    if (vm2.count("rules"))
    {
        std::string rulesFilename = vm2["rules"].as<std::string>();
        parser.loadRules(rulesFilename);
    }
    else
    {
        std::cerr << "Rules file not specified" << std::endl;
        return 1;
    }
    
    std::string dotFilename = "output.dot";
    if (vm.count("graph-out"))
        dotFilename = vm["graph-out"].as<std::string>();

//    parser.loadTagset("tagset.pl.cfg");
    //parser.loadRules("simple.sr");
//    parser.loadRules("simple.sr");
    if (vm.count("file"))
    {
        std::string inputFilename = vm["file"].as<std::string>();
        if (vm.count("xml-out"))
        {
            std::string outputFilename = vm["xml-out"].as<std::string>();
            parser.setXmlOutputFile(outputFilename);
        }
        else
            parser.setXmlOutputFile("morphSh.xml");

        parser.parseFile(inputFilename);
        parser.writeGraph(dotFilename);
    }
    else
    {
        std::string dictionaryFilename = "slownik.pl.bin";
        if (vm2.count("dictionary"))
            dictionaryFilename = vm2["dictionary"].as<std::string>();
//        parser.initTagger(dictionaryFilename);
        parser.setStdoutOutput();
        parser.initNewTagger(dictionaryFilename);
        parser.parseStdin();
        parser.writeGraph(dotFilename);
    }

//    for (std::vector<ParseGraphPtr>::iterator pg = parser.getGraphs()->begin(); pg != parser.getGraphs()->end(); pg ++)
//    {
//        if ((*pg)->num_vertices() > 0)
//        {
//            std::cerr << (*pg)->shallow_path()->str() << std::endl;
//            std::cerr << (*pg)->deepest_path()->str() << std::endl;
//        }
////        else
////            std::cerr << "Ni ma wierzcholkow" << std::endl;
//    }
//    std::cerr << "Grafow mam w wyniku: " << parser.getGraphs()->size() << " sztuk." << std::endl;

//    parser.ruleParser

    return 0;
}*/

