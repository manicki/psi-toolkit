
#include "RuleGenerator.hpp"

namespace poleng
{

namespace bonsai
{

RuleGenerator::RuleGenerator()
{
//    parser = new Parser;
//    alignment = new bool[1];

    include_k = false;
    include_rects = false;
    combinedSymbols = false;
    noLimits = false;
    variants = false;

    tagged = false;

    diag = false;

    include_alignments = false;
    include_alignments_ = false;
    use_word_alignments = false;
//    limit_symbols = 7;
//    limit_non_terminals = 4;
//    limit_depth = 2;
//    limit_compositions = 5;
}

RuleGenerator::~RuleGenerator()
{
//    delete alignment;
//    if (diag)
//    {
//        if (remove(diag_filename.c_str()) != 0)
//        {
//            std::cerr << "Error while deleting temporary file: " << diag_filename << "." << std::endl;
//        }
//    }
  //  delete parser;
}

//struct compare
//{
//    bool operator() (const TreeRange a, const TreeRange b)
//    {
//        return (a.second < b.second || (a.second == b.second && a.first < b.first));
//    }
//};
//
std::string RuleGenerator::normalizeSentence(ParseGraphPtr graph)
{
    std::string ret = "";

    ParseGraph::Graph *g = graph->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::VertexIndex index = get(boost::vertex_index, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    while (v != end)
    {
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
            ParseGraph::Edge e = *p.first;
            if (map[e].getDepth() == 0)
            {
                if (ret != "")
                    ret += " " + map[e].getLabel();
                else
                    ret += map[e].getLabel();
            }
        }
        v ++;
    }
    return ret;
}

std::vector<std::string> RuleGenerator::generateRules(ParseGraphPtr pg, std::vector<RectCoord> rects, std::string &trgSentence, bool include_alignments_)
{
    this->include_alignments = include_alignments_;
    addGraph(pg);
    setRects(rects);
    //createAlign(pg, trgSentence, rects);
    createAlign(pg, trgSentence);
    generateFrontierSet();
    std::vector<std::string> rules;
    rules = combineGraphs();
    return rules;
}

std::vector<std::string> RuleGenerator::generateRules(ParseGraphPtr pg, std::vector<RectCoord> rects, std::string &trgSentence, std::string &alignments)
{
    this->include_alignments = true;
    addGraph(pg);
    setRects(rects);
    //createAlign(pg, trgSentence, rects);
    createAlign(pg, trgSentence);
    setWordAlignments(alignments);
    generateFrontierSet();
    std::vector<std::string> rules;
    rules = combineGraphs();
    return rules;
}

//void RuleGenerator::createAlign(ParseGraphPtr pg, std::string &trgSentence, std::vector<RectCoord> rects)
void RuleGenerator::createAlign(ParseGraphPtr pg, std::string &trgSentence)
//void RuleGenerator::createAlign(std::string &srcSentence, std::string &trgSentence, std::string &groups)
{
    srcTokens.clear();
    trgTokens.clear();
    words_aligned.clear();

    SListPtr slist = pg->deepest_path();
    for (SList::iterator sit = slist->begin(); sit != slist->end(); sit ++)
    {
        srcTokens.push_back(sit->label());
    }

    if (include_alignments)
    {
    //for (std::vector<RectCoord>::iterator rit = rects.begin(); rit != rects.end(); rit ++)
    for (std::map<Coord, Coord>::iterator rit = align_rects.begin(); rit != align_rects.end(); rit ++)
    {
        //if (! rit->type)
        if ((rit->first.second - rit->first.first) == 1)
        {
            std::vector<int> aligned_indexes;
            int i = rit->second.first;
            while (i < rit->second.second)
            {
                aligned_indexes.push_back(i);
                i ++;
            }
            if (aligned_indexes.size() > 0)
            {
   //             std::map<int, std::vector<int> >::iterator wai = words_aligned.find(rit->first.first);
   //             if (wai == words_aligned.end())
                    words_aligned.insert(std::pair<int, std::vector<int> >(rit->first.first, aligned_indexes));
   //             else
   //             {
   //                 std::vector<int> aligned_new = wai->second;
   //                 aligned_new.insert(aligned_new.end(), aligned_indexes.begin(), aligned_indexes.end());
   //                 words_aligned[rit->first.first] = aligned_new;
   //             }
            }
        }
    }
    }

    boost::regex regSep("\\s+");
    boost::regex_token_iterator<std::string::iterator> w(trgSentence.begin(), trgSentence.end(), regSep, -1);
    boost::regex_token_iterator<std::string::iterator> v;
    while (w != v)
    {
        trgTokens.push_back(*w);
//        std::cerr << *w << std::endl;
        w ++;
    }


    s = srcTokens.size();
    t = trgTokens.size();
}

void RuleGenerator::addGraph(ParseGraphPtr pg)
{
    graph = pg;
}

void RuleGenerator::setRects(std::vector<RectCoord> aRects)
{
//    graph = pg;

    rects.clear();
    align_rects.clear();

    //rects.insert(rects.begin(), aRects.begin(), aRects.end());

    for (std::vector<RectCoord>::iterator ir = aRects.begin(); ir != aRects.end(); ir ++)
    {
        if (ir->type)
            rects.push_back(*ir);
    }

    for (int l = 0; l < limit_compositions; l++)
    {
        graphs_by_root[l].clear();
        open_graphs[l].clear();
        closed_graphs[l].clear();
    }
    rect_refs.clear();

    for (std::vector<RectCoord>::iterator ir = rects.begin(); ir != rects.end(); ir ++)
    {
        if (ir->type)
            align_rects.insert(std::pair<Coord, Coord>(std::pair<int, int>(ir->first.first, ir->second.first), std::pair<int, int>(ir->first.second, ir->second.second)));
    }

}

void RuleGenerator::setWordAlignments(std::string alignments)
{
    words_aligned.clear();
    if (alignments.length() > 2)
    {
        alignments = alignments.substr(1, alignments.length() - 2);
        boost::regex regPairSep("\\s*\\]\\s*,\\s*\\[\\s*");
        boost::regex regPair("\\s*(\\d+)\\s*,\\s*(\\d+)\\s*");
        boost::regex_token_iterator<std::string::iterator> item(alignments.begin(), alignments.end(), regPairSep, -1);
        boost::regex_token_iterator<std::string::iterator> end_it;

        while (item != end_it)
        {
            std::string pair = boost::lexical_cast<std::string>(*item);
            if (pair[0] == '[')
                pair = pair.substr(1, std::string::npos);
            if (pair[pair.length() - 1] == ']')
                pair = pair.substr(0, pair.length() - 1);
            boost::smatch container;
            if (boost::regex_match(pair, container, regPair))
            {
                int src = boost::lexical_cast<int>(container[1]) - 1;
                int trg = boost::lexical_cast<int>(container[2]) - 1;
                if (trg == -1)
                {
                    item ++;
                    continue;
                }
                std::map<int, std::vector<int> >::iterator it = words_aligned.find(src);
                if (it != words_aligned.end())
                {
                    std::vector<int> aligns = it->second;
                    aligns.push_back(trg);
                    words_aligned[src] = aligns;
                }
                else
                {
                    std::vector<int> aligns;
                    aligns.push_back(trg);
                    words_aligned.insert(std::pair<int, std::vector<int> >(src, aligns));
                }
            }
            item ++;
        }
    }
}

void RuleGenerator::generateFrontierSet()
{
    frontier_set.clear();
    min_graphs.clear();
    min_graphs_by_root.clear();
    roots_edges.clear();
    graphs_by_signature.clear();

    ParseGraph::Graph *g = graph->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::VertexIndex index = get(boost::vertex_index, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    std::map<ParseGraph::Edge, int> roots_indexes;
    int roots_idx = 0;
    std::vector<RectCoord>::iterator rect = rects.begin();
    int idx = 0;
    while (rect != rects.end())
    {
        int start = rect->first.first;
        int d = rect->second.first - rect->first.first;
//L        std::cerr << "Start: " << start << std::endl << "Dlugosc: "<< d << std::endl;
        v = vertex(start, *g);
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
            ParseGraph::Edge e = *p.first;
            if ((map[e].getEnd() == (start + d)) && (map[e].getDepth() >= 1))
            {
//                frontier_set.push_back(map[e].getLabel());
                frontier_set.push_back(map[e]);
                rect_refs.push_back(idx);
                roots_indexes.insert(std::pair<ParseGraph::Edge, int>(e, roots_idx));
                roots_edges.insert(std::pair<int, ParseGraph::Edge>(roots_idx, e));
//                std::cerr << "Wstawiam do roots_indexes; (" << map[e].getLabel() << "," << roots_idx << ")" << std::endl;
                roots_idx ++;
//                std::cerr << "dodaje do frontier setu: " << map[e].getLabel() << " od " << map[e].getStart() << " do " << map[e].getEnd() << std::endl;
            }
        }
        rect ++;
        idx ++;
    }

    int min_graph_count = 0;
    idx = 0;
    for (std::vector<TransitionInfo>::iterator fe = frontier_set.begin(); fe != frontier_set.end(); fe ++)
    {
        //TransitionInfo ti = *fe;
        int start = fe->getStart();
        int d = fe->getEnd() - fe->getStart();
        int i = 0;
//        std::cerr << "Graf dla: " << fe->getLabel() << " zawiera:" << std::endl;
        std::vector<ParseGraph> mgs;
        ParseGraph mg;
        std::vector<ParseGraph::Edge> common;
        std::vector<bool> terminals;
        while (i < d)
        {
            v = vertex(start + i, *g);
            bool first = true;
            int covered = 0;
            //bool was_fset = false; //TODO: czy w tym?
            std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> pp = out_edges(v, *g);
//X            if (pp.first == pp.second)
//                std::cerr << "Dla i = " << i << " nie ma krawedzi zadnych!" << std::endl;

            std::vector<ParseGraph::Edge> add_edges;
                bool already = false;
            bool was_fset = false; //TODO: to w tym miejscu? - przeniesione tu do wewnatrz petli 28.07, bo nie bylo "z" w "zgodnie z którym". chyba powinno zostać tu
            for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
            {
                //if (map[e].getDepth() < 2)
                //    continue;
                ParseGraph::Edge e = *p.first;
                if (map[e].getEnd() <= (start + d))
                {
                    bool in_fset = false;
                    for (std::vector<TransitionInfo>::iterator fsit = frontier_set.begin(); fsit != frontier_set.end(); fsit ++)
                    {
                        if (fsit->getLabel() == map[e].getLabel() && fsit->getStart() == map[e].getStart() && fsit->getEnd() == map[e].getEnd() && fsit->getDepth() == map[e].getDepth() && fsit->getType() == map[e].getType())
                        {
                            in_fset = true;
                            break;
                        }
                    }
                    //if (frontier_set.find(map[e]) != frontier_set.end())
                    if (in_fset)
                    {
                        bool orajt = false;
                        //if ((fe->getStart() == map[e].getStart()) && (fe->getEnd() == map[e].getEnd()))
                        if (fe->getStart() == map[e].getStart())
                        {
                            //if (!was_fset)
                            //if ((fe->getDepth() - map[e].getDepth()) == 1)
                            if (fe->getDepth() > map[e].getDepth())
                            {
                                if (!was_fset)
                                    orajt = true;
                            }
                        }
                        else
                        {
                            //if (first)
                            if (!was_fset)
                                orajt = true;
                        }
                        //if ((fe->getDepth() - map[e].getDepth()) == 1)
                        if (orajt)
                        {
                            add_edges.push_back(e);
                            was_fset = true;
                            already = true;

                            if (map[e].getDepth() > 0)
                                terminals.push_back(false);
                            else
                                terminals.push_back(true);

                            if ((map[e].getEnd() - map[e].getStart()) > 1)
                                covered = map[e].getEnd() - map[e].getStart() - 1;
                            else
                                covered = 0;
                        }
                    }
                    else
                    {
                        if (!was_fset)
                        {
                            bool done = false;
                            if (map[e].getDepth() > 0)
                            {
                            if (common.size() > 0)
                            {
                                while (map[common.back()].getStart() == map[e].getStart() && map[common.back()].getEnd() == map[e].getEnd() && map[common.back()].getDepth() == map[e].getDepth())
                                {
                                    ParseGraph::Edge com = common.back();
                                    add_edges.push_back(e);
                                    add_edges.push_back(com);
                                    common.pop_back();
                                    done = true;
                                    if (common.size() == 0)
                                        break;
                                }
                            }
                            }
                            if (!done)
                                common.push_back(e);

                            if (map[e].getDepth() > 0)
                                terminals.push_back(false);
                            else
                                terminals.push_back(true);

                            if ((map[e].getEnd() - map[e].getStart()) > 1)
                                covered = map[e].getEnd() - map[e].getStart() - 1;
                            else
                                covered = 0;
                        }
                    }
                    first = false;
                    //mg.add_edge(ti->getStart(), ti->getEnd(), *ti);
                }
            }
            if (add_edges.size() > 0)
            {
                if (mgs.size() != 0)
                {
                    std::vector<ParseGraph> new_mgs;
                    for (std::vector<ParseGraph>::iterator gi = mgs.begin(); gi != mgs.end(); gi ++)
                    {
                        for (std::vector<ParseGraph::Edge>::iterator ei = add_edges.begin(); ei != add_edges.end(); ei ++)
                        {
                            ParseGraph gra = *gi;
                            gra.add_edge(map[*ei].getStart(), map[*ei].getEnd(), map[*ei]);
                            new_mgs.push_back(gra);
                        }
                    }
                    mgs.clear();
                    mgs.insert(mgs.begin(), new_mgs.begin(), new_mgs.end());

                }
                else
                {
                    for (std::vector<ParseGraph::Edge>::iterator ei = add_edges.begin(); ei != add_edges.end(); ei ++)
                    {
                        ParseGraph mini;
                        mini.add_edge(map[*ei].getStart(), map[*ei].getEnd(), map[*ei]);
                        mgs.push_back(mini);
                    }
                }
            }
            i ++;
            i += covered;
        }

        bool add_root = true;
        if (mgs.size() == 0)
        {
            ParseGraph mini;
            mini.add_edge(fe->getStart(), fe->getEnd(), *fe);
            mgs.push_back(mini);
            add_root = false;
        }

        for (std::vector<ParseGraph>::iterator gi = mgs.begin(); gi != mgs.end(); gi ++)
        {
           ParseGraph newmg = *gi;
        if (add_root)
        newmg.add_edge(fe->getStart(), fe->getEnd(), *fe);

           for (std::vector<ParseGraph::Edge>::iterator itcom = common.begin(); itcom != common.end(); itcom ++)
           {
               newmg.add_edge(map[*itcom].getStart(), map[*itcom].getEnd(), map[*itcom]);
           }

	   RuleGraph rg(combinedSymbols);

        rg.setSize(frontier_set.size());
        if (include_alignments)
            rg.setGraph(ParseGraphPtr(new ParseGraph(newmg)), roots_indexes, trgTokens, align_rects, words_aligned);
        else
            rg.setGraph(ParseGraphPtr(new ParseGraph(newmg)), roots_indexes, trgTokens, align_rects);

        rg.setH(min_graph_count);
        if (rg.freeLeavesCount() > 0)
            open_graphs[0].push_back(rg);
        else
            closed_graphs[0].push_back(rg);
        //graphs_by_root[0].insert(std::pair<std::string, RuleGraph>(rg.getRoot(), rg));
//        all_graphs.insert(rg);

        min_graphs.insert(std::pair<int, RuleGraph>(min_graph_count, rg));;
        min_graphs_by_root.insert(std::pair<int, int>(idx, min_graph_count));
       min_graph_count ++;
        }
        idx ++;
    }
    
    //for (std::map<int, RuleGraph>::iterator mgi = min_graphs.begin(); mgi != min_graphs.end(); mgi ++)
    for (std::map<int, RuleGraph>::iterator mgi = min_graphs.begin(); mgi != min_graphs.end(); mgi ++)
    {
        mgi->second.set_history.resize(min_graph_count);
        mgi->second.set_open.resize(min_graph_count);
    }
    
    for (std::vector<RuleGraph>::iterator rgi = open_graphs[0].begin(); rgi != open_graphs[0].end(); rgi ++)
    {
        rgi->set_history.resize(min_graph_count);
        rgi->set_open.resize(min_graph_count);
    }

    for (std::vector<RuleGraph>::iterator rgi = closed_graphs[0].begin(); rgi != closed_graphs[0].end(); rgi ++)
    {
        rgi->set_history.resize(min_graph_count);
        rgi->set_open.resize(min_graph_count);
    }
    
}

void RuleGenerator::combineGraphs(int degree, long &timecopy, long &timeattach)
{
    for (std::vector<RuleGraph>::iterator it = open_graphs[degree - 1].begin(); it != open_graphs[degree - 1].end(); it ++)
    {
        int il = it->set_open.find_first();
        while ((il > -1) && (il < it->set_open.size()))
        {
            boost::dynamic_bitset<> signature(it->getSize());
            std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> range = min_graphs_by_root.equal_range(il);
            while (range.first != range.second)
            {
                RuleGraph ming = min_graphs[range.first->second];
            //RuleGraph att = min_graphs[il];
            RuleGraph att = ming;
            //att.combined_symbols = min_graphs[il].combined_symbols;
            signature = it->set_history;
            signature |= att.set_history;
            if (graphs_by_signature.find(signature) != graphs_by_signature.end())
            {
                range.first ++;
                //il = it->set_open.find_next(il);
                continue;
            }

            if (!noLimits)
            {
             if ((it->getLeaves() + att.getLeaves() - 1) > limit_symbols)
             {
                 range.first ++;
                 //il = it->set_open.find_next(il);
                 continue;
             }
            }

 
                        RuleGraph rg(false); // = new RuleGraph(*it);
                         rg.copy(*it);
                        rg.attach(il, roots_edges[il], att);
                        
                        rg.setLeaves(rg.getLeaves() + att.getLeaves());

                        if (rg.getCompositionLevel() > limit_compositions)
                        {
                            range.first ++;
                            //il = it->set_open.find_next(il);
                            continue;
                        }

                        try
                        {
                        if (rg.freeLeavesCount() > 0)
                        {
                            open_graphs[rg.getCompositionLevel() - 1].push_back(rg);
                        }
                        else
                        {
                            closed_graphs[rg.getCompositionLevel() - 1].push_back(rg);
                        }
                        }
                             catch ( const std::bad_alloc & e )
                             {
                                 //zrobione to tylko po to, zeby nie wysypywalo sie w trybie no limits, jak nie ma miejsca w wektorze czy cos
                             }



                        try
                        {
                        graphs_by_signature.insert(std::pair<boost::dynamic_bitset<>, RuleGraph> (signature, rg));
                        }
                             catch ( const std::bad_alloc & e )
                             {
                                 //zrobione to tylko po to, zeby nie wysypywalo sie w trybie no limits, jak nie ma miejsca w wektorze czy cos
                                }

            //il = it->set_open.find_next(il);
            range.first ++;
            }
            il = it->set_open.find_next(il);
        }
    }
}

std::vector<std::string> RuleGenerator::combineGraphs()
{
    long timecopy = 0;
    long timeattach = 0;
    std::vector<std::string> ret;
    std::multimap<RuleElem, std::string> nodoubles;
    int k = 1;
    while (k < limit_compositions)
    {
        combineGraphs(k, timecopy, timeattach);
        if ((open_graphs[k].size() == 0) && (closed_graphs[k].size() == 0))
        {
            break;
        }
        k ++;
    }
    long timeget = 0;
    
    for (int hh = 0; hh < min_graphs.size(); hh ++)
    {
    for (int deg = 0; deg < limit_compositions; deg ++)
    {
        for (std::vector<RuleGraph>::iterator it = open_graphs[deg].begin(); it != open_graphs[deg].end(); it ++)
        {
            if (it->root_sig != hh)
                continue;

            if (!noLimits)
            {
                if (it->freeLeavesCount() > limit_non_terminals)
                {
                    continue;
                }
                if (it->getDepth() > limit_depth)
                {
                    continue;
                }
            }

            std::string rule = it->getRule(include_rects, include_k);
            bool doublerule = false;
            RuleElem root;
            root.first = std::pair<int, int>(it->root_start, it->root_end);
            root.second = it->getRoot();
            std::pair<std::multimap<RuleElem, std::string>::iterator, std::multimap<RuleElem, std::string>::iterator> range = nodoubles.equal_range(root);
            while (range.first != range.second)
            {
                if (range.first->second == rule)
                {
                    doublerule = true;
                    break;
                }
                range.first ++;
            }
            if (doublerule)
                continue;
            nodoubles.insert(std::pair<RuleElem, std::string>(root, rule));
            ret.push_back(rule);

        }
        for (std::vector<RuleGraph>::iterator it = closed_graphs[deg].begin(); it != closed_graphs[deg].end(); it ++)
        {
            if (it->root_sig != hh)
                continue;

            std::string rule = it->getRule(include_rects, include_k);
            bool doublerule = false;
            RuleElem root;
            root.first = std::pair<int, int>(it->root_start, it->root_end);
            root.second = it->getRoot();
            std::pair<std::multimap<RuleElem, std::string>::iterator, std::multimap<RuleElem, std::string>::iterator> range = nodoubles.equal_range(root);
            while (range.first != range.second)
            {
                if (range.first->second == rule)
                {
                    doublerule = true;
                    break;
                }
                range.first ++;
            }
            if (doublerule)
                continue;
            nodoubles.insert(std::pair<RuleElem, std::string>(root, rule));
            ret.push_back(rule);
        }
    }
    }
    return ret;
}

int RuleGenerator::getLimitCompositions()
{
    return limit_compositions;
}

void RuleGenerator::setLimitCompositions(int aLimit)
{
    limit_compositions = aLimit;
    std::vector<RuleGraph> c;
    std::multimap<std::string, RuleGraph> d;
    open_graphs.resize(limit_compositions, c);
    closed_graphs.resize(limit_compositions, c);
    graphs_by_root.resize(limit_compositions, d);
}

int RuleGenerator::getLimitDepth()
{
    return limit_depth;
}

void RuleGenerator::setLimitDepth(int aLimit)
{
    limit_depth = aLimit;
}

int RuleGenerator::getLimitNonTerminals()
{
    return limit_non_terminals;
}

void RuleGenerator::setLimitNonTerminals(int aLimit)
{
    limit_non_terminals = aLimit;
}

int RuleGenerator::getLimitSymbols()
{
    return limit_symbols;
}

void RuleGenerator::setLimitSymbols(int aLimit)
{
    limit_symbols = aLimit;
}

void RuleGenerator::includeCompositionLevel()
{
    include_k = true;
}

void RuleGenerator::includeRects()
{
    include_rects = true;
}

void RuleGenerator::setCombinedSymbols()
{
    combinedSymbols = true;
}

void RuleGenerator::setNoLimits()
{
    noLimits = true;
}

void RuleGenerator::setParserSrc (puddle::Parser *aParser)
{
    parser_src = aParser;
}

void RuleGenerator::setParserTrg (puddle::Parser *aParser)
{
    parser_trg = aParser;
}

void RuleGenerator::setVariants()
{
    variants = true;
}

void RuleGenerator::setDiag(std::string id)
{
    diag_filename = "generator.diag." + id + ".txt";
    diag = true;
}

void RuleGenerator::setTagged()
{
    tagged = true;
}

void RuleGenerator::setIncludeAlignments()
{
    include_alignments_ = true;
}

void RuleGenerator::setUseWordAlignments()
{
    use_word_alignments = true;
}

void RuleGenerator::extractFromStdin()
{
    std::string sentence_src = "";
    std::string sentence_trg = "";
    std::string alignment_string = "";

//    boost::u32regex regData = boost::make_u32regex("^# (.+)");
//    boost::u32regex regL1 = boost::make_u32regex("^L1: (.+)");
//    boost::u32regex regL2 = boost::make_u32regex("^L2: (.+)");
//    boost::u32regex regAlignment = boost::make_u32regex("^Alignment: (.+)");
    boost::regex regData("^(.+)");
    boost::regex regL1("^L1: (.+)");
    boost::regex regL2("^L2: (.+)");
    boost::regex regAlignment("^Alignment: (.+)");
    boost::smatch container;

    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    int done = 0;
    int timeparse = 0;
    int timegen1 = 0;
    int timegen2 = 0;
    int timegen3 = 0;
    std::string line;
    long int lines = 0;
    while (getline(std::cin, line))
    {
        //if (boost::u32regex_match(line, container, regData))
        if (boost::regex_match(line, container, regData))
        {
            std::string content = container[1];

            //if (boost::u32regex_match(content, container, regL1))
            if (boost::regex_match(content, container, regL1))
                sentence_src = container[1];
            //if (boost::u32regex_match(content, container, regL2))
            if (boost::regex_match(content, container, regL2))
                sentence_trg = container[1];
            //if (boost::u32regex_match(content, container, regAlignment))
            if (boost::regex_match(content, container, regAlignment))
                alignment_string = container[1];

            if ((sentence_src != "") && (sentence_trg != "") && (alignment_string != ""))
            {
                ParseGraphPtr pg_src;
                if (tagged)
                    pg_src = parser_src->parseTaggedString(sentence_src);
                else
                    pg_src = parser_src->parseString(sentence_src);

                std::string normal_trg = sentence_trg;

                std::vector<RectCoord> rects = rectGenerator.generate(sentence_src, normal_trg, alignment_string, pg_src);
                std::vector<std::string> rules;
                if (!use_word_alignments)
                    rules = generateRules(pg_src, rects, normal_trg, include_alignments_);
                else
                    rules = generateRules(pg_src, rects, normal_trg, alignment_string);
                std::cout << "# L1: " << sentence_src << std::endl;
                std::cout << "# L2: " << sentence_trg << std::endl;
                std::cout << "# Alignment: [" << alignment_string << "]" << std::endl;
                std::cout << std::endl;
                for (std::vector<std::string>::iterator it = rules.begin(); it != rules.end(); it ++)
                {
                    std::cout << *it << std::endl;
                }
                std::cout << std::endl;
                done ++;
                if (done % 100 == 0)
                {
                    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
                    boost::posix_time::time_duration delta = pt_end - pt_start;
                    std::cerr << done << " sentences done (" << delta.total_milliseconds() << " ms)." << std::endl;
                }

                sentence_src = "";
                sentence_trg = "";
                alignment_string = "";
            }
        }
        lines ++;
        if (diag)
        {
            std::ofstream tmpoutput(diag_filename.c_str());
            tmpoutput << lines;
            tmpoutput.close();
        }
    }

    if (done % 100 != 0)
    {
        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration delta = pt_end - pt_start;
        std::cerr << done << " sentences done (" << delta.total_milliseconds() << " ms)." << std::endl;
    }

}

//std::vector<RuleStruct> RuleGenerator::extractFromFile(std::string filename)
void RuleGenerator::extractFromFile(std::string filename, std::string output_filename)
{
    std::ifstream input(filename.c_str());
    std::string sentence_src = "";
    std::string sentence_trg = "";
    std::string alignment_string = "";

//    boost::u32regex regData = boost::make_u32regex("^# (.+)");
//    boost::u32regex regL1 = boost::make_u32regex("^L1: (.+)");
//    boost::u32regex regL2 = boost::make_u32regex("^L2: (.+)");
//    boost::u32regex regAlignment = boost::make_u32regex("^Alignment: (.+)");
    boost::regex regData("^(.+)");
    boost::regex regL1("^L1: (.+)");
    boost::regex regL2("^L2: (.+)");
    boost::regex regAlignment("^Alignment: (.+)");
    boost::smatch container;

    std::ofstream output(output_filename.c_str());

    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    int done = 0;
    int timeparse = 0;
    int timegen1 = 0;
    int timegen2 = 0;
    int timegen3 = 0;
    long int lines = 0;
    while (!input.eof())
    {
        std::string line;
        getline(input, line);

        //if (boost::u32regex_match(line, container, regData))
        if (boost::regex_match(line, container, regData))
        {
            std::string content = container[1];

            //if (boost::u32regex_match(content, container, regL1))
            if (boost::regex_match(content, container, regL1))
                sentence_src = container[1];
            //if (boost::u32regex_match(content, container, regL2))
            if (boost::regex_match(content, container, regL2))
                sentence_trg = container[1];
            //if (boost::u32regex_match(content, container, regAlignment))
            if (boost::regex_match(content, container, regAlignment))
                alignment_string = container[1];

            if ((sentence_src != "") && (sentence_trg != "") && (alignment_string != ""))
            {
                ParseGraphPtr pg_src;
                if (tagged)
                    pg_src = parser_src->parseTaggedString(sentence_src);
                else
                    pg_src = parser_src->parseString(sentence_src);
                std::string normal_trg = sentence_trg;

                std::vector<RectCoord> rects = rectGenerator.generate(sentence_src, normal_trg, alignment_string, pg_src);
                std::vector<std::string> rules;
                if (!use_word_alignments)
                    rules = generateRules(pg_src, rects, normal_trg, include_alignments_);
                else
                    rules = generateRules(pg_src, rects, normal_trg, alignment_string);
                output << "# L1: " << sentence_src << std::endl;
                output << "# L2: " << sentence_trg << std::endl;
                output << "# Alignment: " << alignment_string << std::endl;
                output << std::endl;
                for (std::vector<std::string>::iterator it = rules.begin(); it != rules.end(); it ++)
                {
                    output << *it << std::endl;
                }
                output << std::endl;
                done ++;
                if (done % 100 == 0)
                {
                    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
                    boost::posix_time::time_duration delta = pt_end - pt_start;
                    std::cerr << done << " sentences done (" << delta.total_milliseconds() << " ms)." << std::endl;
                }

                sentence_src = "";
                sentence_trg = "";
                alignment_string = "";
            }
        }
        if (diag)
        {
            std::ofstream tmpoutput(diag_filename.c_str());
            tmpoutput << lines;
            tmpoutput.close();
        }
        lines ++;
    }
    output.close();

    if (done % 100 != 0)
    {
        boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration delta = pt_end - pt_start;
        std::cerr << done << " sentences done (" << delta.total_milliseconds() << " ms)." << std::endl;
    }
    input.close();
}

}

}

