#ifndef PARSEGRAPH_H__
#define PARSEGRAPH_H__

#include <stack>
#include <vector>
#include <sstream>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "sorted_adjacency_list.hpp"
#include "TransitionInfo.hpp"
#include "TransferTypes.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/collections_save_imp.hpp>

#include <fstream>

namespace poleng
{

namespace bonsai
{

typedef std::pair<unsigned int, unsigned int> Dependency;                                                                                   
typedef std::set<Dependency> Dependencies;                                                                                   
typedef boost::shared_ptr<Dependencies> DependenciesPtr; 

typedef std::map<unsigned int, TransitionInfo> TokenSequence;
typedef boost::shared_ptr<TokenSequence> TokenSequencePtr;

class ParseGraph {
  public:   
    typedef boost::adjacency_list<multiset_by_depthS, boost::vecS, boost::bidirectionalS, boost::no_property, TransitionInfo> Graph;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::vertex_iterator VertexIt;
    typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
    typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
    typedef boost::graph_traits<Graph>::in_edge_iterator InEdgeIt;
    typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndex;
    typedef boost::property_map<Graph, boost::edge_bundle_t>::type TransitionMap;
    typedef std::vector<Edge> Path;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, std::string, boost::no_property> Tree;
    typedef boost::graph_traits<Tree>::vertex_descriptor TreeVertex;
    typedef boost::graph_traits<Tree>::edge_descriptor TreeEdge;
	    
    ParseGraph();
    ParseGraph(const ParseGraph &pg) {
        Graph og = pg.g;

        TransitionMap map = boost::get(boost::edge_bundle, og);

        std::pair<EdgeIt,EdgeIt> p = boost::edges(og);
        while(p.first != p.second) {
            boost::add_edge(source(*p.first,og), target(*p.first,og), map[*p.first], g);
            p.first++;
        }
        start = pg.start;
        end = pg.end;
    }

    ParseGraph(std::string serial) { load(serial); }

    void add_edge(int u, int v, TransitionInfo ti);
    int num_vertices();
    std::string write_graphviz();
    std::string write_tree_graphviz();
    std::string write_string();
    std::string write_constituents();
    std::string write_tokens();
    std::string write_berkeley();
    std::string write_subgraphs(int, int);

    void normalize();
    void binarize();

    DependenciesPtr reconstruct_dependencies();
    TokenSequencePtr get_token_sequence();
  
    SListPtr deepest_path();
    void list_deepest_paths();
    SListPtr deepest_subpath(Symbol lhs);
    SListPtr shallow_path();
    SListPtr shallow_subpath(Symbol lhs);
    Path shallow_subpath_edges(Edge e);
    std::set<std::string> get_subgraphs(int, int);
    std::set<std::string> get_subgraph(Edge, int, int);
    Graph* getBoostGraph();
    
    template<typename Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar & g;
        //ar & start;
        //ar & end;
    }

    std::string save()
    {
        std::stringstream ss;
        boost::archive::text_oarchive ofs(ss);
        //ofs << *(graph->getBoostGraph());
        ofs << this->g;
        return ss.str();

    }

    void save_to_file(std::string filename)
    {
        std::ofstream outfile(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
        boost::archive::text_oarchive ofs(outfile);
        ofs << this->g;
        outfile.close();
    }

    void load(std::string serializedGraph)
    {
        std::stringstream ss;
        ss << serializedGraph;
        
        //ifs >> graph;
        Graph tg;
        try {
    	    boost::archive::text_iarchive ifs(ss);
    	    ifs >> tg;
    	    
            TransitionMap map = boost::get(boost::edge_bundle, tg);
	    std::pair<EdgeIt,EdgeIt> p = boost::edges(tg);
    	    while(p.first != p.second) {
        	boost::add_edge(source(*p.first,tg), target(*p.first,tg), map[*p.first], this->g);
        	p.first++;
    	    }
    	    this->end = boost::vertex(boost::num_vertices(this->g)-1,this->g);
    	    this->start = 0;

        }
        catch(const boost::archive::archive_exception &e) {
    	    std::cerr << "During deserialization an exception was caught!" << std::endl;
        }
        
        //this->g = graph;
    }

    void load_itf(std::string serializedGraph);

    void load_from_file(std::string filename)
    {
        std::ifstream infile(filename.c_str());
        boost::archive::text_iarchive ifs(infile);
        ifs >> this->g;
        infile.close();
    }

  private:
    Symbol edge_to_symbol(Edge e);
    
    struct graph_property_writer {
        void operator()(std::ostream& out) {
          out << "graph [center=true, overlap=false, rankdir=LR, ranksep=0.5]" << std::endl;
          out << "node [shape=circle]" << std::endl;
        }
    };

    struct edge_property_writer {
        template <class Edge>
        void operator()(std::ostream& out, const Edge& e) {
            Graph g;
            TransitionMap map = get(boost::edge_bundle, g);
            out << "[label=\"" << map[e].getSymbol() << " (" << map[e].getDepth() << ")\"]";
//            out << "[label=\"" << map[e].getSymbol() << " (" << map[e].getDepth() << ")\"]" << map[e].getMorphologyString();
        }
    };    

    struct tree_vertex_property_writer {
        tree_vertex_property_writer(Tree t_) : t(t_) {};
	          
	template <class TreeVertex>
	void operator()(std::ostream& out, const TreeVertex& v) const {
			out << "[label=\"" << t[v] << "\"]";
	}
						      
	Tree t;
    };    
							        
    Graph g;
    Vertex start;
    Vertex end;
};

typedef boost::shared_ptr<ParseGraph> ParseGraphPtr;

}

}

#endif
