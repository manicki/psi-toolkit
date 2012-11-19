#ifndef RULEGRAPH_H__
#define RULEGRAPH_H__

#include <vector>
#include <map>
#include <string>
#include "ParseGraph.hpp"
#include <boost/dynamic_bitset.hpp>

namespace poleng
{

namespace bonsai
{

typedef std::pair<int, int> Coord;

typedef std::pair<std::pair<int, int>, std::string> RuleElem;
struct elemcomp {
    bool operator() (RuleElem a, RuleElem b)
    {
        return ((a.first.first < b.first.first) || (a.first.second < b.first.second));
    }
};
typedef std::set<RuleElem, elemcomp> RuleElems;

typedef std::pair<Coord, Coord> RuleRect;

class RuleGraph
{
    public:
        RuleGraph();
        RuleGraph(bool combined);
        ~RuleGraph();
        std::string getRoot();
        int getCompositionLevel();
        ParseGraphPtr getGraph();
        void setRoot(std::string aRoot);
        void setCompositionLevel(int aLevel);
        void setGraph(ParseGraphPtr aGraph, std::map<ParseGraph::Edge, int> roots_indexes, std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects, std::map<int, std::vector<int> > words_aligned);
        void setGraph(ParseGraphPtr aGraph, std::map<ParseGraph::Edge, int> roots_indexes, std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects);
        int freeLeavesCount();

//        std::multimap<std::string, ParseGraph::Edge> free_leaves;

        //void attach(ParseGraph::Edge edge, ParseGraphPtr aGraph);
        void attach(int index, ParseGraph::Edge edge, RuleGraph rg);

        void copy(RuleGraph rg);
        bool fits(ParseGraph::Edge edge);
        int root_start, root_end, root_depth;
        int root_sig;

        int trans_low, trans_high;

//        std::string getRule(std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects, std::map<int, std::vector<int> > words_aligned, bool include_k);
        std::string getRule(bool include_rects, bool include_k);

        int getLeaves();
        void setLeaves(int aLeaves);
        int getDepth();
        void setDepth(int aDepth);

        RuleRect root_rect;
        std::map<int, RuleRect> non_rects;
        
        std::map<ParseGraph::Edge, int> depths;

        std::vector<ParseGraph::Edge> leaves_list;
        bool differentLeaves(RuleGraph rg);

        bool operator== (RuleGraph rg)
        {
//            std::cerr << "Porownywadlo sie wlaczylo sie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111222222222222" << std::endl;
            if (this->freeLeavesCount() != rg.freeLeavesCount())
                return false;
            if (this->getRoot() != rg.getRoot())
                return false;
            if (this->getLeaves() != rg.getLeaves())
                return false;
//            if (this->getDepth() != rg.getDepth()) //TODO: czy to jest niezbedne?
//                return false;
            bool diffLeaves = this->differentLeaves(rg);
            //TODO: sprawdzenie, czy wszystkie liscie te same

            if (diffLeaves)
                return false;
            //end of TODO

            return true;
//            return ((this->root_start == rg.root_start) && (this->root_end == rg.root_end) && (this->root_depth == rg.root_depth));
        }

//        bool operator< (RuleGraph &rg)
//        {
//        }

        int getSize();

        void setH(int h);
        void setSize(int aSize);

        boost::dynamic_bitset<> set_open;
        boost::dynamic_bitset<> set_history;


        //std::vector<std::string> rule_contents;
        RuleElems rule_contents;
        //std::vector<std::string> rule_translations;
        RuleElems rule_translations;
        //std::map<std::pair<int, int>, std::string> rule_translations;
        //std::vector<std::pair<int, int> > rule_alignments; //TODO: nie lepiej mape od razu?
        std::map<int, std::vector<int> > rule_alignments;
        boost::dynamic_bitset<> non_terminals_src;
        boost::dynamic_bitset<> non_terminals_trg;
        //std::vector<int> non_terminals_trg_mapped;
        std::map<int, int> non_terminals_trg_mapped;
        int rect_start, rect_end;


        bool combined_symbols;

        std::map<int, std::string> trg_toks;

    private:
        int composition_level;
        std::string root_label;
        int leaves;
        int depth;
        ParseGraphPtr graph;

        int size;

        bool include_alignments;
};

//struct rule_graph_compare
//{
//    bool operator() (RuleGraph rg1, RuleGraph rg2)
//    {
////        if (rg1.getCompositionLevel() < rg2.getCompositionLevel())
////            return true;
//        if (rg1.freeLeavesCount() < rg2.freeLeavesCount())
//            return true;
//        if (rg1.getRoot() < rg2.getRoot())
//            return true;
//        if (rg1.getLeaves() < rg2.getLeaves())
//            return true;
//        bool dyf = rg1.differentLeaves(rg2);
//        if (dyf)
//            std::cerr << "Dyfrent zwrocil true" << std::endl;
//        else
//            std::cerr << "Dyfrent zwrocil false" << std::endl;
//        return (dyf);
//        //return ((rg1.getCompositionLevel() < rg2.getCompositionLevel()) || (rg1.getRoot() < rg2.getRoot()) || (rg1.freeLeavesCount() < rg2.freeLeavesCount()) || (rg1.root_start < rg2.root_start) || (rg1.root_depth < rg2.root_depth) || (rg1.root_end < rg2.root_end));
//    }
//};

}

}

#endif

