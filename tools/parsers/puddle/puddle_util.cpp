
#include "puddle_util.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            namespace util {

                /**
                 * escapes special characters from string `s`
                 * original string remains unchanged
                 */
                std::string escapeSpecialChars(std::string &s) {
                    std::string r = s;
                    return r;
                }

                /**
                 * unescapes special characters from string `s`
                 * original string remains unchanged
                 */
                std::string unescapeSpecialChars(std::string &s) {
                    std::string r = s;
                    size_t pos = 0;

                    //left parenthesis
                    pos = r.find("&#40;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "(" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#40;", pos + 1);
                    }
                    //right parenthesis
                    pos = r.find("&#41;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + ")" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#41;", pos + 1);
                    }
                    //asterisk
                    pos = r.find("&#42;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "*" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#42;", pos + 1);
                    }
                    //plus sign
                    pos = r.find("&#43;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "+" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#43;", pos + 1);
                    }
                    //ampersand
                    pos = r.find("&amp;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "&" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&amp;", pos + 1);
                    }
                    //less-than sign
                    pos = r.find("&lt;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "<" + r.substr(pos + 4, std::string::npos);
                        pos = r.find("&lt;", pos + 1);
                    }
                    //greater-than sign
                    pos = r.find("&gt;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + ">" + r.substr(pos + 4, std::string::npos);
                        pos = r.find("&gt;", pos + 1);
                    }
                    //question mark
                    pos = r.find("&#63;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "?" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#63;", pos + 1);
                    }
                    //vertical bar
                    pos = r.find("&#124;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "|" + r.substr(pos + 6, std::string::npos);
                        pos = r.find("&#124;", pos + 1);
                    }

                    return r;
                }

//                /**
//                 * returns a pointer to the edge from the `index`-th vertex of graph `pg`
//                 * with the highest 'depth' //@todo: jak to sie w kracie nazywac bedzie
//                 * note: index numbering starts from 0
//                 */
//                TransitionInfo* getEdge(ParseGraphPtr pg, int index) {
//                    return getEdge(pg, index, 0);
//                }
//
//                /**
//                 * returns a pointer to the edge with the highest 'depth' from
//                 * the vertex at position `offset`starting from position `index`
//                 * in the graph `pg`
//                 * note: numbering starts from 0
//                 */
//                TransitionInfo* getEdge(ParseGraphPtr pg, int index, int offset) {
//                    ParseGraph::Graph *g = pg->getBoostGraph();
//                    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
//
//                    int stop_i = index + offset;
//                    if ( stop_i >= (boost::num_vertices(*g) - 1) ) { //the last vertex of parse graph is only the 'in-vertex', so it is not taken into consideration
//                        //throw wyjatek
//                        return NULL;
//                    }
//
//                    int i = 0;
//                    TransitionInfo *ti = NULL;
//                    while (i <= stop_i) {
//                        ParseGraph::Vertex v = boost::vertex(i, *g);
//                        int max_depth = 0;
//                        int max_end = i + 1; //initial value in case of no edge found, just skip to the next vertex
//                        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) {
//                            ParseGraph::Edge e = *p.first;
//                            if ((max_depth == 0) || ((max_depth > 0) && (map[e].getDepth() > max_depth))) {
//                                max_depth = map[e].getDepth();
//                                max_end = map[e].getEnd();
//                                if (i == stop_i) {
//                                    ti = &(map[e]);
//                                }
//                            }
//                        }
//                        i = max_end;
//                    }
//                    return ti;
//                }
//
//                //@todo: tu trzeba jakies wyjatki sprawdzac/czy tam failbit, badbit!
//                std::string getNewEdgeId(ParseGraphPtr pg) {
//                    ParseGraph::Graph *g = pg->getBoostGraph();
//                    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
//
//                    int max_id_int = -1;
//                    if (boost::num_vertices(*g) > 0) {
//                        for (int i = 0; i < (boost::num_vertices(*g) - 1); i ++) {
//                            ParseGraph::Vertex v = boost::vertex(i, *g);
//                            for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) {
//                                ParseGraph::Edge e = *p.first;
//                                std::stringstream ss;
//                                ss << std::hex << map[e].getId();
//                                int id_int;
//                                ss >> id_int;
//                                if (id_int > max_id_int)
//                                    max_id_int = id_int;
//                            }
//                        }
//                    }
//                    int new_id_int = max_id_int + 1;
//                    std::stringstream ss;
//                    ss << std::hex << new_id_int;
//                    return ss.str();
//                }
//
//                void removeGraphEdge(ParseGraphPtr pg, TransitionInfo edge) {
//                    ParseGraph::Graph *g = pg->getBoostGraph();
//                    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
//
//                    ParseGraph::Vertex v = boost::vertex(edge.getStart(), *g);
//                    for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) {
//                        ParseGraph::Edge e = *p.first;
//                        if (map[e].getEnd() == edge.getEnd() && map[e].getDepth() == edge.getDepth()
//                                && map[e].getType() == edge.getType() ) {
//                            boost::remove_edge(e, *g);
//                        }
//                    }
//                }

                std::string getMorphologyString(Morphology morphology) {
                    std::string s = "";
                    std::string partOfSpeech = morphology["pos"];
                    s = partOfSpeech;
                    for (Morphology::iterator morphoIt = morphology.begin();
                            morphoIt != morphology.end(); ++ morphoIt) {
                        std::string attribute = morphoIt->first;
                        std::string value = morphoIt->second;
                        if (attribute == "pos")
                            continue;
                        if (s != "")
                            s += ":";
                        s += value;
                    }
                    return s;
                }

            }
        }

    }

}

