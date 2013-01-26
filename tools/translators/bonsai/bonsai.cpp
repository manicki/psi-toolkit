#include "bonsai.hpp"

Annotator* Bonsai::Factory::doCreateAnnotator(
        const boost::program_options::variables_map& options) {
    
    std::string lang = options["lang"].as<std::string>();
    
    boost::program_options::variables_map cfg;
    std::ifstream plik(options["config"].as<std::string>().c_str());
    boost::program_options::store(boost::program_options::parse_config_file(plik, optionsHandled()), cfg);
    boost::program_options::notify(cfg);

                			
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
	    exit(1);
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
	    exit(1);
	}
    }
    
    poleng::bonsai::Floats tm_weights;
    double word_penalty_weight = 0;
    poleng::bonsai::Floats rs_weights;
    poleng::bonsai::Floats lm_weights;
    
    if (options.count("tm_weight")) {
	tm_weights = options["tm_weight"].as<Floats>();
    } 
    else if (cfg.count("tm_weight")) {
	tm_weights = cfg["tm_weight"].as<Floats>();
    } 
    else {
	std::cerr << "Warning. No translation model weights given. Parameters will be weighted equally." << std::endl;
    }
    
    if (options.count("rs_weight")) {
	rs_weights = options["rs_weight"].as<Floats>();
    } 
    else if (cfg.count("rs_weight")) {
	rs_weights = cfg["rs_weight"].as<Floats>();
    } 
    else {
	std::cerr << "Warning. No rule set weights given. Rule sets will be weighted equally." << std::endl;
    }
    
    if (options.count("lm_weight")) {
	lm_weights = options["lm_weight"].as<Floats>();
    } 
    else if (cfg.count("lm_weight")) {
	lm_weights = cfg["lm_weight"].as<Floats>();
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
    
    std::cerr << std::endl << "Initializing translator" << std::endl;
    
    Translator::set_tm_weights(tm_weights);
    Translator::set_rs_weights(rs_weights);
    Translator::set_lm_weights(lm_weights);
    Translator::set_word_penalty_weight(word_penalty_weight);
    
    poleng::bonsai::LmContainerPtr lmc ( new LmContainer() );
    for(int i = 0; i < lm_files.size(); i++) {
        poleng::bonsai::LanguageModelPtr lm( new LanguageModel(lm_files[i]) );
	if(i == 0) lm->set_lc(true);
	
        lmc->add_lm(lm);
    }

    std::cerr << "test1" << std::endl;
    
    poleng::bonsai::RuleLoaderPtr rl( new RuleLoader(7, 4, lmc) );
    for(int i = 0; i < rs_files.size(); i++) {
	std::cerr << rs_files[i] << std::endl;
	RuleSetPtr rs( new RuleSet( rs_files[i], 7, 4, i, lmc ) );
        rl->add_rule_set( rs );
    }
    rl->set_max_transformations_per_hyperedge(options["max_trans"].as<int>());    
    rl->set_max_hyperedges_per_nonterminal(options["max_hyper"].as<int>());    
    rl->set_max_transformation_factor(options["eps"].as<double>());
    
    rl->set_verbosity(options["verbose"].as<int>()-1);
    
        
    poleng::bonsai::TranslatorPtr translator(
	new poleng::bonsai::Translator(
	    rl,
	    lmc,
	    options["stacksize"].as<int>()
	)
    );
    
    translator->set_verbosity(options["verbose"].as<int>());
    translator->set_pedantry(options.count("pedantic") > 0);
    translator->set_mert(options.count("mert") > 0);
    translator->set_nbest(options["nbest"].as<int>());
    
    std::cerr << "Finished initialization." << std::endl << std::endl;        
    
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    Bonsai* bonsai = new Bonsai(translator, lang);
    return bonsai;
}

void Bonsai::Factory::doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) {
    
    namespace po = boost::program_options;
    
    optionsDescription.add_options()
        ("help", "Display this message")
	("config", po::value<std::string>()->default_value("bonsai.cfg"), "Path to configuration")
	("rs", po::value<std::vector<std::string> >(), "Paths to translation rule sets")
	("lm", po::value<std::vector<std::string> >(), "Paths to language models")
	
	("stacksize", po::value<int>()->default_value(20), "Node translation stack size")
	("max_trans", po::value<int>()->default_value(20), "Maximal number of transformations per hyper edge")
	("max_hyper", po::value<int>()->default_value(20), "Maximal number of hyper edges per symbol")
	("eps", po::value<double>()->default_value(-1), "Allowed transformation cost factor")
	("nbest", po::value<int>()->default_value(1), "Display n best translations")
	
	("verbose,v", po::value<int>()->default_value(0), "Level of verbosity: 0, 1, 2")
	("pedantic,p", "Pedantic cost calculation (for debugging)")
	("mert", "Output for MERT (combine with nbest)")
	
	("tm_weight,t", po::value<Floats>(), "Weights for translation model parameters")
        ("rs_weight,r", po::value<Floats>(), "Weights for different translation rules sets")
        ("lm_weight,l", po::value<Floats>(), "Weights for different language models")
        ("word_penalty,w", po::value<double>(), "Weight for word penalty")
     
        ("graph-out,g", boost::program_options::value<std::string>(), "output Dot file")
    ;
    
}

std::string Bonsai::Factory::doGetName() const {
    return "bonsai";
}

boost::filesystem::path Bonsai::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Bonsai::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Bonsai::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Bonsai::Factory::doProvidedLayerTags() {
    return std::list<std::string>();
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
    if (!segmentIt.hasNext()) {
        Lattice::VertexDescriptor start = lattice.getFirstVertex();
        Lattice::VertexDescriptor end   = lattice.getLastVertex();
        
        poleng::bonsai::ParseGraphPtr pg(new poleng::bonsai::ParseGraph());
        std::string line = "22 serialization::archive 9 0 0 10 25 0 0 0 1 0 0 0 0 5 token 2 un 1 1 0  0 1 0 0 0 5 0 0 0 2 un 3 adj 0 2 un 8 det:sg:m 1 2 un 4 pron 0 2 un 10 subst:sg:m 0 2 Un 9 name:sg:? 0 2 un 1 2 5 token 10 ordinateur 1 2 0  1 2 0 2 0 10 ordinateur 8 adj:sg:m 0 10 ordinateur 10 subst:sg:m 1 10 ordinateur 2 3 5 token 3 est 1 3 0  2 3 0 8 0 3 est 8 adj:pl:f 0 3 est 8 adj:sg:f 0 3 est 8 adj:pl:m 0 3 est 8 adj:sg:m 0 3 est 3 adv 0 3 est 10 subst:sg:m 0 5 être 19 verb:ind:fin:ter:sg 1 3 Est 9 name:sg:? 0 3 est 3 4 5 token 2 en 1 4 0  3 4 0 5 0 2 en 8 adj:sg:m 0 2 en 3 adv 0 2 en 4 prep 1 2 en 17 perspron:ter:sg:m 0 2 en 17 perspron:ter:sg:f 0 2 en 4 5 5 token 2 la 1 5 0  4 5 0 4 0 2 la 17 perspron:ter:sg:f 0 2 la 10 subst:pl:m 0 2 la 10 subst:sg:m 0 2 le 8 det:sg:f 1 2 la 5 6 5 token 6 maison 1 6 0  5 6 0 6 0 6 maison 8 adj:pl:f 0 6 maison 8 adj:sg:f 0 6 maison 8 adj:pl:m 0 6 maison 8 adj:sg:m 0 6 maison 10 subst:sg:f 1 6 Maison 9 name:sg:? 0 6 maison 6 7 5 token 2 de 1 7 0  6 7 0 4 0 2 de 8 det:pl:f 0 2 de 8 det:pl:m 0 2 de 4 prep 1 2 De 9 name:sg:? 0 2 de 7 8 5 token 2 l' 1 8 0  7 8 0 4 0 2 la 17 perspron:ter:sg:f 0 2 le 8 det:sg:f 0 2 le 8 det:sg:m 1 2 le 17 perspron:ter:sg:m 0 2 l' 8 9 5 token 5 homme 1 9 0  8 9 0 2 0 5 homme 10 subst:sg:m 1 5 Homme 9 name:sg:m 0 5 homme 2 3 5 group 2 VN 1 a 1 3 2 3 2 8 0 3 est 8 adj:pl:f 0 3 est 8 adj:sg:f 0 3 est 8 adj:pl:m 0 3 est 8 adj:sg:m 0 3 est 3 adv 0 3 est 10 subst:sg:m 0 5 être 19 verb:ind:fin:ter:sg 1 3 Est 9 name:sg:? 0 2 VN 0 2 5 group 2 NP 1 b 1 2 0 2 2 2 0 10 ordinateur 8 adj:sg:m 0 10 ordinateur 10 subst:sg:m 1 2 NP 7 9 5 group 2 NP 1 d 1 9 7 9 2 2 0 5 homme 10 subst:sg:m 1 5 Homme 9 name:sg:m 0 2 NP 6 9 5 group 2 PP 1 e 1 7 6 9 2 4 0 2 de 8 det:pl:f 0 2 de 8 det:pl:m 0 2 de 4 prep 1 2 De 9 name:sg:? 0 2 PP 4 9 5 group 2 NP 1 f 1 6 4 9 2 6 0 6 maison 8 adj:pl:f 0 6 maison 8 adj:sg:f 0 6 maison 8 adj:pl:m 0 6 maison 8 adj:sg:m 0 6 maison 10 subst:sg:f 1 6 Maison 9 name:sg:? 0 2 NP 3 9 5 group 2 PP 2 10 1 4 3 9 2 5 0 2 en 8 adj:sg:m 0 2 en 3 adv 0 2 en 4 prep 1 2 en 17 perspron:ter:sg:m 0 2 en 17 perspron:ter:sg:f 0 2 PP 0 9 5 group 4 Ssub 2 11 1 3 0 9 3 8 0 3 est 8 adj:pl:f 0 3 est 8 adj:sg:f 0 3 est 8 adj:pl:m 0 3 est 8 adj:sg:m 0 3 est 3 adv 0 3 est 10 subst:sg:m 0 5 être 19 verb:ind:fin:ter:sg 1 3 Est 9 name:sg:? 0 4 Ssub 0 1 3 pos 3 det 2 12 1 1 0 1 1 5 0 2 un 3 adj 0 2 un 8 det:sg:m 1 2 un 4 pron 0 2 un 10 subst:sg:m 0 2 Un 9 name:sg:? 0 3 det 1 2 3 pos 5 subst 2 13 1 2 1 2 1 2 0 10 ordinateur 8 adj:sg:m 0 10 ordinateur 10 subst:sg:m 1 5 subst 2 3 3 pos 4 verb 2 14 1 3 2 3 1 8 0 3 est 8 adj:pl:f 0 3 est 8 adj:sg:f 0 3 est 8 adj:pl:m 0 3 est 8 adj:sg:m 0 3 est 3 adv 0 3 est 10 subst:sg:m 0 5 être 19 verb:ind:fin:ter:sg 1 3 Est 9 name:sg:? 0 4 verb 3 4 3 pos 4 prep 2 15 1 4 3 4 1 5 0 2 en 8 adj:sg:m 0 2 en 3 adv 0 2 en 4 prep 1 2 en 17 perspron:ter:sg:m 0 2 en 17 perspron:ter:sg:f 0 4 prep 4 5 3 pos 3 det 2 16 1 5 4 5 1 4 0 2 la 17 perspron:ter:sg:f 0 2 la 10 subst:pl:m 0 2 la 10 subst:sg:m 0 2 le 8 det:sg:f 1 3 det 5 6 3 pos 5 subst 2 17 1 6 5 6 1 6 0 6 maison 8 adj:pl:f 0 6 maison 8 adj:sg:f 0 6 maison 8 adj:pl:m 0 6 maison 8 adj:sg:m 0 6 maison 10 subst:sg:f 1 6 Maison 9 name:sg:? 0 5 subst 6 7 3 pos 4 prep 2 18 1 7 6 7 1 4 0 2 de 8 det:pl:f 0 2 de 8 det:pl:m 0 2 de 4 prep 1 2 De 9 name:sg:? 0 4 prep 7 8 3 pos 3 det 2 19 1 8 7 8 1 4 0 2 la 17 perspron:ter:sg:f 0 2 le 8 det:sg:f 0 2 le 8 det:sg:m 1 2 le 17 perspron:ter:sg:m 0 3 det 8 9 3 pos 5 subst 2 1a 1 9 8 9 1 2 0 5 homme 10 subst:sg:m 1 5 Homme 9 name:sg:m 0 5 subst";
        
        pg->load(line);
        poleng::bonsai::TranslationQueuePtr tq = translator_->translate(pg);
        TranslationPtr best = *(tq->begin());
        
        AnnotationItem ai("TRANS", StringFrag(best->str()));
        LayerTagCollection tags
            = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
                "trans", langCode_);
        
        lattice.addEdge(start, end, ai, tags);
    }
    while (segmentIt.hasNext()) {

        Lattice::EdgeDescriptor segment = segmentIt.next();
        Lattice::VertexDescriptor start = lattice.getEdgeSource(segment);
        Lattice::VertexDescriptor end   = lattice.getEdgeTarget(segment);
        
        AnnotationItem ai("TRANS", StringFrag("lala lala lala"));
        LayerTagCollection tags
            = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
                "trans", langCode_);
        
        lattice.addEdge(start, end, ai, tags);
   }
}

