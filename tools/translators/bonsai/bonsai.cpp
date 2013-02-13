#include "bonsai.hpp"
#include "lattice_wrapper.hpp"

Annotator* Bonsai::Factory::doCreateAnnotator(
        const boost::program_options::variables_map& options) {
    
    std::string lang = options["lang"].as<std::string>();
    
    boost::program_options::variables_map cfg;
    if(options.count("config")) {
	std::ifstream plik(options["config"].as<std::string>().c_str());
	boost::program_options::store(boost::program_options::parse_config_file(plik, optionsHandled()), cfg);
	boost::program_options::notify(cfg);
    }
                			
    if (options.count("help")) {
        std::cerr << optionsHandled() << std::endl;
    }
	
    std::vector<std::string> rs_files;
    std::vector<std::string> lm_files;
    			
    if (options.count("rs")) {
        rs_files = options["rs"].as<std::vector<std::string> >();
    }
    else {
	if (cfg.count("rs")) {
            rs_files = cfg["rs"].as<std::vector<std::string> >();
	}
	else {
	    std::cerr << "Error. No translation rules sets given. Terminating." << std::endl;
	    //exit(1);
	    // Co zamiast exitow? Wyjatki
	}
    }
    
    if (options.count("lm")) {
        lm_files = options["lm"].as<std::vector<std::string> >();
    }
    else {
	if (cfg.count("lm")) {
	    lm_files = cfg["lm"].as<std::vector<std::string> >();
	}
	else {
	    std::cerr << "Error. No language models given. Terminating" << std::endl;
	    //exit(1);
	}
    }
    
    poleng::bonsai::Floats tm_weights;
    double word_penalty_weight = 0;
    poleng::bonsai::Floats rs_weights;
    poleng::bonsai::Floats lm_weights;
    
    if (options.count("tm_weight")) {
	tm_weights = options["tm_weight"].as<poleng::bonsai::Floats>();
    } 
    else if (cfg.count("tm_weight")) {
	tm_weights = cfg["tm_weight"].as<poleng::bonsai::Floats>();
    } 
    else {
	std::cerr << "Warning. No translation model weights given. Parameters will be weighted equally." << std::endl;
    }
    
    if (options.count("rs_weight")) {
	rs_weights = options["rs_weight"].as<poleng::bonsai::Floats>();
    } 
    else if (cfg.count("rs_weight")) {
	rs_weights = cfg["rs_weight"].as<poleng::bonsai::Floats>();
    } 
    else {
	std::cerr << "Warning. No rule set weights given. Rule sets will be weighted equally." << std::endl;
    }
    
    if (options.count("lm_weight")) {
	lm_weights = options["lm_weight"].as<poleng::bonsai::Floats>();
    } 
    else if (cfg.count("lm_weight")) {
	lm_weights = cfg["lm_weight"].as<poleng::bonsai::Floats>();
    } 
    else {
	std::cerr << "Warning. No language model weights given. Language models will be weighted equally." << std::endl;
    }
    
    if (options.count("word_penalty")) {
	word_penalty_weight = options["word_penalty"].as<double>();
    } 
    else if (cfg.count("word_penalty")) {
	word_penalty_weight = cfg["word_penalty"].as<double>();
    } 
    else {
	std::cerr << "Warning. No word penalty given. Using 0 as word penalty weight." << std::endl;
	word_penalty_weight = 0;
    }
    
    if(rs_weights.size() != rs_files.size()) {
	std::cerr << "Warning. Number of rule sets and rule set weights differs. Rule sets will be weighted equally." << std::endl;
	rs_weights.clear();
	rs_weights.resize(rs_files.size(), 1/rs_files.size());
    }
    
    if(lm_weights.size() != lm_files.size()) {
	std::cerr << "Warning. Number of language models and language model weights differs. Language models will be weighted equally." << std::endl;
	lm_weights.clear();
	lm_weights.resize(lm_files.size(), 1/lm_files.size());
    }
    
    poleng::bonsai::Translator::set_tm_weights(tm_weights);
    poleng::bonsai::Translator::set_rs_weights(rs_weights);
    poleng::bonsai::Translator::set_lm_weights(lm_weights);
    poleng::bonsai::Translator::set_word_penalty_weight(word_penalty_weight);
    
    poleng::bonsai::LmContainerPtr lmc ( new poleng::bonsai::LmContainer() );
    for(size_t i = 0; i < lm_files.size(); i++) {
        poleng::bonsai::LanguageModelPtr lm( new poleng::bonsai::LanguageModel(lm_files[i]) );
	if(i == 0) lm->set_lc(true);
	
        lmc->add_lm(lm);
    }
    
    poleng::bonsai::RuleLoaderPtr rl( new poleng::bonsai::RuleLoader(7, 4, lmc) );
    
    rl->set_verbosity(options["verbose"].as<int>());
    rl->set_max_transformations_per_hyperedge(options["max_trans"].as<int>());    
    rl->set_max_hyperedges_per_nonterminal(options["max_hyper"].as<int>());    
    rl->set_max_transformation_factor(options["eps"].as<double>());
    
    for(size_t i = 0; i < rs_files.size(); i++) {
	poleng::bonsai::RuleSetPtr rs( new poleng::bonsai::RuleSet( rs_files[i], 7, 4, i, lmc ) );
	rl->add_rule_set( rs );
    }
       
    poleng::bonsai::TranslatorPtr translator(
	new poleng::bonsai::Translator(
	    rl,
	    lmc,
	    options["nbest"].as<int>(),
	    options["stacksize"].as<int>()
	)
    );
    
    translator->set_verbosity(options["verbose"].as<int>());
    translator->set_pedantry(options.count("pedantic") > 0);
    translator->set_mert(options.count("mert") > 0);
    translator->set_nbest(options["nbest"].as<int>());    
    
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    Bonsai* bonsai = new Bonsai(translator, lang);
    return bonsai;
}

void Bonsai::Factory::doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) {
    
    namespace po = boost::program_options;
    
    optionsDescription.add_options()
        ("help", "Display this message")
	("config", po::value<std::string>(), "Path to configuration")
	("rs", po::value<std::vector<std::string> >(), "Paths to translation rule sets")
	("lm", po::value<std::vector<std::string> >(), "Paths to language models")
	
	("stacksize", po::value<int>()->default_value(20), "Node translation stack size")
	("max_trans", po::value<int>()->default_value(20), "Maximal number of transformations per hyper edge")
	("max_hyper", po::value<int>()->default_value(20), "Maximal number of hyper edges per symbol")
	("eps", po::value<double>()->default_value(-1), "Allowed transformation cost factor")
	("nbest", po::value<int>()->default_value(1), "Display n best translations")
	
	("verbose", po::value<int>()->default_value(0), "Level of verbosity: 0, 1, 2")
	("pedantic", "Pedantic cost calculation (for debugging)")
	("mert", "Output for MERT (combine with nbest)")
	
	("tm_weight", po::value<poleng::bonsai::Floats>(), "Weights for translation model parameters")
        ("rs_weight", po::value<poleng::bonsai::Floats>(), "Weights for different translation rules sets")
        ("lm_weight", po::value<poleng::bonsai::Floats>(), "Weights for different language models")
        ("word_penalty", po::value<double>(), "Weight for word penalty")
    ;
    
}

std::string Bonsai::Factory::doGetName() const {
    return "bonsai";
}

boost::filesystem::path Bonsai::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Bonsai::Factory::doRequiredLayerTags() {
        return boost::assign::list_of(
            boost::assign::list_of(std::string("parse")));
}

std::list<std::list<std::string> > Bonsai::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Bonsai::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("trans");
    return layerTags;
}

LatticeWorker* Bonsai::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Bonsai::Worker::Worker(Bonsai& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void Bonsai::Worker::doRun() {
    processor_.translate(lattice_);
}

std::string Bonsai::doInfo() {
    return "Bonsai translator";
}

Bonsai::Bonsai(poleng::bonsai::TranslatorPtr translator, const std::string& langCode) :
    translator_(translator), langCode_(langCode)
{ }

void Bonsai::translate(Lattice &lattice) {
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "segment", langCode_);
    Lattice::EdgesSortedBySourceIterator segmentIt =
        lattice.edgesSortedBySource(segmentMask);
	
    if (!segmentIt.hasNext())
    {
        Lattice::VertexDescriptor start = lattice.getFirstVertex();
        Lattice::VertexDescriptor end   = lattice.getLastVertex();
	
        translator_->translate(lattice, start, end, langCode_);
    }
    while (segmentIt.hasNext())
    {
        Lattice::EdgeDescriptor segment = segmentIt.next();
        Lattice::VertexDescriptor start = lattice.getEdgeSource(segment);
        Lattice::VertexDescriptor end   = lattice.getEdgeTarget(segment);
        
	translator_->translate(lattice, start, end, langCode_);
   }
}

