
#include "puddle.hpp"

namespace poleng {

namespace bonsai {

namespace puddle {

    Annotator* Puddle::Factory::doCreateAnnotator(
            const boost::program_options::variables_map& options) {

        TagsetLoader tagset_loader;
        RuleLoader rule_loader;
        //@todo: tu laduje reguly i tagset
        TagsetPtr tagset;

        std::string lang = options["lang"].as<std::string>();
        LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

        if (options.count("tagset")) {
            std::string tagsetFilename = options["tagset"].as<std::string>();
            boost::filesystem::path tagsetPath =
                fileFetcher.getOneFile(tagsetFilename);
            std::string tagsetPathString = tagsetPath.string();
            tagset = tagset_loader.load(tagsetPathString);
            if (tagset->size() == 0) {
                throw PuddleNoTagsetException("No tagset loaded");
            }
            rule_loader.setTagset(tagset);
        } else {
            throw PuddleNoTagsetException("No tagset loaded");
        }

        RulesPtr rules;
        if (options.count("rules")) {
            std::string rulesFilename = options["rules"].as<std::string>();
            boost::filesystem::path rulesPath =
                fileFetcher.getOneFile(rulesFilename);
            std::string rulesPathString = rulesPath.string();
            rules = rule_loader.readFromFile(rulesPathString);
            if (rules->size() == 0) {
                throw PuddleNoRulesException("No rules loaded");
            }
        } else {
            throw PuddleNoRulesException("No rules loaded");
        }
        Puddle *puddle = new Puddle(tagset, rules);

        return puddle;
    }

    void Puddle::Factory::doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) {

        optionsDescription.add_options()
            ("tagset", boost::program_options::value<std::string>()
             ->default_value(DEFAULT_TAGSET_FILE), "tagset file")
            ("rules", boost::program_options::value<std::string>()
             ->default_value(DEFAULT_RULE_FILE), "rules file")
            ;
    }

    std::string Puddle::Factory::doGetName() {
        return "puddle";
    }

    std::list<std::list<std::string> > Puddle::Factory::doRequiredLayerTags() {
        return std::list<std::list<std::string> >();
    }

    std::list<std::list<std::string> > Puddle::Factory::doOptionalLayerTags() {
        return std::list<std::list<std::string> >();
    }

    std::list<std::string> Puddle::Factory::doProvidedLayerTags() {
        std::list<std::string> layerTags;
        layerTags.push_back("parse");
        return layerTags;
    }

    const std::string Puddle::Factory::DEFAULT_TAGSET_FILE
        = "%ITSDATA%/%LANG%/tagset.%LANG%.cfg";
    const std::string Puddle::Factory::DEFAULT_RULE_FILE
        = "%ITSDATA%/%LANG%/rules.%LANG%";

    LatticeWorker* Puddle::doCreateLatticeWorker(Lattice& lattice) {
        return new Worker(*this, lattice);
    }

    Puddle::Worker::Worker(Puddle& processor, Lattice& lattice):
        LatticeWorker(lattice), processor_(processor) {
            //@todo: tu robi obiekt puddle
        }

    void Puddle::Worker::doRun() {
        processor_.parse(lattice_);
    }

    std::string Puddle::doInfo() {
        return "puddle shallow parser";
    }


Puddle::Puddle() {
    initProperties();
}

Puddle::Puddle(TagsetPtr tagset_, RulesPtr rules_) {
    initProperties();

    this->tagset = tagset_;
#if _WITH_BONSAI_PARSEGRAPH
    describe = tagset_->containsDesc();
#endif
    ruleMatcher->setTagset(tagset_);

    this->rules = rules_;
    ruleMatcher->setRules(rules_);
}

void Puddle::setTagset(bonsai::puddle::TagsetPtr tagset_) {
    if (tagset_->size() == 0) {
        throw PuddleNoTagsetException("No tagset loaded");
//        return;
    }
    this->tagset = tagset_;
#if _WITH_BONSAI_PARSEGRAPH
    describe = tagset_->containsDesc();
#endif

    ruleMatcher->setTagset(tagset_);
}

void Puddle::setRules(bonsai::puddle::RulesPtr rules_) {
    this->rules = rules_;
    ruleMatcher->setRules(rules_);
}

bool Puddle::parse(Lattice &lattice) {
    ruleMatcher->applyRules(lattice);
#if _WITH_BONSAI_PARSEGRAPH
    ParseGraphPtr outputGraph = lattice::convertToBonsaiGraph(lattice);
    std::cerr << outputGraph->write_graphviz() << std::endl;
#endif
    return true;
}

void Puddle::initProperties() {
    ruleMatcher = RuleMatcherPtr( new RuleMatcher() );
    ruleModifier = RuleModifierPtr( new RuleModifier() );

    syntok = true;
    verbose = false;
    norepeats = false;
    describe = false;
}


}

}

}

