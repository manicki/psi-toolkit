#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"

int main(int argc, char **argv) {
    boost::program_options::options_description desc("Options");
    desc.add_options()
        ("file,f", boost::program_options::value<std::string>(), "input XML file")
        ("tagset,t", boost::program_options::value<std::string>(), "tagset file")
        ("description", boost::program_options::value<std::string>(), "description file")
        ("rules,r", boost::program_options::value<std::string>(), "rules file")
        ("dictionary,d", boost::program_options::value<std::string>(), "inflected forms dictionary")
        ("model,m", boost::program_options::value<std::string>(), "Input model for tagging")
        ("verbose", "talks a lot on stderr")
        ("constituents", "constituents only")
        ("tagged", "pre-tagged input text")
        ("no-syntok", "do not include syntok in output")
        ("dot", "dot graph only")
        ("tagset-log", boost::program_options::value<std::string>(), "tagset log file path")
        ("rules-log", boost::program_options::value<std::string>(), "rules log file path")
        ("help", "prints this information")
        ;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;
    poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

    poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();

    if (vm.count("tagset")) {
        std::string tagsetFilename = vm["tagset"].as<std::string>();
        poleng::bonsai::puddle::TagsetPtr tagset;
        tagset = tagset_loader.load(tagsetFilename);
        if (tagset->size() == 0) {
            std::cerr << "Tagset not loaded." << std::endl;
            return 1;
        }
        tagger->setTagset(tagset);
        puddle->setTagset(tagset);
        //parser.logTagset();
        rule_loader.setTagset(tagset);
        //tagger->setTagset(tagset);
    } else {
        std::cerr << "Tagset not loaded." << std::endl;
        return 1;
    }

    if (vm.count("rules")) {
        std::string rulesFilename = vm["rules"].as<std::string>();
        poleng::bonsai::puddle::RulesPtr rules =
            rule_loader.readFromFile(rulesFilename);//, puddle->getLatticeWrapper());
        if (rules->size() == 0) {
            std::cerr << "Rules not loaded. " << std::endl;
            return 1;
        }
        for (poleng::bonsai::puddle::Rules::iterator ir = rules->begin(); ir != rules->end(); ++ ir)
        {
            std::cerr << (*ir)->log();
            std::cerr << std::endl;
        }
        puddle->setRules(rules);
        //parser.logRules();
    } else {
        std::cerr << "Rules not loaded." << std::endl;
        return 1;
    }
    puddle->setTagger(tagger);

    std::string line;
    while (getline(std::cin, line)) {
        //@todo: robic krate z line i do puddle'a
        //poleng::bonsai::ParseGraphPtr pg = puddle->parseTaggedString(line);
        //std::cerr << pg->write_graphviz();
    }
//delete puddle;

    return 0;
}
