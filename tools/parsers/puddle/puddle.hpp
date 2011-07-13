#ifndef PARSER_H__
#define PARSER_H__

//#include "TransitionGraph.hpp"
//#include "../common/ParseGraph.hpp"
//#include "XmlParser.hpp"
#include "tagset.hpp"
#include "rule_compiler.hpp"
#include "rule_matcher.hpp"
#include "rule.hpp"
#include "graph_writer.hpp"
//#include "TranslaticaGraph.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include "tagger.hpp"
//#include "me_tagger.hpp"
#include "parse_converter.hpp"
#include "rule_modifier.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Puddle
{
    public:
        Puddle();
        ~Puddle();
        virtual void setTagset(bonsai::puddle::TagsetPtr tagset_);
        //bool loadTagset(std::string filename);
        //bool loadTagset(std::string filename, std::string descFilename);
        virtual void setRules(bonsai::puddle::RulesPtr rules_);
        //void setTagger(bonsai::puddle::MeTaggerPtr tagger_);
        void setTagger(bonsai::puddle::TaggerPtr tagger_);
        //bool loadRules(std::string filename);
//        bool parseFile(std::string filename);
        void parseStdin();
        //bool parseTranslatica(zsyntree *syntree);
        bool writeGraph(std::string filename);
//        bool writeXmlOutput(std::string filename);
//        bool initTagger(std::string filename);
//        void setStdoutOutput() { this->xmlWriter->setStdoutOutput(); }
        void setFlag(std::string flag);
        bool getFlag(std::string flag);
        void setTagsetLogFile(std::string filename);
        void setRulesLogFile(std::string filename);
        void logTagset();
        void logRules();
//        void setXmlOutputFile(std::string filename);
        void setDotOutputFile(std::string filename);
        ParseGraphPtr parseString(std::string line);
        ParseGraphPtr parseTaggedString(std::string line);
//        std::vector<ParseGraphPtr>* getGraphs();
//        bool initNewTagger(std::string filename);
        void writeXml(std::string filename);

//        std::string save();
//        void load(std::string serializedGraph);
//        ParseGraphPtr getGraph();
        
    protected:
//        Tagset *tagset;
        TagsetPtr tagset;
        //RuleCompiler *ruleCompiler;
        RulesPtr rules;
        RuleMatcher *ruleMatcher;
//        XmlParser *xmlParser;
        GraphWriter *graphWriter;
        ParseConverter *parseConverter;
//        XmlWriter *xmlWriter;
//        Tagger *tagger;
//        Rules *rules;
        bool syntok;
        bool disamb;
        bool verbose;
        std::string tagsetLogFilename;
        std::string rulesLogFilename;
//        std::string xmlOutputFilename;
        std::string dotOutputFilename;

        //MeTaggerPtr tagger;
        TaggerPtr tagger;

        bool describe;
        bool norepeats;

        RuleModifier *ruleModifier;

//        ParseGraphPtr graph;
};

typedef boost::shared_ptr<Puddle> PuddlePtr;

}

}

}

#endif

