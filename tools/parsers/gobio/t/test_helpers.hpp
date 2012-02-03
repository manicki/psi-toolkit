#ifndef TEST_HELPERS_HDR
#define TEST_HELPERS_HDR


#include <queue>
#include <string>
#include <fstream>


template<class C,class S,class V,class R>
int count_vertices(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator it = ch.vertices();
    int vertex_count = 0;
    while (it.hasNext()) {
        it.next();
        ++vertex_count;
    }
    return vertex_count;
}


template<class C,class S,class V,class R>
int count_out_edges(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator vertex_it = ch.vertices();
    int edge_count = 0;
    while (vertex_it.hasNext()) {
        typename chart<C,S,V,R>::out_edge_iterator it = ch.out_edges(vertex_it.next());
        while (it.hasNext()) {
            it.next();
            ++edge_count;
        }
    }
    return edge_count;
}

/*
template<class C,class S,class V,class R>
int count_in_edges(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator vertex_it = ch.vertices().first;
    typename chart<C,S,V,R>::vertex_iterator vertex_end = ch.vertices().second;

    int edge_count = 0;

    while (vertex_it != vertex_end)
    {
    typename chart<C,S,V,R>::in_edge_iterator it =
        ch.in_edges(*vertex_it).first;
    typename chart<C,S,V,R>::in_edge_iterator end =
        ch.in_edges(*vertex_it).second;

    while (it != end)
    {
        ++edge_count;
        ++it;
    }

    ++vertex_it;
    }

    return edge_count;
}


template<class C,class S,class V,class R>
int count_marked_out_edges(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator vertex_it = ch.vertices().first;
    typename chart<C,S,V,R>::vertex_iterator vertex_end = ch.vertices().second;

    int edge_count = 0;

    while (vertex_it != vertex_end)
    {
    typename chart<C,S,V,R>::marked_edges_index_type::marked_out_edge_iterator it =
        ch.marked_edges_index(*vertex_it).marked_out_edges().first;
    typename chart<C,S,V,R>::marked_edges_index_type::marked_out_edge_iterator end =
        ch.marked_edges_index(*vertex_it).marked_out_edges().second;

    while (it != end)
    {
        ++edge_count;
        ++it;
    }

    ++vertex_it;
    }

    return edge_count;
}


template<class C,class S,class V,class R>
int count_marked_in_edges(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator vertex_it = ch.vertices().first;
    typename chart<C,S,V,R>::vertex_iterator vertex_end = ch.vertices().second;

    int edge_count = 0;

    while (vertex_it != vertex_end)
    {
    typename chart<C,S,V,R>::marked_edges_index_type::marked_in_edge_iterator it =
        ch.marked_edges_index(*vertex_it).marked_in_edges().first;
    typename chart<C,S,V,R>::marked_edges_index_type::marked_in_edge_iterator end =
        ch.marked_edges_index(*vertex_it).marked_in_edges().second;

    while (it != end)
    {
        ++edge_count;
        ++it;
    }

    ++vertex_it;
    }

    return edge_count;
}

*/
template<class C,class S,class V,class R>
bool is_vertex_consistent(chart<C,S,V,R>& ch, typename chart<C,S,V,R>::vertex_descriptor v)
{
    typename chart<C,S,V,R>::out_edge_iterator oei = ch.out_edges(v);
    while (oei.hasNext()) {

        typename chart<C,S,V,R>::edge_descriptor oe = oei.next();
        if (ch.edge_source(oe) != v) {
            return false;
        }

        typename chart<C,S,V,R>::in_edge_iterator oeiei = ch.in_edges(ch.edge_target(oe));
        int edgeCount = 0;
        while (oeiei.hasNext()) {
            if (oeiei.next() == oe) {
                ++edgeCount;
            }
        }
        if (edgeCount != 1) {
            return false;
        }

    }

    typename chart<C,S,V,R>::in_edge_iterator iei = ch.in_edges(v);
    while (iei.hasNext()) {

        typename chart<C,S,V,R>::edge_descriptor ie = iei.next();
        if (ch.edge_target(ie) != v) {
            return false;
        }

        typename chart<C,S,V,R>::out_edge_iterator ieoei = ch.out_edges(ch.edge_source(ie));
        int edgeCount = 0;
        while (ieoei.hasNext()) {
            if (ieoei.next() == ie) {
                ++edgeCount;
            }
        }
        if (edgeCount != 1) {
            return false;
        }

    }

    {
    typename chart<C,S,V,R>::marked_edges_index_type::marked_out_edge_iterator it =
        ch.marked_edges_index(v).marked_out_edges().first;
    typename chart<C,S,V,R>::marked_edges_index_type::marked_out_edge_iterator end =
        ch.marked_edges_index(v).marked_out_edges().second;

    while (it != end)
    {
        if (ch.edge_source((*it)) != v)
        return false;

        if (std::count(
           ch.marked_edges_index(ch.edge_target((*it))).marked_in_edges().first,
           ch.marked_edges_index(ch.edge_target((*it))).marked_in_edges().second,
           (*it)) != 1)
        return false;

        ++it;
    }
    }

    {
    typename chart<C,S,V,R>::marked_edges_index_type::marked_in_edge_iterator it =
        ch.marked_edges_index(v).marked_in_edges().first;
    typename chart<C,S,V,R>::marked_edges_index_type::marked_in_edge_iterator end =
        ch.marked_edges_index(v).marked_in_edges().second;

    while (it != end)
    {
        if (ch.edge_target((*it)) != v)
        return false;

        if (std::count(
           ch.marked_edges_index(ch.edge_source((*it))).marked_out_edges().first,
           ch.marked_edges_index(ch.edge_source((*it))).marked_out_edges().second,
           (*it)) != 1)
        return false;

        ++it;
    }
    }

    return true;
}


template<class C,class S,class V,class R>
bool is_consistent(chart<C,S,V,R>& ch)
{
    typename chart<C,S,V,R>::vertex_iterator vertex_it = ch.vertices();
    while (vertex_it.hasNext()) {
        if (!is_vertex_consistent(ch, (vertex_it.next()))) {
            return false;
        }
    }
    return true;
}

/*
inline std::string slurp_file(const std::string& filename)
{
    std::string line;
    std::string contents;

    std::ifstream s(filename.c_str());
    while (getline(s, line))
    {
    contents += line;
    contents += "\n";
    }

    return contents;
}
*/

#endif
