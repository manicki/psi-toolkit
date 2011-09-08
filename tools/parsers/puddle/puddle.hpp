#ifndef PARSER_H__
#define PARSER_H__

#include "config.h"

//#include "TransitionGraph.hpp"
//#include "../common/ParseGraph.hpp"
//#include "XmlParser.hpp"
#include "tagset.hpp"
#include "rule_loader.hpp"
#include "rule_matcher.hpp"
#include "rule.hpp"
//#include "graph_writer.hpp"
//#include "TranslaticaGraph.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include "tagger.hpp"
//#include "me_tagger.hpp"
//#include "parse_converter.hpp"
#include "rule_modifier.hpp"
#include "lattice_wrapper.hpp"
#include "tagset_loader.hpp"

#include "annotator.hpp"
#include "annotator_factory.hpp"

#include "lang_specific_processor_file_fetcher.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Puddle : public Annotator {
    public:
        class Factory : public AnnotatorFactory {
            virtual Annotator* doCreateAnnotator(
                    const boost::program_options::variables_map& options);

            virtual boost::program_options::options_description doOptionsHandled();

            virtual std::string doGetName();

            virtual std::list<std::list<std::string> > doRequiredLayerTags();

            virtual std::list<std::list<std::string> > doOptionalLayerTags();

            virtual std::list<std::string> doProvidedLayerTags();

            static const std::string DEFAULT_TAGSET_FILE;
            static const std::string DEFAULT_RULE_FILE;
        };

        Puddle();
        Puddle(TagsetPtr tagset_, RulesPtr rules_);//,
                //const boost::program_options::variables_map& options);
        virtual ~Puddle();
        virtual void setTagset(bonsai::puddle::TagsetPtr tagset_); //@todo: po kiego te funkcje sa wirtualne?
        //bool loadTagset(std::string filename);
        //bool loadTagset(std::string filename, std::string descFilename);
        virtual void setRules(bonsai::puddle::RulesPtr rules_);
        //void setTagger(bonsai::puddle::MeTaggerPtr tagger_);
        void setTagger(bonsai::puddle::TaggerPtr tagger_);
        //bool loadRules(std::string filename);
//        bool parseFile(std::string filename);
//        void parseStdin(); //@todo: parseStdin tez moze byc, ale chyba w innej postaci (nie klasa puddle?)
        //bool parseTranslatica(zsyntree *syntree);
//        bool writeGraph(std::string filename);
//        bool writeXmlOutput(std::string filename);
//        bool initTagger(std::string filename);
//        void setStdoutOutput() { this->xmlWriter->setStdoutOutput(); }
        void setFlag(std::string flag);
        bool getFlag(std::string flag) const;
        //@todo: parseString, parseTaggedString mogą wrócić, tylko kwestia taggera/lematyzatora pozostaje. sama lattice zen da sie zrobic.
        //@todo: czy parse ma zwracac nowa lattice czy dodawac do wejscioewj to jest juz kwestia do przemyslenia
//        ParseGraphPtr parseString(std::string line);
//        ParseGraphPtr parseTaggedString(std::string line);
//        ParseGraphPtr parse(ParseGraphPtr graph); // void parse(ParseGraphPtr &graph);
        bool parse(Lattice &lattice);
//        std::vector<ParseGraphPtr>* getGraphs();
//        LatticeWrapperPtr getLatticeWrapper();

    protected:
//        Tagset *tagset;
        TagsetPtr tagset;
        //RuleCompiler *ruleCompiler;
        RulesPtr rules;
        RuleMatcher *ruleMatcher;
//        XmlParser *xmlParser;
//        GraphWriter *graphWriter;
//        ParseConverter *parseConverter;
//        XmlWriter *xmlWriter;
//        Tagger *tagger;
//        Rules *rules;
        bool syntok;
        bool disamb;
        bool verbose;
//        std::string xmlOutputFilename;
        std::string dotOutputFilename;

        //MeTaggerPtr tagger;
        TaggerPtr tagger;

        bool describe;
        bool norepeats;

        RuleModifier *ruleModifier;
//        LatticeWrapperPtr latticeWrapper;

//        ParseGraphPtr graph;
    private:
        class Worker : public LatticeWorker {
            public:
                Worker(Puddle& processor, Lattice& lattice);
            private:
                virtual void doRun();
                Puddle& processor_;
        };

        virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

        virtual std::string doInfo();

};

typedef boost::shared_ptr<Puddle> PuddlePtr;

}

}

}

#endif

