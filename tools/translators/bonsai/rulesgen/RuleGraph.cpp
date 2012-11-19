// # L1: Jego oczy wyglądały jak pęcherze na wiekowej skórze . 
// # L2: sus ojos parecían ampollas sobre su piel arrugada .
// # Alignment: [[1,1],[2,2],[3,3],[5,4],[6,5],[7,8],[8,7],[9,9]]

#include "RuleGraph.hpp"

namespace poleng
{

namespace bonsai
{

RuleGraph::RuleGraph()
{
    combined_symbols = false;
}

RuleGraph::RuleGraph(bool combined)
{
    combined_symbols = combined;
}

RuleGraph::~RuleGraph()
{
}

//void RuleGraph::attach(ParseGraph::Edge edge, ParseGraphPtr aGraph)
void RuleGraph::attach(int index, ParseGraph::Edge edge, RuleGraph rg)
{
//    std::cerr << "Do grafu: " << std::endl << graph->write_graphviz() << std::endl;
//    std::cerr << "Dolanczam: " << std::endl << rg.getGraph()->write_graphviz() << std::endl;

    ParseGraph::Graph *g = rg.getGraph()->getBoostGraph();
//    if (vertex_index > boost::num_vertices(*g))
//    {
//        std::cerr << "Nielegalny indeks w attach: " << vertex_index << ", a wierzcholkow: " << boost::num_vertices(*g) << std::endl;
//        return;
//    }

//    ParseGraph::Vertex at = vertex(vertex_index, *graph);
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
    ParseGraph::TransitionMap map2 = get(boost::edge_bundle, *(this->graph->getBoostGraph()));

//    int max = 0;
//    v = vertex(vertex_index, *g);
//    for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
//    {
//        ParseGraph::Edge e = *p.first;
//        if (map[e].getDepth() > max)
//            max = map[e].getDepth();
//    }

    bool found_over = false;
    std::string label = "";
    int i = 0;
    v = vertex(0, *g);
    while (v != end)
    {
        v = vertex(i, *g);
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
            ParseGraph::Edge e = *p.first;
            if ((i == map2[edge].getStart()) && (map[e].getDepth() == map2[edge].getDepth()))
            {
//                if (combined_symbols)
//                    label = map[e].getLabelDesc();
//                else
                    label = map[e].getLabel();
                found_over = true;
                continue;
            }
            graph->add_edge(map[e].getStart(), map[e].getEnd(), map[e]);
        }
        i ++;
    }
//    std::cerr << "po krawedziach" << std::endl;
    
//    if (found_over)
//    {
//        for (std::pair<std::multimap<std::string, ParseGraph::Edge>::iterator, std::multimap<std::string, ParseGraph::Edge>::iterator> p = free_leaves.equal_range(label); p.first != p.second; ++p.first)
//        {
//            if (map[(p.first)->second].getLabel() == map[edge].getLabel() && map[(p.first)->second].getStart() == map[edge].getStart() && map[(p.first)->second].getEnd() == map[edge].getEnd() && map[(p.first)->second].getDepth() == map[edge].getDepth())
//            //if ((p.first)->second == edge)
//            {
//                free_leaves.erase(p.first);
//                break;
//            }
////            if ((p.first)->second == map[edge].getStart())
////            {
////                free_leaves.erase(p.first);
////                break;
////            }
//        }
//        free_leaves.insert(rg.free_leaves.begin(), rg.free_leaves.end());
//    }

    set_history |= rg.set_history;

    set_open |= rg.set_open;
    set_open[index] = 0;
//    std::cerr << "po historii/open" << std::endl;

//    std::cerr << "lista lisci przed dodaniem" << std::endl;
//    for (std::vector<ParseGraph::Edge>::iterator it = leaves_list.begin(); it != leaves_list.end(); it ++)
//        std::cerr << map[*it].getLabel() << "/" << map[*it].getDepth() << " " << map[*it].getStart() << " -> " << map[*it].getEnd() << std::endl;

    for (std::vector<ParseGraph::Edge>::iterator it = leaves_list.begin(); it != leaves_list.end(); it ++)
    {
        if (map[*it].getLabel() == map[edge].getLabel() && map[*it].getStart() == map[edge].getStart() && map[*it].getEnd() == map[edge].getEnd() && map[*it].getDepth() == map[edge].getDepth())
        //if (*it == edge)
        {
            std::vector<ParseGraph::Edge>::iterator j = leaves_list.erase(it);
            leaves_list.insert(j, rg.leaves_list.begin(), rg.leaves_list.end());
            break;
        }
    }
//    std::cerr << "po lisciach" << std::endl;

//    if (free_leaves.size() > 0)
//    {
    int old_depth = 0;
    for (std::map<ParseGraph::Edge, int>::iterator di = depths.begin(); di != depths.end(); di ++)
    {
        if (map[di->first].getLabel() == map[edge].getLabel() && map[di->first].getStart() == map[edge].getStart() && map[di->first].getEnd() == map[edge].getEnd() && map[di->first].getDepth() == map[edge].getDepth())
        //if (di->first == edge)
        {
            old_depth = di->second;
            depths.erase(di);
//            std::cerr << "stara glebokosc: " << old_depth << std::endl;
            break;
        }
    }

    if (rg.freeLeavesCount() > 0)
    {
    for (std::map<ParseGraph::Edge, int>::iterator di = rg.depths.begin(); di != rg.depths.end(); di ++)
    {
        depths.insert(std::pair<ParseGraph::Edge, int>(di->first, di->second + old_depth));
    }
    //depths.insert(rg.depths.begin(), rg.depths.end()); TODO: tak to chyba bylo za prosto, czy rozwiazanie powyzsze zdaje egzamin?

    if ((old_depth + rg.getDepth()) > depth)
    {
//        std::cerr << "Ustawiam glebokosc nowa: " << (old_depth + rg.getDepth()) << " z " << old_depth << " + " << rg.getDepth() << std::endl;
        depth = old_depth + rg.getDepth();
    }
    }
    else
    {
        int max_depth = 0;
        if (freeLeavesCount() > 0)
        {
            for (std::map<ParseGraph::Edge, int>::iterator di = depths.begin(); di != depths.end(); di ++)
            {
                if (di->second > max_depth)
                    max_depth = di->second;
            }
        }
        depth = max_depth;
    }
//    std::cerr << "po glebokosciach" << std::endl;
//    else
//    {
//        std::cerr << "Zostawiam glebokosc, bo mam tu: " << old_depth << " + " << rg.getDepth() << std::endl;
//    }
//    }
//    else
//    {
//        depth = 0;
//        depths.clear();
//    }

//    std::cerr << "lista lisci po dodaniu" << std::endl;
//    for (std::vector<ParseGraph::Edge>::iterator it = leaves_list.begin(); it != leaves_list.end(); it ++)
//        std::cerr << map[*it].getLabel() << "/" << map[*it].getDepth() << " " << map[*it].getStart() << " -> " << map[*it].getEnd() << std::endl;

    leaves --; //be jeden wierzcholek zajety zostal. trzeba jeszcze dodac z kolei liczbe wierzcholkow z dodanego grafu, ale to jest TODO, na razie robione w innym miejscu

    composition_level += rg.getCompositionLevel();

//TUTEJ 

    int start = map[edge].getStart();
//
//    int before_src = 0;
//    //for (std::map<std::pair<int, int>, std::string>::iterator it = rule_contents.begin(); it != rule_contents.end(); it ++)
//    for (RuleElems::iterator it = rule_contents.begin(); it != rule_contents.end(); it ++)
//    {
//        if (it->first.first < start)
//            before_src ++;
//        else
//            break;
//    }
//
//    if (rg.getLeaves() > 1)
//    {
//        std::map<int, std::vector<int> > new_alignments = rule_alignments;
//        for (std::map<int, std::vector<int> >::iterator it = rule_alignments.begin(); it != rule_alignments.end(); it ++)
//        {
//            if (it->first >= start) //TODO: czy > ?
//                new_alignments.insert(std::pair<int, std::vector<int> >(it->first + (rg,getLeaves() - 1), it->second));
//            else
//                new_alignments.insert(*it);
//        }
//        rule_alignments = new_alignments;
//    }
//    if (before_src > 0)
//        before_src ++;

    std::pair<int, int> coord = std::pair<int, int>(map[edge].getStart(), map[edge].getEnd());
    rule_contents.erase(std::pair<std::pair<int, int>, std::string>(coord, map[edge].getLabel()));
//    rule_contents.erase(std::pair<std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()));
    rule_contents.insert(rg.rule_contents.begin(), rg.rule_contents.end());
//    std::cerr << "po kontencie" << std::endl;

    int tr_start = rg.rule_translations.begin()->first.first;
    int tr_end = rg.rule_translations.rbegin()->first.second;


    RuleElems::iterator itrt = rule_translations.begin();
    while (itrt != rule_translations.end())
    {
        //if (itrt->first.first == tr_start && itrt->first.second == tr_end)
        if (itrt->first.first == tr_start)
            break;
        itrt ++;
    }
    if (itrt != rule_translations.end())
    {
//        std::cerr << "znalazlem" << std::endl;
//        std::cerr << "tr_start = " << tr_start << std::endl << "tr_end: "<< tr_end << std::endl;

        if (itrt->first.second > tr_end)
        {
//            std::cerr << "Za krotki, konczy sie na: " << tr_end <<  " a mial siegac do: " << itrt->first.second << "; a ja jestem: "  << getRoot() << std::endl;
            for (int x = tr_end; x < itrt->first.second; x ++)
           {
                
                Coord tr_coord(x, x + 1);
                rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, trg_toks[x]));
//                std::cerr << "dorzucam: " << trg_toks[x] << std::endl;
            }
        }
        rule_translations.erase(itrt);
//        std::cerr << "usunalem" << std::endl;
        rule_translations.insert(rg.rule_translations.begin(), rg.rule_translations.end());
//        std::cerr << "dolozylem" << std::endl;
    }
    else
    {
//        std::cerr << "Nie mam w tlumaczeniu elementu o indeksie (" << tr_start << "," << tr_end << ")" << std::endl;
    }
//    std::cerr << "polowa" << std::endl;

    int tr_x = tr_start;
    bool found = false;
    while (tr_x < tr_end)
    {
    std::map<int, int>::iterator itntm = non_terminals_trg_mapped.find(tr_x);
    if (itntm != non_terminals_trg_mapped.end())
    {
        non_terminals_trg_mapped.erase(itntm);
        non_terminals_trg_mapped.insert(rg.non_terminals_trg_mapped.begin(), rg.non_terminals_trg_mapped.end());
        found = true;
        break;
    }
    else
    {
//        std::cerr << "Nie znalazlem mapowania nieterminali " << tr_x << " --> " << map[edge].getStart() << std::endl;
     }
    tr_x ++;
    }
//    std::cerr << "po translacjach" << std::endl;

    rule_alignments.insert(rg.rule_alignments.begin(), rg.rule_alignments.end());

//    std::cerr << "po alajmentach" << std::endl;
//    non_terminals_src[start] = 0;
//    non_terminals_src |= rg.non_terminals_src;

//TODO |= ?
    int il = rg.non_terminals_src.find_first();
    non_terminals_src[start] = 0;
    while ((il > -1) && (il < rg.non_terminals_src.size()))
    {
        non_terminals_src[il] = 1;
        il = rg.non_terminals_src.find_next(il);
    }
    
    include_alignments = rg.include_alignments;

    std::map<int, RuleRect>::iterator rect_it = non_rects.find(rg.root_start);
    if (rect_it != non_rects.end())
    {
        if (rect_it->second.first.first == rg.root_rect.first.first && rect_it->second.first.second == rg.root_rect.first.second && rect_it->second.second.first == rg.root_rect.second.first && rect_it->second.second.second == rg.root_rect.second.second)
        {
            non_rects.erase(rect_it);
        }
        else
        {
//            std::cerr << "znalazlem prostokat, ale nie jest on taki sam!" << std::endl;
        }
    }
    else
    {
//        std::cerr << "nie znalazlem prostokatu zaczynajacego sie od: " << rg.root_start << std::endl;
    }

    non_rects.insert(rg.non_rects.begin(), rg.non_rects.end());
//    std::cerr << "po non_terminals_src" << std::endl;

//    int add = 0;
//    il = non_terminals_trg.find_first();
//    while ((il > -1) && (il < rg.rect_start) && (il < non_terminals_trg.size()))
//    {
//        add ++;
//        if (non_terminals_trg.find_next(il) >= rg.rect_start)
//            break;
//        il = non_terminals_trg.find_next(il);
//    }
//
////    for (int x = rg.rect_start; x < rg.rect_end; x ++)
////    {    
//        non_terminals_trg[rg.rect_start] = 0;
//        //non_terminals_trg_mapped[rg.rect_start] = -1;
////    }
//    int more = 0;
//    il = rg.non_terminals_trg.find_first();
//    //while ((il > -1) && (il < rg.non_terminals_trg.size()))
//    while ((il > -1) && (il < rg.rect_end))
//    {
//        non_terminals_trg[il] = 1;
//        //non_terminals_trg_mapped[il] = add + rg.non_terminals_trg_mapped[il];
//        il = rg.non_terminals_trg.find_next(il);
//        more ++;
//    }
//    //if (more)
//        more --;
//    il = non_terminals_trg.find_next(rg.rect_end - 1);
//    while ((il > -1) && (il < non_terminals_trg.size()))
//    {
//        //non_terminals_trg[il] = 1;
//        //non_terminals_trg_mapped[il] = more + non_terminals_trg_mapped[il];
//        il = non_terminals_trg.find_next(il);
//    }
//    for (int ni = rg.rect_start; ni < rg.rect_end; ni ++)
//    {
//        non_terminals_trg_mapped[ni] = add + rg.non_terminals_trg_mapped[ni];
//    }

//    int before_tr = 0;
//    for (int tri = 0; tri < rg.rule_translations.size(); tri ++)
//    {
//        if (tri < rg.rect_start)
//            if (rule_translations[tri] != "")
//                before_tr ++;
//        if (rg.rule_translations[tri] != "")
//        {
//            rule_translations[tri] = rg.rule_translations[tri];
//        }
//    }
//    if (before_tr > 0)
//        before_tr ++;

//    std::cerr << "Rule alajnments przed: " << std::endl; 
//    for (std::map<int, std::vector<int> >::iterator it = rule_alignments.begin(); it != rule_alignments.end(); it ++)
//        for (std::vector<int>::iterator uu = it->second.begin(); uu != it->second.end(); uu ++)
//            std::cerr << "[" << it->first << "," << *uu << "]" << std::endl;
//    std::cerr << "Rule alajnments dolaczane: " << std::endl; 
//    for (std::map<int, std::vector<int> >::iterator it = rg.rule_alignments.begin(); it != rg.rule_alignments.end(); it ++)
//        for (std::vector<int>::iterator uu = it->second.begin(); uu != it->second.end(); uu ++)
//            std::cerr << "[" << it->first << "," << *uu << "]" << std::endl;
//    std::cerr << "before_src = " << before_src << std::endl << "before_tr = " << before_tr << std::endl;

//    for (std::map<int, std::vector<int> >::iterator it = rg.rule_alignments.begin(); it != rg.rule_alignments.end(); it ++)
//    {
//        std::vector<int> alis = it->second;
//        for (int x = 0; x < alis.size(); x ++)
//            alis[x] += before_tr;
////        if (rule_alignments.find(before_src + it->first) != rule_alignments.end())
////        {
////            std::map<int, std::vector<int> >::iterator q = rule_alignments.find(before_src + it->first);
////            while (q != rule_alignments.end())
////            {
////                q ++;
////            }
////        }
//        rule_alignments.insert(std::pair<int, std::vector<int> >(before_src + it->first, alis));
//    }
//    std::cerr << "Rule alajnments post factum: " << std::endl; 
//    for (std::map<int, std::vector<int> >::iterator it = rule_alignments.begin(); it != rule_alignments.end(); it ++)
//        for (std::vector<int>::iterator uu = it->second.begin(); uu != it->second.end(); uu ++)
//            std::cerr << "[" << it->first << "," << *uu << "]" << std::endl;

}

std::string RuleGraph::getRoot()
{
    return root_label;
}

int RuleGraph::getCompositionLevel()
{
    return composition_level;
}

ParseGraphPtr RuleGraph::getGraph()
{
    return graph;
}

int RuleGraph::freeLeavesCount()
{
    return set_open.count();
    //return free_leaves.size();
}

void RuleGraph::setRoot(std::string aRoot)
{
    root_label = aRoot;
}

void RuleGraph::setCompositionLevel(int aLevel)
{
    composition_level = aLevel;
}

void RuleGraph::setDepth(int aDepth)
{
    depth = aDepth;
}

int RuleGraph::getDepth()
{
    return depth;
}

void RuleGraph::setLeaves(int aLeaves)
{
    leaves = aLeaves;
}

int RuleGraph::getLeaves()
{
    return leaves;
}

void RuleGraph::setGraph(ParseGraphPtr aGraph, std::map<ParseGraph::Edge, int> roots_indexes, std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects, std::map<int, std::vector<int> > words_aligned)
{
    include_alignments = true;
    graph = aGraph;
//    free_leaves.clear();
    leaves = 0;
    depth = 0;
    leaves_list.clear();
    set_open.reset();

    rule_contents.clear();
    rule_translations.clear();
    rule_alignments.clear();
    non_terminals_src.clear();
    non_terminals_trg.clear();
    non_terminals_trg_mapped.clear();

    depths.clear();

    non_rects.clear();

    trg_toks.clear();
//    rule_translations.insert(rule_translations.begin(), trgTokens.size(), "");
    ParseGraph::Graph *g = graph->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

//    rule_translations.insert(rule_translations.begin(), trgTokens.begin(), trgTokens.end());
    non_terminals_src.resize(boost::num_vertices(*g) - 1, false);
    non_terminals_trg.resize(trgTokens.size(), false);
    //non_terminals_trg_mapped.insert(non_terminals_trg_mapped.begin(), trgTokens.size(), -1);
    rect_start = 1000;
    rect_end = -1;

    bool found_over = false;
    int first_vertex = 0;
    while (first_vertex < (num_vertices(*g) - 1))
    {
        v = vertex(first_vertex, *g);
        if (v != end)
        {
            std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g);
            if (p.first == p.second)
            {
                first_vertex ++;
                continue;
            }
            else
                break;
        }
    }

    std::vector<int> ending(boost::num_vertices(*g), 0);

    int non_terminals = 0;

    int i = first_vertex;
    v = vertex(i, *g);
    int max = 0;
    std::string root = "";
    int r_start = 0;
    int r_end = 0;
    int all_edges = 0;
    int passed = 0;
    int last_end = -1;
    while (v != end)
    {
//        std::cerr << "i: " << i << std::endl;
        v = vertex(i, *g);
        int min = 1000;
        ParseGraph::Edge edge;
        bool found = false;
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
//            std::cerr << "krawedz!" << std::endl;
            ParseGraph::Edge e = *p.first;

//            if (!found_over)
//            {
//                this->setRoot(map[e].getLabel());
//                std::cerr << "ustawiam na ruta: " <<  map[e].getLabel() << std::endl;
//                found_over = true;
//            }
            if (i == first_vertex)
            {
                if (map[e].getDepth() > max)
                {
                    max = map[e].getDepth();
//                    if (combined_symbols)
//                        root = map[e].getLabelDesc();
//                    else
                        root = map[e].getLabel();
                    r_start = map[e].getStart();
                    r_end = map[e].getEnd();
                }
            }

            if (map[e].getDepth() < min)
            {
                min = map[e].getDepth();
                edge = e;
                found = true;
            }

            ending[map[e].getEnd() - 1] ++;
            all_edges ++;

        }
        int length = 1;
        passed += ending[i];
        if (found)
        {
            if (map[edge].getDepth() > 0)
            {
                length = map[edge].getEnd() - map[edge].getStart();
                if (length > 1)
                {
                    for (int ii = i + 1; ii < (i + length); ii ++)
                        passed += ending[ii];
                }
//                free_leaves.insert(std::pair<std::string, ParseGraph::Edge>(map[edge].getLabel(), edge));
                for (std::map<ParseGraph::Edge, int>::iterator ri = roots_indexes.begin(); ri != roots_indexes.end(); ri ++)
                {
                    if (map[ri->first].getLabel() == map[edge].getLabel() && map[ri->first].getStart() == map[edge].getStart() && map[ri->first].getEnd() == map[edge].getEnd() && map[ri->first].getDepth() == map[edge].getDepth())
                    {
                        set_open[ri->second] = 1;
                        break;
                    }
                }

//                std::cerr << "i = " << i << std::endl;
//                std::cerr << "Ziom, all_edges  = " << all_edges << ", zas passed = " << passed << std::endl;
                depths.insert(std::pair<ParseGraph::Edge, int>(edge, ending[map[edge].getEnd() - 1] - 1 + (all_edges - passed)));
//                std::cerr << "w efekcie glebokosc: " << (ending[map[edge].getEnd() - 1] - 1 + (all_edges - passed)) << std::endl;
            }
//                std::cerr << "moj wystrzalowy set_open: ";
//                for (boost::dynamic_bitset<>::size_type xx = 0; xx < set_open.size(); xx ++)
//                    std::cerr << set_open[xx];
//                std::cerr << std::endl;

            leaves_list.push_back(edge);
            leaves ++;

            int start = map[edge].getStart();
            int end = map[edge].getEnd();
            //rule_contents.push_back(map[edge].getLabel());
            if (map[edge].getDepth() == 0)
            {
//                if (combined_symbols)
//                    rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), map[edge].getLabelDesc()));
//                else

                    rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), map[edge].getLabel()));
                    std::map<int, std::vector<int> >::iterator itwa = words_aligned.find(start);
                    if (itwa != words_aligned.end())
                    {
                        rule_alignments.insert(*itwa);
                    }
                    else
                    {
                        std::vector<int> als;
                        als.push_back(-1);
                        rule_alignments.insert(std::pair<int, std::vector<int> >(start, als));
//                        std::cerr << "Nie znalazlem slow dopasowanych do: " << start << std::endl;
                    }
            }
            else
            {
                std::string label;
//                if (combined_symbols)
//                    label = "<" + map[edge].getLabelDesc() + ">";
//                else
                    label = "<" + map[edge].getLabel() + ">";
                //label += "[" + boost::lexical_cast<std::string>(non_terminals) + "]";
                rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), label));
                non_terminals_src[start] = 1;
            }

//            std::string contents;
//            for (std::map<std::pair<int, int>, std::string>::iterator it = rule_contents.begin(); it != rule_contents.end(); it ++)
//            {
//                if (contents != "")
//                    contents += " ";
//                contents += it->second;
//            }

            Coord ks(start, end);
            std::map<Coord, Coord>::iterator itar = align_rects.find(ks);
            if (itar != align_rects.end())
            {
                int tr_start = itar->second.first;
                int tr_end = itar->second.second;

//                if (last_end >= 0)
//                {
//                    if (tr_start > last_end)
//                    {
//                        for (int x = last_end; x < tr_start; x ++)
//                        {
//                            std::string x_label = trgTokens[x];
//                            non_terminals_trg[x] = 0;
//                            Coord tr_coord(x, x + 1);
//                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
//                    std::cerr << "wsadzam tlumaczenie brakujace dla: " << map[edge].getLabel() << " - " << x_label << std::endl;
//                        }
//                        std::cerr << "tr_start = " << tr_start << " a last_end: " << last_end << std::endl;
//                    }
//                }

                std::string tr_label;
                if (map[edge].getDepth() > 0)
                {
                    tr_label = "<X>";
                    non_terminals_trg[tr_start] = 1;
                    Coord tr_coord(tr_start, tr_end);
                    rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, tr_label));
                    non_terminals_trg_mapped.insert(std::pair<int, int>(tr_start, start));
//                    std::cerr << "wstawiam <X> od: " << tr_label << "  " << tr_start << " ==> " << start << std::endl;
                    Coord rect_y(tr_start, tr_end);
                    RuleRect rule_rect(ks, rect_y);
                    non_rects.insert(std::pair<int, RuleRect>(start, rule_rect));
                }
                else
                {
                    //while (tr_end != tr_start)
                    while (tr_start < tr_end)
                    {
                        tr_label = trgTokens[tr_start];
                        non_terminals_trg[tr_start] = 0;
                        Coord tr_coord(tr_start, tr_start + 1);
                        rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, tr_label));
                        tr_start ++;
                    }
                }
                last_end = tr_end;
            }
            else
            {
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;
                if (map[edge].getDepth() > 0)
                {
                    std::string x_label = "<X>";
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;

                }
                else
                {
                    std::map<int, std::vector<int> >::iterator itwa = words_aligned.find(start);
                    if (itwa != words_aligned.end())
                    {
                        for (std::vector<int>::iterator word = itwa->second.begin(); word != itwa->second.end(); word ++)
                        {
                            std::string x_label = trgTokens[*word];
                            non_terminals_trg[*word] = 0;
                            Coord tr_coord(*word, *word + 1);
                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
                            last_end += *word + 1;
                        }
                    }
                    else
                    {
                        //w tlumaeczniu nic, jak nie ma slow dopasowanych
//                        std::cerr << "Nie znalazlem slow dopasowanych do: " << start << std::endl;
                    }
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;
                }
            }

//            Coord k;
//            if (map[edge].getDepth() == 0)
//                k = Coord(r_start, r_end);
//            else
//                k = Coord(start, end);
//            std::map<Coord, Coord>::iterator itr = align_rects.find(k);
//            int trans_start, trans_end;
//            if (itr != align_rects.end())
//            {
//                trans_start = itr->second.first;
//                trans_end = itr->second.second;
//
//                if (trans_start < rect_start)
//                    rect_start = trans_start;
//                if (trans_end > rect_end)
//                    rect_end = trans_end;
////                for (int t = itr->second.first; t < itr->second.second; t ++)
////                    rule_translations[t] = trgTokens[t];
//            }
//            else
//            {
////                std::cerr << "nie znalazlem alajn rekta od (" << start << "," << end << ")" << std::endl;
//                if (map[edge].getDepth() == 0)
//                    trans_start = 0;
//                else
//                {
//                    trans_start = start;
//                    trans_end = end;
//                if (trans_start < rect_start)
//                    rect_start = trans_start;
//                if (trans_end > rect_end)
//                    rect_end = trans_end;
////                    std::cerr << "Houston, mamy problem" << std::endl;
//                }
//            }

//            if (map[edge].getDepth() > 0)
//            {
//                std::map<int, std::vector<int> >::iterator ai = words_aligned.find(start);
//                if (ai != words_aligned.end())
//                {
//                   //rule_translations[ai->second.front()] = "<X>[" + boost::lexical_cast<std::string>(non_terminals) + "]";
//                   rule_translations[ai->second.front()] = "<X>";
//                }
//                //rule_translations[trans_start] = "<X>";
//                if (trans_end - trans_start > 1)
//                for (int t = trans_start + 1; t < trans_end; t ++)
//                    rule_translations[t] = "";
//
//                non_terminals_trg_mapped[trans_start] = non_terminals;
//                non_terminals_trg[trans_start] = 1;
//                non_terminals ++;
//            }
//            else
//            {
//                std::map<int, std::vector<int> >::iterator ai = words_aligned.find(start);
//                if (ai != words_aligned.end())
//                {
//                    std::vector<int> alis;
//                    for (std::vector<int>::iterator it = ai->second.begin(); it != ai->second.end(); it ++)
//                    {
////                        std::cerr << "bach: (" << ai->first << "," << *it << ")" << std::endl;
//                        rule_translations[*it] = trgTokens[*it];
//                        alis.push_back(*it - trans_start);
//                    }
//                    rule_alignments.insert(std::pair<int, std::vector<int> >(ai->first - start, alis));
//                }
//                else
//                {
////                    std::cerr << "nie mam!" << std::endl;
//                    std::vector<int> alis;
//                    alis.push_back(-1);
//                    rule_alignments.insert(std::pair<int, std::vector<int> >(start - r_start, alis));
//                }
//            }
        }

        //for (int ii = i; ii < (i + length); ii ++)
        //{
        //}
//        if (found)
//        {
//            if (map[edge].getDepth() > 0)
//            {
//            }
//        }

//        std::cerr << "length: " << length << std::endl;
        i += length;
    }

    trans_low = -1;
    trans_high = -1;
    if (root != "")
    {
        this->setRoot(root);
        root_start = r_start;
        root_end = r_end;
        root_depth = max;

        Coord r_coord(r_start, r_end);
        std::map<Coord, Coord>::iterator arit = align_rects.find(r_coord);
        if (arit != align_rects.end())
        {
            trans_low = arit->second.first;
            trans_high = arit->second.second;
            Coord r_y(trans_low, trans_high);
            root_rect.first = r_coord;
            root_rect.second = r_y;
//            std::cerr << "takie ograniczenia: od " << arit->second.first << " do " << arit->second.second << std::endl;
        }
        else
        {
//            std::cerr << "nie ma prostokatu dla takiej reguly" << std::endl;
        }

    }

    std::string xxtr = "";
    int last = trans_low;

    for (RuleElems::iterator it = rule_translations.begin(); it != rule_translations.end(); it ++)
    {
        if (last >= 0)
        {
            if (it->first.first > last)
            {
                        for (int x = last; x < it->first.first; x ++)
                        {
                            std::string x_label = trgTokens[x];
                            non_terminals_trg[x] = 0;
                            Coord tr_coord(x, x + 1);
                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
//                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
                        }
            }
        }
        last = it->first.second;
    }
    if (last < trans_high)
    {
        for (int x = last; x < trans_high; x ++)
        {
            std::string x_label = trgTokens[x];
            non_terminals_trg[x] = 0;
            Coord tr_coord(x, x + 1);
            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
            //                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
        }
    }

//    std::string xxtr = "";
//    int last = -1;
//
//    for (RuleElems::iterator it = rule_translations.begin(); it != rule_translations.end(); it ++)
//    {
//        if (last >= 0)
//        {
//            if (it->first.first > last)
//            {
//                        for (int x = last; x < it->first.first; x ++)
//                        {
//                            std::string x_label = trgTokens[x];
//                            non_terminals_trg[x] = 0;
//                            Coord tr_coord(x, x + 1);
//                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
////                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
//                        }
//            }
//        }
//        last = it->first.second;
//    }

//    Coord ks(r_start, r_end);
//    std::map<Coord, Coord>::iterator itar = align_rects.find(ks);
//    if (itar != align_rects.end())
//    {
//        int r_tr_start = itar->second.first;
//        int r_tr_end = itar->second.second;
//        if (rule_translations.rbegin()->first.second < r_tr_end)
//        {
//            std::cerr << "Wykrylem zakrotkosc w: " << root << "; powinna siegac do: " << r_tr_end << ", a sie konczy na:"  << rule_translations.rbegin()->first.second << std::endl;
//            for (int x = rule_translations.rbegin()->first.second; x < r_tr_end; x ++)
//            {
//                std::string x_label = trgTokens[x];
//                non_terminals_trg[x] = 0;
//                Coord tr_coord(x, x + 1);
//                rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
////                std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
//            }
//        }
//    }
//    else
//    {
//        std::cerr << "Nie znalazlem prostokata dla calej reguly!" << std::endl;
//    }

    int trg_b = rule_translations.begin()->first.first;
    int trg_e = rule_translations.rbegin()->first.second;

    for (int x = trg_b; x < trg_e; x ++)
    {
        trg_toks.insert(std::pair<int, std::string>(x, trgTokens[x]));
    }

//    if (root != "")
//    {
//        this->setRoot(root);
//        root_start = r_start;
//        root_end = r_end;
//        root_depth = max;
//
//        Coord r_coord(r_start, r_end);
//        std::map<Coord, Coord>::iterator arit = align_rects.find(r_coord);
//        if (arit != align_rects.end())
//        {
//            trans_low = arit->second.first;
//            trans_high = arti->second.second;
//            //std::cerr << "takie ograniczenia: od " << arit->second.first << " do " << arit->second.second << std::endl;
//        }
//        else
//        {
//            std::cerr << "nie ma prostokatu dla takiej reguly" << std::endl;
//        }
//
//
////        Coord k = Coord(root_start, root_end);
////        std::map<Coord, Coord>::iterator itr = align_rects.find(k);
////        int trans_start, trans_end;
////        if (itr != align_rects.end())
////        {
////            trans_start = itr->second.first;
////            trans_end = itr->second.second;
//////        for (int t = 0; t < trans_start; t ++)
//////            rule_translations[t] = "";
//////        for (int t = trans_end; t < rule_translations.size(); t ++)
//////            rule_translations[t] = "";
////        }
//    }

    int max_depth = 0;
    for (std::map<ParseGraph::Edge, int>::iterator di = depths.begin(); di != depths.end(); di++)
    {
        if (di->second > max_depth)
            max_depth = di->second;
    }
    depth = max_depth;

//    std::cerr << "Regula: " << root_label << " -->" << std::endl;
//    for (RuleElems::iterator hu = rule_contents.begin(); hu != rule_contents.end(); hu ++)
//        std::cerr << "(" << hu->first.first << "," << hu->first.second << ") " << hu->second << std::endl;
//    std::cerr << "tlumaczenie:" << std::endl;
//    for (RuleElems::iterator hu = rule_translations.begin(); hu != rule_translations.end(); hu ++)
//        std::cerr << "(" << hu->first.first << "," << hu->first.second << ") " << hu->second << std::endl;
//    std::cerr << "Dopasowania: " << std::endl;
//    for (std::map<int, std::vector<int> >::iterator hu = rule_alignments.begin(); hu != rule_alignments.end();  hu ++)
//    {
//        std::cerr << hu->first << " =>";
//        std::vector<int> pic = hu->second;
//        for (std::vector<int>::iterator wac = pic.begin(); wac != pic.end(); wac ++)
//            std::cerr << " " << *wac;
//        std::cerr << std::endl;
//    }
//    std::cerr << "Dopasowania nt: " << std::endl;
//    for (std::map<int, int>::iterator hu = non_terminals_trg_mapped.begin(); hu != non_terminals_trg_mapped.end();  hu ++)
//        std::cerr << hu->first << " -=> " << hu->second << std::endl;


    composition_level = 1;

}

void RuleGraph::setGraph(ParseGraphPtr aGraph, std::map<ParseGraph::Edge, int> roots_indexes, std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects)
{
    include_alignments = false;
    graph = aGraph;
//    free_leaves.clear();
    leaves = 0;
    depth = 0;
    leaves_list.clear();
    set_open.reset();

    rule_contents.clear();
    rule_translations.clear();
//    rule_alignments.clear();
    non_terminals_src.clear();
    non_terminals_trg.clear();
    non_terminals_trg_mapped.clear();

    non_rects.clear();

    depths.clear();

    trg_toks.clear();
//    rule_translations.insert(rule_translations.begin(), trgTokens.size(), "");
    ParseGraph::Graph *g = graph->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

//    rule_translations.insert(rule_translations.begin(), trgTokens.begin(), trgTokens.end());
    non_terminals_src.resize(boost::num_vertices(*g) - 1, false);
    non_terminals_trg.resize(trgTokens.size(), false);
    //non_terminals_trg_mapped.insert(non_terminals_trg_mapped.begin(), trgTokens.size(), -1);
    rect_start = 1000;
    rect_end = -1;

    bool found_over = false;
    int first_vertex = 0;
    while (first_vertex < (num_vertices(*g) - 1))
    {
        v = vertex(first_vertex, *g);
        if (v != end)
        {
            std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g);
            if (p.first == p.second)
            {
                first_vertex ++;
                continue;
            }
            else
                break;
        }
    }

    std::vector<int> ending(boost::num_vertices(*g), 0);

    int non_terminals = 0;

    int i = first_vertex;
    v = vertex(i, *g);
    int max = 0;
    std::string root = "";
    int r_start = 0;
    int r_end = 0;
    int all_edges = 0;
    int passed = 0;
    int last_end = -1;
    while (v != end)
    {
//        std::cerr << "i: " << i << std::endl;
        v = vertex(i, *g);
        int min = 1000;
        ParseGraph::Edge edge;
        bool found = false;
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
//            std::cerr << "krawedz!" << std::endl;
            ParseGraph::Edge e = *p.first;

//            if (!found_over)
//            {
//                this->setRoot(map[e].getLabel());
//                std::cerr << "ustawiam na ruta: " <<  map[e].getLabel() << std::endl;
//                found_over = true;
//            }
            if (i == first_vertex)
            {
                if (map[e].getDepth() > max)
                {
                    max = map[e].getDepth();
//                    if (combined_symbols)
//                        root = map[e].getLabelDesc();
//                    else
                        root = map[e].getLabel();
                    r_start = map[e].getStart();
                    r_end = map[e].getEnd();
                }
            }

            if (map[e].getDepth() < min)
            {
                min = map[e].getDepth();
                edge = e;
                found = true;
            }

            ending[map[e].getEnd() - 1] ++;
            all_edges ++;

        }
        int length = 1;
        passed += ending[i];
        if (found)
        {
            if (map[edge].getDepth() > 0)
            {
                length = map[edge].getEnd() - map[edge].getStart();
                if (length > 1)
                {
                    for (int ii = i + 1; ii < (i + length); ii ++)
                        passed += ending[ii];
                }
//                free_leaves.insert(std::pair<std::string, ParseGraph::Edge>(map[edge].getLabel(), edge));
                for (std::map<ParseGraph::Edge, int>::iterator ri = roots_indexes.begin(); ri != roots_indexes.end(); ri ++)
                {
                    if (map[ri->first].getLabel() == map[edge].getLabel() && map[ri->first].getStart() == map[edge].getStart() && map[ri->first].getEnd() == map[edge].getEnd() && map[ri->first].getDepth() == map[edge].getDepth())
                    {
                        set_open[ri->second] = 1;
                        break;
                    }
                }

//                std::cerr << "i = " << i << std::endl;
//                std::cerr << "Ziom, all_edges  = " << all_edges << ", zas passed = " << passed << std::endl;
                depths.insert(std::pair<ParseGraph::Edge, int>(edge, ending[map[edge].getEnd() - 1] - 1 + (all_edges - passed)));
//                std::cerr << "w efekcie glebokosc: " << (ending[map[edge].getEnd() - 1] - 1 + (all_edges - passed)) << std::endl;
            }
//                std::cerr << "moj wystrzalowy set_open: ";
//                for (boost::dynamic_bitset<>::size_type xx = 0; xx < set_open.size(); xx ++)
//                    std::cerr << set_open[xx];
//                std::cerr << std::endl;

            leaves_list.push_back(edge);
            leaves ++;

            int start = map[edge].getStart();
            int end = map[edge].getEnd();
            //rule_contents.push_back(map[edge].getLabel());
//X            std::cerr << "wstawiam: ((" << map[edge].getStart() << "," << map[edge].getEnd() << ")," << map[edge].getLabel() << ")" << std::endl;
            if (map[edge].getDepth() == 0)
            {
//                if (combined_symbols)
//                    rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), map[edge].getLabelDesc()));
//                else

                    rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), map[edge].getLabel()));
                    //std::map<int, std::vector<int> >::iterator itwa = words_aligned.find(start);
                    //if (itwa != words_aligned.end())
                    //{
                    //    rule_alignments.insert(*itwa);
                    //}
                    //else
                    //{
                    //    std::vector<int> als;
                    //    als.push_back(-1);
                    //    rule_alignments.insert(std::pair<int, std::vector<int> >(start, als));
//                  //      std::cerr << "Nie znalazlem slow dopasowanych do: " << start << std::endl;
                    //}
            }
            else
            {
                std::string label;
//                if (combined_symbols)
//                    label = "<" + map[edge].getLabelDesc() + ">";
//                else
                    label = "<" + map[edge].getLabel() + ">";
                //label += "[" + boost::lexical_cast<std::string>(non_terminals) + "]";
                rule_contents.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(map[edge].getStart(), map[edge].getEnd()), label));
                non_terminals_src[start] = 1;
            }

//            std::string contents;
//            for (std::map<std::pair<int, int>, std::string>::iterator it = rule_contents.begin(); it != rule_contents.end(); it ++)
//            {
//                if (contents != "")
//                    contents += " ";
//                contents += it->second;
//            }

            Coord ks(start, end);
            std::map<Coord, Coord>::iterator itar = align_rects.find(ks);
            if (itar != align_rects.end())
            {
                int tr_start = itar->second.first;
                int tr_end = itar->second.second;

//                if (last_end >= 0)
//                {
//                    if (tr_start > last_end)
//                    {
//                        for (int x = last_end; x < tr_start; x ++)
//                        {
//                            std::string x_label = trgTokens[x];
//                            non_terminals_trg[x] = 0;
//                            Coord tr_coord(x, x + 1);
//                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
//                    std::cerr << "wsadzam tlumaczenie brakujace dla: " << map[edge].getLabel() << " - " << x_label << std::endl;
//                        }
//                        std::cerr << "tr_start = " << tr_start << " a last_end: " << last_end << std::endl;
//                    }
//                }

                std::string tr_label;
                if (map[edge].getDepth() > 0)
                {
                    tr_label = "<X>";
                    non_terminals_trg[tr_start] = 1;
                    Coord tr_coord(tr_start, tr_end);
                    rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, tr_label));
                    non_terminals_trg_mapped.insert(std::pair<int, int>(tr_start, start));
                    Coord rect_y(tr_start, tr_end);
                    RuleRect rule_rect(ks, rect_y);
                    non_rects.insert(std::pair<int, RuleRect>(start, rule_rect));
                }
                else
                {
                    //while (tr_end != tr_start)
                    while (tr_start < tr_end)
                    {
                        tr_label = trgTokens[tr_start];
                        non_terminals_trg[tr_start] = 0;
                        Coord tr_coord(tr_start, tr_start + 1);
                        rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, tr_label));
                        tr_start ++;
                    }
                }
                last_end = tr_end;
            }
            else
            {
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;
                if (map[edge].getDepth() > 0)
                {
                    std::string x_label = "<X>";
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;

                }
                else
                {
                    //std::map<int, std::vector<int> >::iterator itwa = words_aligned.find(start);
                    //if (itwa != words_aligned.end())
                    //{
                    //    for (std::vector<int>::iterator word = itwa->second.begin(); word != itwa->second.end(); word ++)
                    //    {
                    //        std::string x_label = trgTokens[*word];
                    //        non_terminals_trg[*word] = 0;
                    //        Coord tr_coord(*word, *word + 1);
                    //        rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
                    //        last_end += *word + 1;
                    //    }
                    //}
                    //else
                    //{
                    //    //w tlumaeczniu nic, jak nie ma slow dopasowanych
                    //    std::cerr << "Nie znalazlem slow dopasowanych do: " << start << std::endl;
                    //}
//                    std::cerr << "Nie ma prostokatu dla: (" << start << "," << end << ")" << "Label: " << map[edge].getLabel() << std::endl;
                }
            }

//            Coord k;
//            if (map[edge].getDepth() == 0)
//                k = Coord(r_start, r_end);
//            else
//                k = Coord(start, end);
//            std::map<Coord, Coord>::iterator itr = align_rects.find(k);
//            int trans_start, trans_end;
//            if (itr != align_rects.end())
//            {
//                trans_start = itr->second.first;
//                trans_end = itr->second.second;
//
//                if (trans_start < rect_start)
//                    rect_start = trans_start;
//                if (trans_end > rect_end)
//                    rect_end = trans_end;
////                for (int t = itr->second.first; t < itr->second.second; t ++)
////                    rule_translations[t] = trgTokens[t];
//            }
//            else
//            {
////                std::cerr << "nie znalazlem alajn rekta od (" << start << "," << end << ")" << std::endl;
//                if (map[edge].getDepth() == 0)
//                    trans_start = 0;
//                else
//                {
//                    trans_start = start;
//                    trans_end = end;
//                if (trans_start < rect_start)
//                    rect_start = trans_start;
//                if (trans_end > rect_end)
//                    rect_end = trans_end;
////                    std::cerr << "Houston, mamy problem" << std::endl;
//                }
//            }

//            if (map[edge].getDepth() > 0)
//            {
//                std::map<int, std::vector<int> >::iterator ai = words_aligned.find(start);
//                if (ai != words_aligned.end())
//                {
//                   //rule_translations[ai->second.front()] = "<X>[" + boost::lexical_cast<std::string>(non_terminals) + "]";
//                   rule_translations[ai->second.front()] = "<X>";
//                }
//                //rule_translations[trans_start] = "<X>";
//                if (trans_end - trans_start > 1)
//                for (int t = trans_start + 1; t < trans_end; t ++)
//                    rule_translations[t] = "";
//
//                non_terminals_trg_mapped[trans_start] = non_terminals;
//                non_terminals_trg[trans_start] = 1;
//                non_terminals ++;
//            }
//            else
//            {
//                std::map<int, std::vector<int> >::iterator ai = words_aligned.find(start);
//                if (ai != words_aligned.end())
//                {
//                    std::vector<int> alis;
//                    for (std::vector<int>::iterator it = ai->second.begin(); it != ai->second.end(); it ++)
//                    {
////                        std::cerr << "bach: (" << ai->first << "," << *it << ")" << std::endl;
//                        rule_translations[*it] = trgTokens[*it];
//                        alis.push_back(*it - trans_start);
//                    }
//                    rule_alignments.insert(std::pair<int, std::vector<int> >(ai->first - start, alis));
//                }
//                else
//                {
////                    std::cerr << "nie mam!" << std::endl;
//                    std::vector<int> alis;
//                    alis.push_back(-1);
//                    rule_alignments.insert(std::pair<int, std::vector<int> >(start - r_start, alis));
//                }
//            }
        }

        //for (int ii = i; ii < (i + length); ii ++)
        //{
        //}
//        if (found)
//        {
//            if (map[edge].getDepth() > 0)
//            {
//            }
//        }

//        std::cerr << "length: " << length << std::endl;
        i += length;
    }

    trans_low = -1;
    trans_high = -1;
    if (root != "")
    {
        this->setRoot(root);
        root_start = r_start;
        root_end = r_end;
        root_depth = max;

        Coord r_coord(r_start, r_end);
        std::map<Coord, Coord>::iterator arit = align_rects.find(r_coord);
        if (arit != align_rects.end())
        {
            trans_low = arit->second.first;
            trans_high = arit->second.second;
            Coord r_y(trans_low, trans_high);
            root_rect.first = r_coord;
            root_rect.second = r_y;
//            std::cerr << "takie ograniczenia: od " << arit->second.first << " do " << arit->second.second << std::endl;
        }
        else
        {
//            std::cerr << "nie ma prostokatu dla takiej reguly" << std::endl;
        }

    }

    std::string xxtr = "";
    int last = trans_low;

    for (RuleElems::iterator it = rule_translations.begin(); it != rule_translations.end(); it ++)
    {
        if (last >= 0)
        {
            if (it->first.first > last)
            {
                        for (int x = last; x < it->first.first; x ++)
                        {
                            std::string x_label = trgTokens[x];
                            non_terminals_trg[x] = 0;
                            Coord tr_coord(x, x + 1);
                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
//                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
                        }
            }
        }
        last = it->first.second;
    }
    if (last < trans_high)
    {
        for (int x = last; x < trans_high; x ++)
        {
            std::string x_label = trgTokens[x];
            non_terminals_trg[x] = 0;
            Coord tr_coord(x, x + 1);
            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
            //                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
        }
    }

//    std::string xxtr = "";
//    int last = -1;
//
//    for (RuleElems::iterator it = rule_translations.begin(); it != rule_translations.end(); it ++)
//    {
//        if (last >= 0)
//        {
//            if (it->first.first > last)
//            {
//                        for (int x = last; x < it->first.first; x ++)
//                        {
//                            std::string x_label = trgTokens[x];
//                            non_terminals_trg[x] = 0;
//                            Coord tr_coord(x, x + 1);
//                            rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
////                    std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
//                        }
//            }
//        }
//        last = it->first.second;
//    }

//    Coord ks(r_start, r_end);
//    std::map<Coord, Coord>::iterator itar = align_rects.find(ks);
//    if (itar != align_rects.end())
//    {
//        int r_tr_start = itar->second.first;
//        int r_tr_end = itar->second.second;
//        if (rule_translations.rbegin()->first.second < r_tr_end)
//        {
//            std::cerr << "Wykrylem zakrotkosc w: " << root << "; powinna siegac do: " << r_tr_end << ", a sie konczy na:"  << rule_translations.rbegin()->first.second << std::endl;
//            for (int x = rule_translations.rbegin()->first.second; x < r_tr_end; x ++)
//            {
//                std::string x_label = trgTokens[x];
//                non_terminals_trg[x] = 0;
//                Coord tr_coord(x, x + 1);
//                rule_translations.insert(std::pair<std::pair<int, int>, std::string>(tr_coord, x_label));
////                std::cerr << "wsadzam tlumaczenie brakujace - " << x_label << std::endl;
//            }
//        }
//    }
//    else
//    {
//        std::cerr << "Nie znalazlem prostokata dla calej reguly!" << std::endl;
//    }

    int trg_b = rule_translations.begin()->first.first;
    int trg_e = rule_translations.rbegin()->first.second;

    for (int x = trg_b; x < trg_e; x ++)
    {
        trg_toks.insert(std::pair<int, std::string>(x, trgTokens[x]));
    }

//    if (root != "")
//    {
//        this->setRoot(root);
//        root_start = r_start;
//        root_end = r_end;
//        root_depth = max;
//
//        Coord r_coord(r_start, r_end);
//        std::map<Coord, Coord>::iterator arit = align_rects.find(r_coord);
//        if (arit != align_rects.end())
//        {
//            trans_low = arit->second.first;
//            trans_high = arti->second.second;
//            //std::cerr << "takie ograniczenia: od " << arit->second.first << " do " << arit->second.second << std::endl;
//        }
//        else
//        {
//            std::cerr << "nie ma prostokatu dla takiej reguly" << std::endl;
//        }
//
//
////        Coord k = Coord(root_start, root_end);
////        std::map<Coord, Coord>::iterator itr = align_rects.find(k);
////        int trans_start, trans_end;
////        if (itr != align_rects.end())
////        {
////            trans_start = itr->second.first;
////            trans_end = itr->second.second;
//////        for (int t = 0; t < trans_start; t ++)
//////            rule_translations[t] = "";
//////        for (int t = trans_end; t < rule_translations.size(); t ++)
//////            rule_translations[t] = "";
////        }
//    }

    int max_depth = 0;
    for (std::map<ParseGraph::Edge, int>::iterator di = depths.begin(); di != depths.end(); di++)
    {
        if (di->second > max_depth)
            max_depth = di->second;
    }
    depth = max_depth;

//    std::cerr << "Regula: " << root_label << " -->" << std::endl;
//    for (RuleElems::iterator hu = rule_contents.begin(); hu != rule_contents.end(); hu ++)
//        std::cerr << "(" << hu->first.first << "," << hu->first.second << ") " << hu->second << std::endl;
//    std::cerr << "tlumaczenie:" << std::endl;
//    for (RuleElems::iterator hu = rule_translations.begin(); hu != rule_translations.end(); hu ++)
//        std::cerr << "(" << hu->first.first << "," << hu->first.second << ") " << hu->second << std::endl;
//    std::cerr << "Dopasowania: " << std::endl;
//    for (std::map<int, std::vector<int> >::iterator hu = rule_alignments.begin(); hu != rule_alignments.end();  hu ++)
//    {
//        std::cerr << hu->first << " =>";
//        std::vector<int> pic = hu->second;
//        for (std::vector<int>::iterator wac = pic.begin(); wac != pic.end(); wac ++)
//            std::cerr << " " << *wac;
//        std::cerr << std::endl;
//    }

    composition_level = 1;

}

void RuleGraph::copy(RuleGraph rg)
{
//    std::cerr << "weszlem" << std::endl;
    graph = ParseGraphPtr(new ParseGraph(*(rg.getGraph())));
//    std::cerr << "jest graf" << std::endl;
    combined_symbols = rg.combined_symbols;
    root_label = rg.getRoot();
    composition_level = rg.getCompositionLevel();

    root_start = rg.root_start;
    root_end = rg.root_end;
    root_depth = rg.root_depth;

    root_sig = rg.root_sig;

    root_rect = rg.root_rect;

//    free_leaves.clear();

//    ParseGraph::Graph *g = rg.getGraph()->getBoostGraph();
//    ParseGraph::Vertex v = vertex(0, *g);
//    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
//    ParseGraph::TransitionMap map = get(edge_bundle, *g);
//    int i = 0;
//    while (v != end)
//    {
//        v = vertex(i, *g);
//        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
//        {
//            ParseGraph::Edge e = *p.first;
//            this->graph->add_edge(map[e].getStart(), map[e].getEnd(), map[e]);
//        }
//        i ++;
//    }

//    for (std::multimap<std::string, ParseGraph::Edge>::iterator it = rg.free_leaves.begin(); it != rg.free_leaves.end(); it ++)
//        free_leaves.insert(*it);

    leaves = rg.getLeaves();
//    std::cerr << "sa pierdoly" << std::endl;

    leaves_list.insert(leaves_list.begin(), rg.leaves_list.begin(), rg.leaves_list.end());
//    std::cerr << "sa liscie" << std::endl;
    //for (std::vector<ParseGraph::Edge>::iterator it = rg.leaves_list.begin(); it != rg.leaves_list.end(); it ++)
    //    leaves_list.push_back(*it);

    depths.insert(rg.depths.begin(), rg.depths.end());
    //for (std::map<ParseGraph::Edge, int>::iterator di = rg.depths.begin(); di != rg.depths.end(); di ++)
    //    depths.insert(std::pair<ParseGraph::Edge, int>(di->first, di->second));
    depth = rg.getDepth();
//    std::cerr << "sa glebokosci" << std::endl;

//    for (std::vector<std::string>::iterator it = rg.rule_contents.begin(); it != rg.rule_contents.end(); it ++)
//        rule_contents.push_back(*it);
    rule_contents.insert(rg.rule_contents.begin(), rg.rule_contents.end());
//    std::cerr << "jest zwartosc" << std::endl;
    //for (std::map<std::pair<int, int>, std::string>::iterator it = rg.rule_contents.begin(); it != rg.rule_contents.end(); it ++)
    //    rule_contents.insert(*it);
    rule_translations.insert(rg.rule_translations.begin(), rg.rule_translations.end());
//    std::cerr << "jest tlumaczenie" << std::endl;
    //for (std::vector<std::string>::iterator it = rg.rule_translations.begin(); it != rg.rule_translations.end(); it ++)
    //    rule_translations.push_back(*it);
//    for (std::map<std::pair<int, int>, std::string>::iterator it = rg.rule_translations.begin(); it != rg.rule_translations.end(); it ++)
//        rule_translations.insert(*it);
    rule_alignments.insert(rg.rule_alignments.begin(), rg.rule_alignments.end());
//    std::cerr << "jest dopasowanie" << std::endl;
    //for (std::map<int, std::vector<int> >::iterator it = rg.rule_alignments.begin(); it != rg.rule_alignments.end(); it ++)
    //    rule_alignments.insert(*it);

    trg_toks.insert(rg.trg_toks.begin(), rg.trg_toks.end());
//    std::cerr << "sa trg_toks" << std::endl;

    non_terminals_trg_mapped.insert(rg.non_terminals_trg_mapped.begin(), rg.non_terminals_trg_mapped.end());
    //for (std::vector<int>::iterator it = rg.non_terminals_trg_mapped.begin(); it != rg.non_terminals_trg_mapped.end(); it ++)
    //    non_terminals_trg_mapped.push_back(*it);
    non_terminals_src = rg.non_terminals_src;
    non_terminals_trg = rg.non_terminals_trg;

    non_rects.insert(rg.non_rects.begin(), rg.non_rects.end());

    rect_start = rg.rect_start;
    rect_end = rg.rect_end;

    size = rg.getSize();
    set_history = rg.set_history;
////    set_history.resize(size);
//    set_history.resize(size);
//    std::cerr << "size; " << size << std::endl;
//    std::cerr << "rili: " << set_history.size() << std::endl;
//    set_history.reset();
//    boost::dynamic_bitset<>::size_type x = rg.set_history.find_first();
//    while (x != rg.set_history.npos)
//    {
//        set_history[x] = 1;
//        x = rg.set_history.find_next(x);
//    }
    set_open = rg.set_open;
//    std::cerr << "i po gupotach" << std::endl;
//    set_open.resize(size);
//    //set_open.resize(666);
//    set_open.reset();
//    x = rg.set_open.find_first();
//    while (x != rg.set_open.npos)
//    {
//        set_open[x] = 1;
//        x = rg.set_open.find_next(x);
//    }
}

bool RuleGraph::fits(ParseGraph::Edge edge)
{
    ParseGraph::Graph *g = graph->getBoostGraph();
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
 //   if (map[edge].getStart() >= boost::num_vertices(*g))
 //       return false;
//    std::cerr << "Sprawdzam: " << map[edge].getStart() << " z " << root_start << std::endl
//        << map[edge].getEnd() << " z " << root_end << std::endl
//        << map[edge].getDepth() << " z " << root_depth << std::endl
//        << map[edge].getLabel() << " z " << root_label << std::endl;
    return ((map[edge].getStart() == root_start) && (map[edge].getEnd() == root_end) && (map[edge].getDepth() == root_depth) && (map[edge].getLabel() == root_label));
//    ParseGraph::Vertex v = vertex(map[edge].getStart(), *g);
//    bool ret = false;
//    for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
//    {
//        ParseGraph::Edge e = *p.first;
//        if (map[e].getStart() == map[edge].getStart() && map[e].getEnd() == map[edge].getEnd() && map[e].getDepth() == map[edge].getDepth() && map[e].getLabel() == map[edge].getLabel())
//        {
//            ret = true;
//            break;
//        }
//
//    }
//    return ret;
}

bool RuleGraph::differentLeaves(RuleGraph rg)
{
//    if (this->leaves_list.size() != rg.leaves_list.size())
//        return true;
//
    ParseGraph::TransitionMap map_a = get(boost::edge_bundle, *(this->graph->getBoostGraph()));
    ParseGraph::TransitionMap map_b = get(boost::edge_bundle, *(rg.getGraph()->getBoostGraph()));
    
    bool diff = false;
    std::vector<ParseGraph::Edge>::iterator it_a = this->leaves_list.begin();
    std::vector<ParseGraph::Edge>::iterator it_b = rg.leaves_list.begin();
    while (it_a != this->leaves_list.end())
    {
        if (map_a[*it_a].getLabel() != map_b[*it_b].getLabel())
        {
            diff = true;
            break;
        }
        if (map_a[*it_a].getStart() != map_b[*it_b].getStart())
        {
            diff = true;
            break;
        }
        if (map_a[*it_a].getDepth() != map_b[*it_b].getDepth())
        {
            diff = true;
            break;
        }
        if (map_a[*it_a].getEnd() != map_b[*it_b].getEnd())
        {
            diff = true;
            break;
        }
        it_a ++;
        it_b ++;
    }
//    if (diff)
//        std::cerr << "Rozne liwsy zwracaja true" << std::endl;
//    else
//        std::cerr << "Rozne liwsy zwracaja false" << std::endl;
    return diff;
}

//std::string RuleGraph::getRule(std::vector<std::string> trgTokens, std::map<Coord, Coord> align_rects, std::map<int, std::vector<int> > words_aligned, bool include_k)
//{
//    std::vector<std::string> translation;
//    for (std::vector<std::string>::iterator it = trgTokens.begin(); it != trgTokens.end(); it ++)
//        translation.push_back(*it);
//
//    std::stringstream ss;
//    ss << "<" << root_label << ">";
//    ss << " -->";
//    ParseGraph::Graph *g = graph->getBoostGraph();
//    ParseGraph::Vertex v = vertex(0, *g);
//    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
//    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
//    int trans_start = -1;
//    int trans_count = trgTokens.size();
//    Coord k;
//    k.first = root_start;
//    k.second = root_end;
//    std::map<Coord, Coord>::iterator itr = align_rects.find(k);
//    if (itr != align_rects.end())
//    {
//        trans_start = itr->second.first;
//        trans_count = itr->second.second - trans_start;
//    }
//    int i = 0;
//    int y = 0;
//    int non_terminals = 0;
//    int deleted = 0;
//    std::vector<int> del(trgTokens.size(), 0);
//    std::string alignments = "";
//
//    while (v != end)
//    {
//        v = vertex(i, *g);
//        int min = 1000;
//        bool found = false;
//        ParseGraph::Edge edge;
//        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
//        {
//            ParseGraph::Edge e = *p.first;
//            if (map[e].getDepth() < min)
//            {
//                min = map[e].getDepth();
//                found = true;
//                edge = e;
//            }
//        }
//        int length = 1;
//        if (found)
//        {
//            ss << " ";
//            if (map[edge].getDepth() > 0)
//            {
//                ss << "<" << map[edge].getLabel() << ">" << "[" << boost::lexical_cast<std::string>(non_terminals) << "]";
//                Coord k;
//                k.first = map[edge].getStart();
//                k.second = map[edge].getEnd();
//                std::map<Coord, Coord>::iterator it = align_rects.find(k);
//                //            while (it == align_rects.end())
//                //            {
//                //                y ++;
//                //                if (y >= trgTokens.size())
//                //                    break;
//                //                it = align_rects.find(k);
//                //            }
//                if (it != align_rects.end())
//                {
//                    //int xs = it->first.first;
//                    int ys = it->second.first;
//                    //int xd = it->second.first - xs;
//                    int yd = it->second.second - ys;
//                    //ys -= deleted;
//                    int real_ys = ys;
//
//                    for (int i = 0; i < ys; i ++)
//                        if (del[i])
//                            real_ys -= del[i];
////                    std::cerr << "Usuwam od: " << real_ys << " do: " << (ys + yd) << std::endl;
//                    std::vector<std::string>::iterator yi = translation.begin() + real_ys;
//                    std::vector<std::string>::iterator ynew = translation.erase(yi, yi + yd);
//                    if (yd > 1)
//                    {
//                        for (int i = ys; i < (ys + yd - 1); i ++)
//                            del[i] ++;
//                        deleted += (yd - 1);
//                    }
//                    //                if (yi >= trgTokens.size())
//                    //                    trgTokens.push_back("<X>[" + boost::lexical_cast<std::string(non_terminals - 1) + "]");
//                    translation.insert(ynew, "<X>[" + boost::lexical_cast<std::string>(non_terminals) + "]");
//                }
////L                else
////                {
////                    std::cerr << "Nie znalazlem prostokata dopasowujacego sie od: " << k.first << " do: " << k.second << " na osi s." << std::endl;
////                }
//                non_terminals ++;
//            }
//            else
//            {
//                ss << map[edge].getLabel();
//                int i = map[edge].getStart() - root_start;
//                std::map<int, std::vector<int> >::iterator ai = words_aligned.find(map[edge].getStart());
//                if (ai != words_aligned.end())
//                {
//                    for (std::vector<int>::iterator it = ai->second.begin(); it != ai->second.end(); it ++)
//                    {
//                        if (alignments != "")
//                            alignments += ",";
//                        alignments += "[";
//                        for (int k = 0; k < map[edge].getStart(); k ++)
//                            if (del[k])
//                                i -= del[k];
//                        alignments += boost::lexical_cast<std::string>(i + 1);
//                        alignments += ",";
//                        int j = *it - trans_start;
//                        for (int k = 0; k < *it; k ++)
//                            if (del[k])
//                                j -= del[k];
//                        alignments += boost::lexical_cast<std::string>(j + 1);
//                        alignments += "]";
//                    }
//                }
//                else
//                {
//                    if (alignments != "")
//                        alignments += ",";
//                    alignments += "[";
//                    alignments += boost::lexical_cast<std::string>(i + 1);
//                    alignments += ",0]";
//
////L                    std::cerr << "Nie znalazlem dopasowan dla " << (i + 1) << "-tego tokenu." << std::endl;
//                }
//
//            }
//            length = map[edge].getEnd() - map[edge].getStart();
//        }
//        i += length;
//        if (i > root_end)
//            break;
//    }
////    std::cerr << "po petli" << std::endl;
////    std::cerr << "trans_start: " << trans_start << std::endl;
////    std::cerr << "trans_count: " << trans_count << std::endl;
////    std::cerr << "deleted: " << deleted << std::endl;
////    for (std::vector<std::string>::iterator it = translation.begin(); it != translation.end(); it ++)
////        std::cerr << *it << std::endl;
//    
//
////    std::cerr << "Tyle mam przed krachem: " << ss.str() << std::endl;
////    std::cerr << "translations.size(): " << translation.size() << std::endl;
////    std::cerr << "trans_start: " << trans_start << std::endl << "trans_count: " << trans_count << std::endl << "deleted: " << deleted << std::endl;
//
//    ss << " ::";
//    //for (std::vector<std::string>::iterator it = translation.begin() + trans_start; it != translation.end(); it ++)
//    for (std::vector<std::string>::iterator it = (translation.begin() + trans_start); it != (translation.begin() + trans_start + trans_count - deleted); it ++)
//        ss << " " << *it;
//
//    ss << "\t" << "[" << alignments << "]";
//
//    if (include_k)
//        ss << "\t" << this->getCompositionLevel();
//
//    return ss.str();
//}

std::string RuleGraph::getRule(bool include_rects, bool include_k)
{
    std::stringstream ss;

    ss << "<" << root_label << ">";
    ss << " --> ";

    std::string contents;
    std::map<int, int> nonterminal_numbers;
    std::map<int, int> elem_numbers;
    int non_terminals = 0;
    int elems = 0;
    for (RuleElems::iterator it = rule_contents.begin(); it != rule_contents.end(); it ++)
    {
        if (contents != "")
            contents += " ";
        contents += it->second;
        if (non_terminals_src[it->first.first])
        {
            contents += "[" + boost::lexical_cast<std::string>(non_terminals) + "]";
            nonterminal_numbers.insert(std::pair<int, int>(it->first.first, non_terminals));
            non_terminals ++;
        }
        elem_numbers.insert(std::pair<int, int>(it->first.first, elems)); // w sumie to mozna dorzucac tylko dla terminali, zeby nie robic za duzo tych operacji dodawania, zawsze pol sekundy sie oszczedzi
        elems ++;
    }
    ss << contents;
//    std::cerr << "Kontents: " << contents << std::endl;

    ss << " :: ";
//    ss << "translation todo";

    std::string translation = "";
    std::map<int, int> trg_elems;
    int tri = 0;
    for (RuleElems::iterator it = rule_translations.begin(); it != rule_translations.end(); it ++)
    {
        //if (*it != "")
        //{
        

            if (translation != "")
                translation += " ";
            translation += it->second;
            if (non_terminals_trg_mapped.find(it->first.first) != non_terminals_trg_mapped.end())
            {
                translation += "[" + boost::lexical_cast<std::string>(nonterminal_numbers[non_terminals_trg_mapped[it->first.first]]) + "]";
            }
//            translation += "(";
//            translation += boost::lexical_cast<std::string>(it->first.first);
//            translation += ",";
//            translation += boost::lexical_cast<std::string>(it->first.second);
//            translation += ")";

            //if (non_terminals_trg[it->first.first])
            //    translation += "[" + boost::lexical_cast<std::string>(non_terminals_trg_mapped[tri]) + "]";
        //}
        trg_elems.insert(std::pair<int, int>(it->first.first, tri));
        tri ++;
    }
    ss << translation;

    if (include_alignments)
    {

    std::string alignments = "";
    for (std::map<int, std::vector<int> >::iterator it = rule_alignments.begin(); it != rule_alignments.end(); it ++)
    {
        std::vector<int> alis = it->second;
        for (std::vector<int>::iterator ali = alis.begin(); ali != alis.end(); ali ++)
        {
            if (*ali > -1)
            {
                std::map<int, int>::iterator map_it = trg_elems.find(*ali);
                if (map_it != trg_elems.end())
                {
                    if (alignments != "")
                        alignments += ",";
                    alignments += "[" + boost::lexical_cast<std::string>(elem_numbers[it->first] + 1);

                    alignments += "," + boost::lexical_cast<std::string>(map_it->second + 1) + "]";
                    //alignments += "," + boost::lexical_cast<std::string>(trg_elems[*ali] + 1) + "]";
                }
            }
            else
            {
                if (alignments != "")
                    alignments += ",";
                alignments += "[" + boost::lexical_cast<std::string>(elem_numbers[it->first] + 1);
                alignments += ",0]";
            }
        }

    }
    
    ss << "\t" << "[" << alignments << "]";

    }

    if (include_rects)
    {
        ss << "\t";
        ss << "(" << root_rect.first.first << "," << root_rect.first.second << "," << root_rect.second.first << "," << root_rect.second.second << ")";
        for (std::map<int, RuleRect>::iterator xx = non_rects.begin(); xx != non_rects.end(); xx ++)
        {
            ss << ",";
            ss << "(" << xx->second.first.first << "," << xx->second.first.second << "," << xx->second.second.first << "," << xx->second.second.second << ")";
        }
    }

    if (include_k)
        ss << "\t" << composition_level;

    return ss.str();
}

int RuleGraph::getSize()
{
    return size;
}

void RuleGraph::setH(int h)
{
    if (set_history.size() <= h)
    {
        set_history.resize(h + 1);
        set_open.resize(h + 1);
    }
//    std::cerr << "ustawiam: " << h << ", a rozmiar: " << set_history.size() << std::endl;
    set_history[h] = 1;
    root_sig = h;
}

void RuleGraph::setSize(int aSize)
{
    size = aSize;
    set_history.resize(aSize);
    set_open.resize(aSize);
    set_history.reset();
    set_open.reset();
}

}

}

