#ifndef SORTED_ADJACENCY_H__
#define SORTED_ADJACENCY_H__

#include <boost/graph/adjacency_list.hpp>
#include "TransitionInfo.hpp"

using namespace poleng::bonsai;

struct multiset_by_depthS { };

template <class Edge>
struct sort_by_depth : public std::binary_function<Edge,Edge,bool> {
    bool operator()(const Edge& e1, const Edge& e2) {
	TransitionInfo ts1 = get(boost::edge_bundle, e1);
	TransitionInfo ts2 = get(boost::edge_bundle, e2);
	return ts1.getDepth() > ts2.getDepth();
    }
};
				
namespace boost {
    template <class ValueType>
    struct container_gen<multiset_by_depthS, ValueType> {
        typedef std::multiset<ValueType, sort_by_depth<ValueType> > type;
    };
						    
    template <>
    struct parallel_edge_traits<multiset_by_depthS> {
        typedef allow_parallel_edge_tag type;
    };
}

#endif
