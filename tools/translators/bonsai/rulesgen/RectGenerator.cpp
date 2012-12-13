
#include "RectGenerator.hpp"

namespace poleng
{

namespace bonsai
{

RectCoord::RectCoord(int ff_, int fs_, int sf_, int ss_, bool type_)
{
    first.first = ff_;
    first.second = fs_;
    second.first = sf_;
    second.second = ss_;
    type = type_;
}

RectCoord::RectCoord(Coord c0, Coord c1, bool type_)
{
    first = c0;
    second = c1;
    type = type_;
}

RectGenerator::RectGenerator()
{
    alignment = new bool[1];
}

RectGenerator::RectGenerator(PhraseFunctionWordMap &pfmap_) {
    alignment = new bool[1];
    pfmap = pfmap_;
}

RectGenerator::~RectGenerator()
{
    delete [] alignment;
}

struct compare
{
    bool operator() (const TreeRange a, const TreeRange b) const
    {
        return (a.second < b.second || (a.second == b.second && a.first < b.first));
    }
};


std::vector<RectCoord> RectGenerator::generate(std::string &srcSentence, std::string &trgSentence, std::string &groups, ParseGraphPtr pg)
{
    srcTokens.clear();
    trgTokens.clear();

    rects.clear();

    boost::regex regSep("\\s+");
    boost::regex_token_iterator<std::string::iterator> uit(srcSentence.begin(), srcSentence.end(), regSep, -1);
    boost::regex_token_iterator<std::string::iterator> vit;
    while (uit != vit)
    {
        srcTokens.push_back(*uit);
//        std::cerr << *u << std::endl;
        uit ++;
    }
    boost::regex_token_iterator<std::string::iterator> wit(trgSentence.begin(), trgSentence.end(), regSep, -1);
    while (wit != vit)
    {
        trgTokens.push_back(*wit);
//        std::cerr << *w << std::endl;
        wit ++;
    }

    groups = groups.substr(1, groups.length() - 2);

    boost::regex regPairSep("\\s*\\]\\s*,\\s*\\[\\s*");
    boost::regex regPair("\\s*(\\d+)\\s*,\\s*(\\d+)\\s*");
    boost::regex_token_iterator<std::string::iterator> item(groups.begin(), groups.end(), regPairSep, -1);
    //boost::regex regGroupSep("\\s*\\]\\s*,\\s*\\[\\s*");
    //boost::regex regComma("\\s*,\\s*");
    //boost::regex regNumber("(\\d+)");
    //boost::regex_token_iterator<std::string::iterator> item(groups.begin(), groups.end(), regGroupSep, -1);

    s = srcTokens.size();
    t = trgTokens.size();
    if (alignment != NULL)
        delete [] alignment;
    alignment = new bool[s * t];
    for (int x = 0; x < (s * t); x ++)
        alignment[x] = false;

    //int srcIndex = 0;
    while (item != vit)
    {
        std::string aligned = boost::lexical_cast<std::string>(*item);
        if (aligned[0] == '[')
            aligned = aligned.substr(1, std::string::npos);
        if (aligned[aligned.length() - 1] == ']')
            aligned = aligned.substr(0, aligned.length() - 1);
//        std::string aligned = s.substr(1, s.length() - 2);
        //boost::regex_token_iterator<std::string::iterator> idx(aligned.begin(), aligned.end(), regComma, -1);
//        std::cerr << "aligned: " << aligned << std::endl;
        //while (idx != vit)
        boost::smatch container;
        if (boost::regex_match(aligned, container, regPair))
        {
            int srcIndex = boost::lexical_cast<int>(container[1]) - 1;
            int trgIndex = boost::lexical_cast<int>(container[2]) - 1;
            if (trgIndex == -1)
            {
                item ++;
                continue;
            }
            alignment[srcIndex * t + trgIndex] = true;
            //std::string ss = boost::lexical_cast<std::string>(*idx);
            //boost::smatch container;
            //if (boost::regex_match(ss, container, regNumber))
            //{
            //    int trgIndex = boost::lexical_cast<int>(container[1]);
            //    alignment[srcIndex * t + trgIndex] = true;

                  //  Coord tmp_x0(srcIndex, srcIndex + 1);
                  //  Coord tmp_x1(trgIndex, trgIndex + 1);
                  //  RectCoord tmp_rc(tmp_x0, tmp_x1, false);
                  //  rects.push_back(tmp_rc);
//                std::cerr << srcIndex << "->" << trgIndex << std::endl;
            //}
            //idx ++;
        }

//        srcIndex ++;
        item ++;
    }

    ParseGraph::Graph *g = pg->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::VertexIndex index = get(boost::vertex_index, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    int graph_vertices = boost::num_vertices(*g);

    std::set<TreeRange, compare> ranges;
    std::map<TreeRange, std::string> rangeCatMap;

    while (v != end)
    {
        for (std::pair<ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = boost::out_edges(v, *g); p.first != p.second; ++ p.first)
        {
            ParseGraph::Edge e = *p.first;

            std::pair<int, int> range = std::pair<int, int>(map[e].getStart(), map[e].getEnd() - map[e].getStart());
            ranges.insert(range);
            if(pfmap.count(map[e].getLabel())) {
                //std::cerr << "Adding: " << range.first << "," << range.second << " : "  << map[e].getLabel() << std::endl;
                rangeCatMap[std::make_pair(map[e].getStart(), map[e].getEnd())] = map[e].getLabel();
            }
        }
        v ++;
    }

//L    std::cerr << "Oto zakresy drzewne" << std::endl;
//    std::set<TreeRange, compare>::iterator rrx = ranges.begin();
//    while (rrx != ranges.end())
//    {
//        std::cerr << "(" << rrx->first << "," << rrx->second << ")" << std::endl;
//        rrx ++;
//    }

    std::set<TreeRange, compare>::iterator r = ranges.begin();
//    r = ranges.begin();
    while (r != ranges.end())
    {
//L        std::cerr << "Zakres: " << r->first << " -> " << r->second << std::endl;
        int xs = r->first;
//L        std::cerr << "xs = " << xs << std::endl;
        int xd = r->second;
//L        std::cerr << "xd = " << xd << std::endl;

        int x, y;
        bool found = false;
        bool gap = false;

        int ys, yd;

        if (xd == 1)
        {
            int real_d = 1;
            for (x = xs * t; x < (xs + 1) * t; x ++) //pierwsza kolumna, zawsze to robione!
            {
                //sprawdz "krzyz", czy cos zawadzo
                if (alignment[x])
                {
                    bool row_ok = true;
                    bool more_in_row = false;
                    //for (int z = (xs + 1); z < s; z ++)
                    for (int z = 0; z < s; z ++)
                    {
                        int crd = (x % t) + z * t;
                        if (alignment[crd])
                        {
                            if (z < xs)
                            {
                                //TODO wstepnie tu te wartosci na false
                                row_ok = false;
//L                                std::cerr << "tu skracam, bo cos przed " << std::endl;
                                break;
                            }
                            if (z > xs)
                            {
//L                                std::cerr << "dopasowalem nadmiarowo w: " << crd << std::endl;
                                more_in_row = true;
                                real_d ++;
                                if (real_d > xd)
                                {
//L                                    std::cerr << "za szeroki na ten zakres, ide stad" << std::endl;
                                    row_ok = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if (more_in_row)
                            {
                                row_ok = false;
                                break;
                            }
                        }
                    }
                    if (!row_ok)
                    {
                        found = false;
                        gap = false;
                        //continue;
                        break;
                    }

                    if (gap)
                    {
                        found = true;
                        break;
                    }
                    if (!found)
                    {
                        found = true;
                        ys = x % t;
                        yd = 1;
//L                        std::cerr << "Dopasowanie w (" << xs << "," << ys << ")" << std::endl;
                    }
                    else
                    {
//L                        std::cerr << "zwiekszam yd" << std::endl;
                        yd ++;
                    }
                }
                else
                {
                    if (found)
                    {
//L                        std::cerr << "przez gap nie ma fand" << std::endl;
                        gap = true;
                        found = false;
                        //break;
                    }
                }
            }
            if (gap && found)
            {
//L                std::cerr << "jest tu przerwa tetmajer" << std::endl;
            }
            else
            {
                if (found || gap)
                {
                    Coord x0(xs, ys);
                    Coord x1(xs + real_d, ys + yd);
                    RectCoord rc(x0, x1, true);
                    rects.push_back(rc);
//L                    std::cerr << "dodalbym se prostokat: " << std::endl
//                        << "(" << xs << "," << ys << "," << (xs + real_d) << "," << (ys + yd) << ")" << std::endl;
                }
                //dodaj bydle
            }

        }
        else
        {
            bool goodRow = true;
            int startZ = -1;
            int delta_z = 0;
            int dd = 0;
//            std::cerr << "xs mod s = " << (xs % s) << std::endl;
//            std::cerr << "xs mod t = " << (xs % t) << std::endl;
            while (dd < xd)
            {
            for (x = (xs + dd) * t; x < (xs + 1 + dd) * t; x ++)
            {
                if (alignment[x])
                {
                    startZ = x % t;
//L                                      std::cerr << "startz = " << startZ << "(dd: " << dd << ")" << std::endl;
//                    std::cerr << "startZ: " << "(" << x % t << "," << x / t << ")" << std::endl;
                    break;
                }
            }
                        if (startZ == -1)
                        {
//                            std::cerr << "tu nic nie ma: " << (xs + dd) <<  std::endl;
                            delta_z ++;
                        }
                        else
                        {
//                            std::cerr << "jest startz: " << (xs + dd) << std::endl;
                            break; //dd < xd
                        }
               dd ++;
            }
            //do
            //{

            if (startZ > -1)
            {
                bool foundSth = false;
                int maxZ = startZ;
                bool emptyRow = false;
//                bool prevEmpty = false;
//                int empty_count = 0;
                bool was_good_row = false;
                for (int z = startZ; z < t; z ++)
                {
                    emptyRow = true;
                    for (int y = 0; y < s; y ++)
                    {
                        int idx = z + (y * t);
                        if (alignment[idx])
                        {
                            emptyRow = false;
                            if ((y < xs) || (y >= (xs + xd)))
                            {
                                goodRow = false;
                                break;
                            }
                            else
                            {
                                foundSth = true;
                            }
                        }
                    }
                    if (!goodRow)
//                    {
//                        empty_count --;
                        break;
//                    }
                    else
                    {
                        if (!emptyRow)
                            was_good_row = true;
                    }
                    if (!foundSth)
                        break;
//                    if (emptyRow)
//                    {
//                        prevEmpty = true;
//                        empty_count ++;
//                    }
//                    else
//                    {
//                        prevEmpty = false;
//                        empty_count = 0;
//                    }
                    if (!emptyRow)
                        maxZ = z;
                }
//                if (prevEmpty)
//                {
//                    std::cerr << empty_count << " pustych wierszy tu mialem: " << maxZ << std::endl;
                    //maxZ -= empty_count;
//                }
//L                std::cerr << "bedzie kwadrat od " << startZ << " do " << maxZ << std::endl;
                //}
                //while (goodRow);

                bool sthElse = false;
                int delta_start = 0;
                int delta_t = 0; //przesuniecie wzgledem pierwszego dopasowania znalezionego przy wyszukiwaniu wg rosnacej pozycji na 'osi' tokenow zrodlowych; gdy znajdzie sie cos wczesniej (np. dopasowanie 'na krzyz'), prostokat powinien na 'osi' tokenow docelowych zaczac sie odpowiednio wczesniej
                for (int y = 0; y < xd; y ++)
                {
                    for (x = (xs+y)*t; x < ((startZ) + (xs+y)*t); x ++)
                    {
                        if (alignment[x])
                        {
                            if ((x % t) == (startZ - delta_z))
                            {
                                delta_start = delta_z; //obswerwuj tu
//                                std::cerr << "x nie robie sthelse: " << (x % t) << std::endl;
//                                std::cerr << "y nie robie sthelse: " << (x % s) << std::endl;
                                break;
                            }
//L                            std::cerr << "x dla sthelse: " << (x % t) << std::endl;
//                            std::cerr << "y dla sthelse: " << (x / t) << std::endl;
                            int before_matched_s = x / t; //pozycja znalezionego wczesniejszego dopasowania na 'osi' tokenow zrodlowych
                            int before_matched_t = x % t;
//                            std::cerr << "startz: " << startZ << std::endl;
                            if ((before_matched_s >= xs) && (before_matched_s < (xs + xd))) //jesli znalezione dopasowanie miesci sie w ramach zakresu wynikajacego z parsingu, to nie jest to nic zlego. patrz: zimowym chlodem -> frio invernal (dopasowanie 'na krzyz' tu wystepuje)
                            {
                                if (before_matched_t < startZ)
                                {
                                    if ((startZ - before_matched_t) > delta_t)
                                    {
                                        delta_t = startZ - before_matched_t;
                                    }
                                }
                            }
                            else
                            {
                                sthElse = true;
                                break;
                            }
                        }
                    }
                    if (sthElse)
                        break;
                    for (x = (xs+y)*t + maxZ + 1; x < (xs+y+1)*t; x ++)
                    {
                        if (alignment[x])
                        {
                            sthElse = true;
                            break;
                        }
                    }
                    if (sthElse)
                        break;
                }
                if (!sthElse)
                {
                    //if (!(maxZ == startZ && !goodRow))
                    if (!(maxZ == startZ && !was_good_row))
                    {
//L                        std::cerr << "bedzie kwadrat z tej strony od " << xs << " do " << (xs + xd) << std::endl;
//                        std::cerr << "deltaz: " << delta_z << std::endl;
//                        std::cerr << "delta_start: " << delta_start << std::endl;
                        Coord x0(xs, startZ - delta_t - delta_start); //TODO: czy te 'delty' tu sie moga kumulowac? nie bedzie jakiejs tragedii w zwiazku z tym?
                        if (x0.second < 0)
                            x0.second = 0;
                        //if (startZ == 20)
                        //    x0.second = startZ - 1;
                        Coord x1(xs + xd, maxZ + 1);
                        bool wrong = false;
                        int t_min = x0.second;
                        int t_max = x1.second;
                        for (int y = t_min; y < t_max; y ++)
                        {
                            for (x = 0; x < xs; x ++)
                            {
                                int c = x*t + y;
                                if (alignment[c])
                                {
                                    wrong = true;
                                    break;
                                }
                            }
                            for (x = xs+xd; x < s; x ++)
                            {
                                int c = x*t + y;
                                if (alignment[c])
                                {
                                    wrong = true;
                                    break;
                                }
                            }
                        }
                        if (!wrong)
                        {
                            RectCoord rc(x0, x1, true);
                            rects.push_back(rc);
                        }
                    }
                }
            }

        }

        r ++;
    }

    if(pfmap.size()) {
        //std::cerr << "extending" << std::endl;
        for(std::vector<RectCoord>::iterator it = rects.begin(); it != rects.end(); it++) {           
            TreeRange tr(it->first.first, it->second.first);    
            if(rangeCatMap.count(tr) and pfmap.count(rangeCatMap[tr])) {
                //std::cerr << tr.first << " " << tr.second << " : " << rangeCatMap[tr] << std::endl;
                std::set<std::string> fws = pfmap[rangeCatMap[tr]];
                
                int y0 = it->first.second-1;
                int y1 = it->second.second;
                
                while(y0 >= 0 && fws.count(trgTokens[y0])) {
                    bool aligned = false;
                    
                    for(int i = 0; i < s; i++) {
                        if(alignment[i * t + y0]) {
                            aligned = true;
                            break;
                        }
                    }
                    
                    if(not aligned) {
                        it->first.second = y0;
                        for(std::vector<RectCoord>::iterator it2 = rects.begin(); it2 != rects.end(); it2++) {
                            if(it2->first.second == y0 + 1 && it2->first.first <= tr.first && it2->second.first >= tr.second)
                                it2->first.second = y0;
                                
                        }
                    }
                    else
                        break;
                    
                    y0--;
                }
            }
        }
    }

    return rects;

}

}

}

