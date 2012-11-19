#ifndef RULEGENERATOR_H__
#define RULEGENERATOR_H__

#include <string>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include "RuleGraph.hpp"
#include "RuleStruct.hpp"
#include "ParseGraph.hpp"
#include <boost/program_options.hpp>
#include "Parser.hpp"
#include <cstdio>
#include <cstdlib>
#include "RectGenerator.hpp"


//typedef std::pair<Coord, Coord> RectCoord;
//typedef std::pair<int, int> TreeRange;

namespace poleng
{

namespace bonsai
{

typedef std::vector<std::pair<ParseGraph, int> > ParseGraphs;

class RuleGenerator {

    private:
//        bool *alignment;
        ParseGraphPtr graph;
        int s, t;
        std::vector<RectCoord> rects;
        std::vector<std::string> srcTokens;
        std::vector<std::string> trgTokens;
        //std::vector<std::pair<ParseGraph, int> > min_graphs;
        std::map<int, RuleGraph> min_graphs;
        std::multimap<int, int> min_graphs_by_root;
        std::vector<TransitionInfo> frontier_set;
        int graph_vertices;
//        std::vector<std::string> translations;
        std::vector<int> rect_refs;

//        std::vector<std::pair<int, std::string> > unaligned;

        int limit_symbols;
        int limit_non_terminals;
        int limit_depth;
        int limit_compositions;

        std::vector<std::multimap<std::string, RuleGraph> > graphs_by_root;

        std::vector<std::vector<RuleGraph> > open_graphs;
        std::vector<std::vector<RuleGraph> > closed_graphs;

        std::map<Coord, Coord> align_rects;
        std::map<int, std::vector<int> > words_aligned;

        bool include_k;
        bool include_rects;
        bool combinedSymbols;
        bool noLimits;
        bool variants;

        bool tagged;

        bool diag;
        std::string diag_filename;
        //char diag_filename[L_tmpnam];
//        std::set<RuleGraph, rule_graph_compare> all_graphs;

        puddle::Parser *parser_src;
        puddle::Parser *parser_trg;
//        ParseConverter *conv;

        std::map<int, ParseGraph::Edge> roots_edges;
        std::map<boost::dynamic_bitset<>, RuleGraph> graphs_by_signature;

        RectGenerator rectGenerator;

        bool include_alignments;
        bool include_alignments_;
        bool use_word_alignments;

    public:
        RuleGenerator();
        ~RuleGenerator();
        //void createAlign(ParseGraphPtr pg, std::string &trgSentence, std::vector<RectCoord> rects);
        void createAlign(ParseGraphPtr pg, std::string &trgSentence);
        void setRects(std::vector<RectCoord> aRects);
        void generateFrontierSet();
        std::vector<std::string> generateRules(ParseGraphPtr pg, std::vector<RectCoord> rects, std::string &trgSentence, bool include_alignments_);
        std::vector<std::string> generateRules(ParseGraphPtr pg, std::vector<RectCoord> rects, std::string &trgSentence, std::string &alignments);
        std::vector<std::string> combineGraphs(); 
        void addGraph(ParseGraphPtr pg);
        void setWordAlignments(std::string alignments);
        
        //void combineGraphs(int degree);
        void combineGraphs(int degree, long &timecopy, long &timeattach);

        //std::vector<RuleStruct> extractFromFile(std::string filename);
        void extractFromFile(std::string filename, std::string output_filename);
        void extractFromStdin();

        void setParserSrc(puddle::Parser *aParser);
        void setParserTrg(puddle::Parser *aParser);
//        void setConverter(ParseConverter *aConverter);

        std::string normalizeSentence(ParseGraphPtr graph);

        int getLimitCompositions();
        void setLimitCompositions(int aLimit);
        int getLimitDepth();
        void setLimitDepth(int aLimit);
        int getLimitNonTerminals();
        void setLimitNonTerminals(int aLimit);
        int getLimitSymbols();
        void setLimitSymbols(int aLimit);

        void includeCompositionLevel();
        void includeRects();
        void setCombinedSymbols();
        void setNoLimits();
        void setVariants();

        void setDiag(std::string id);

        void setTagged();
        void setIncludeAlignments();
        void setUseWordAlignments();
};

}

}

#endif

