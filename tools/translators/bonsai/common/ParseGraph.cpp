#include <ParseGraph.hpp>
    
using namespace poleng::bonsai;

ParseGraph::ParseGraph() {
    start = vertex(0, g);
}

void ParseGraph::add_edge(int u, int v, TransitionInfo ti) {
    boost::add_edge(u, v, ti, g);
    end = vertex(boost::num_vertices(g)-1,g);
}

int ParseGraph::num_vertices() {
    return boost::num_vertices(g);
}

std::string ParseGraph::write_graphviz() {
    std::stringstream ss;
    boost::default_writer dw;
    graph_property_writer gw;
    edge_property_writer epw;
    boost::write_graphviz(ss, g, dw, epw, gw);
    return ss.str();
}

TokenSequencePtr ParseGraph::get_token_sequence() {
    TokenSequencePtr t( new TokenSequence() );
    
    TransitionMap map = boost::get(boost::edge_bundle, g);
    for(std::pair<EdgeIt,EdgeIt> eit = boost::edges(g); eit.first != eit.second; eit.first++) {
	Edge e = *eit.first;
	if(map[e].isLexical())
	    (*t)[map[e].getStart()+1] = map[e]; 
    }
    return t;
}

DependenciesPtr ParseGraph::reconstruct_dependencies() {
    DependenciesPtr d( new Dependencies() );

    TransitionMap map = boost::get(boost::edge_bundle, g);
    std::map<std::string, unsigned int> idmap;
    
    for(std::pair<EdgeIt,EdgeIt> eit = boost::edges(g); eit.first != eit.second; eit.first++) {
	Edge e = *eit.first;
	if(map[e].isLexical())
	    idmap[map[e].getId()] = map[e].getStart() + 1; 
    }
    
    std::vector<unsigned int> stack;
    for(int i = 0; i < num_vertices(); i++) {
        Vertex act = vertex(i, g);

        std::pair<InEdgeIt,InEdgeIt> p1 = boost::in_edges(act, g);
        while(p1.first != p1.second) {
            Edge e = *p1.first;
	    stack.pop_back();
	    p1.first++;
        }

        std::pair<OutEdgeIt,OutEdgeIt> p2 = boost::out_edges(act, g);
        while(p2.first != p2.second) {
            Edge e = *p2.first;

	    unsigned int head = idmap[map[e].getHead()];
	    if(!map[e].isLexical() and stack.size() > 0) {
		unsigned int stack_head = stack.back();
		if(stack_head != head)
		    d->insert(Dependency(stack_head, head));
            }
	    stack.push_back(head);
	
            p2.first++;
        }
    }
    return d;
}

std::string ParseGraph::write_tree_graphviz() {
    Tree tree;
    std::vector<TreeVertex> stack;

    TransitionMap map = boost::get(boost::edge_bundle, g);
    for(int i = 0; i < num_vertices(); i++) {
        Vertex act = vertex(i, g);

        std::pair<InEdgeIt,InEdgeIt> p1 = boost::in_edges(act, g);
        while(p1.first != p1.second) {
            Edge e = *p1.first;
	    stack.pop_back();
	    p1.first++;
        }

        std::pair<OutEdgeIt,OutEdgeIt> p2 = boost::out_edges(act, g);
        while(p2.first != p2.second) {
            Edge e = *p2.first;

	    TreeVertex tv = boost::add_vertex(map[e].getLabel(), tree);
	    if(stack.size() > 0) {
		boost::add_edge(stack.back(), tv, tree);
            }
	    stack.push_back(tv);
	    
            p2.first++;
        }
    }
    std::stringstream ss;
    tree_vertex_property_writer vpw(tree);
    boost::write_graphviz(ss, tree, vpw);
    return ss.str();
}

std::string ParseGraph::write_string() {
    std::stringstream ss;
    VertexIndex index = get(boost::vertex_index, g);
    TransitionMap map = get(boost::edge_bundle, g);
	
    for(int i = 0; i < num_vertices(); i++) {
        Vertex act = vertex(i, g);

        std::pair<InEdgeIt,InEdgeIt> p1 = boost::in_edges(act, g);
        while(p1.first != p1.second) {
            Edge e = *p1.first;
            if(map[e].isPhrasal() and map[e].getLabel() != "SYNTOK" and map[e].getLabel() != "NE")
                ss << "] ";
            p1.first++;
        }

        std::pair<OutEdgeIt,OutEdgeIt> p2 = boost::out_edges(act, g);
        std::list<std::string> in;
        while(p2.first != p2.second) {
            Edge e = *p2.first;
            if(map[e].isPhrasal() and map[e].getLabel() != "SYNTOK" and map[e].getLabel() != "NE") {
                ss << map[e].getLabel();
                ss << "[ ";
            }
            if(map[e].isLexical()) {
                ss << map[e].getLabel();
                ss << " ";
            }
            p2.first++;
        }
    }

    return ss.str();
}

void ParseGraph::normalize() {
    TransitionMap map = boost::get(boost::edge_bundle, g);
    for(std::pair<EdgeIt,EdgeIt> eit = boost::edges(g); eit.first != eit.second; eit.first++) {
        Edge e = *eit.first;
        if(map[e].isLexical()) {
            std::string label = map[e].getLabel();
            std::string base  = map[e].getPosInfo().at(0).get<0>();
                                   
	    UnicodeString label_u = icuConverter::fromUtf(label);
	    label_u.toLower();
	    std::string label_low = icuConverter::toUtf(label_u);

	    UnicodeString base_u = icuConverter::fromUtf(base);
	    base_u.toLower();
	    std::string base_low = icuConverter::toUtf(base_u);
	    
	    if(label != label_low && base == base_low) {
		map[e].setLabel(label_low);
	    }	    
	    
	    /*		                            
            if( label.size() > 1 && base.size() > 1 && label[0] != base[0] && label[1] == base[1] ) {
                label[0] = base[0];
	        map[e].setLabel(label);
            }
            else if( label.size() == 1 && base.size() == 1 && label != base && base != "0" ) {
                label[0] = base[0];
	        map[e].setLabel(label);
            }
	    */
        }
    }
}

/*
void ParseGraph::binarize() {
    TransitionMap map = boost::get(boost::edge_bundle, g);
    VertexIndex index = boost::get(boost::vertex_index, g);
    
    bool continuous = true;
    
    for(int i = 0; i < num_vertices()-1 && continuous; i++) {
        Vertex act = vertex(i, g);

        std::pair<OutEdgeIt,OutEdgeIt> p = boost::out_edges(act, g);
        bool step = false;
	while(p.first != p.second && !step) {
	    //std::cerr << *p.first << std::endl;
	    if(boost::target(*p.first, g) - act == 1) {
		step = true;
	    }
	    p.first++;
	}
	//std::cerr << "step: " << step << std::endl;
	if(!step)
	    continuous = false;
    }
    
    //std::cerr << continuous << std::endl;
    
    if(continuous) {
	for(std::pair<VertexIt, VertexIt> vit = boost::vertices(g); vit.first != vit.second; vit.first++) {
	    for(std::pair<OutEdgeIt,OutEdgeIt> eit = boost::out_edges(*vit.first, g); eit.first != eit.second; eit.first++) {
		Edge e = *eit.first;
		
		if(map[e].isPhrasal()) {
		    Path p = shallow_subpath_edges(e);
		    
		    //std::cerr << map[e].getLabel() << " " << e << " : " << p.size() << std::endl;
		    
		    if(p.size() > 2) {
			// Find head
			int head_idx = 0;
			for(int i = 0; i < p.size(); i++)
			    if(map[p[i]].getHead() == map[e].getHead())
				head_idx = i;
			
			Edge head_edge = p[head_idx];
			
			// Binarize from head to the left
			for(int i = head_idx-1; i >= 0 && head_idx-i < p.size()-1; i--) {			    
			    int start = map[p[i]].getStart();
			    int end   = map[head_edge].getEnd();
			    
			    std::string new_label = map[e].getLabel();
			    //std::string new_label = map[e].getLabel() + "_B";
			    
			    TransitionInfo ti;
			    ti.setType("group");
			    ti.setLabel(new_label);
			    ti.setStart(start);
			    ti.setEnd(end);
			    ti.setHead(map[e].getHead());
			    
			    ti.setDepth(map[p[i]].getDepth() + 1);
			    
			    std::set<Edge> temp;
			    std::pair<OutEdgeIt,OutEdgeIt> op = boost::out_edges(boost::source(p[i], g), g);
			    while(op.first != op.second) {
				if(map[*op.first].getDepth() >= map[p[i]].getDepth() + 1) {
				    temp.insert(*op.first);
				}
				op.first++;
			    }
			    BOOST_FOREACH( Edge te, temp ) {
				TransitionInfo tti = map[te];
				tti.setDepth(tti.getDepth()+1);
				
				boost::remove_edge(te, g);
				//std::cerr << "Re-inserting " << tti.getLabel() << " (" << tti.getStart() << "," << tti.getEnd() << ")" << std::endl;
				boost::add_edge(tti.getStart(), tti.getEnd(), tti, g);
			    }
			    
			    //std::cerr << "Adding left " << new_label << " (" << start << "," << end << ")" << std::endl;
			    std::pair<Edge, bool> ne = boost::add_edge(start, end, ti, g);
			    head_edge = ne.first;
			}
			
			// Binarize from head to the right
			for(int i = head_idx + 1; i < p.size()-1; i++) {   
			    int start = map[p[0]].getStart();
			    int end   = map[p[i]].getEnd();
			    
			    std::string new_label = map[e].getLabel();
			    //std::string new_label = map[e].getLabel() + "_B";
			    
			    TransitionInfo ti;
			    ti.setType("group");
			    ti.setLabel(new_label);
			    ti.setStart(start);
			    ti.setEnd(end);
			    ti.setHead(map[e].getHead());
			    
			    ti.setDepth(map[head_edge].getDepth()+1);
			    
			    std::set<Edge> temp;
			    std::pair<OutEdgeIt,OutEdgeIt> op = boost::out_edges(boost::source(head_edge, g), g);
			    while(op.first != op.second) {
				if(map[*op.first].getDepth() >= map[head_edge].getDepth()+1) {
				    temp.insert(*op.first);
				}
				op.first++;
			    }
			    BOOST_FOREACH( Edge te, temp ) {
				TransitionInfo tti = map[te];
				tti.setDepth(tti.getDepth()+1);
				
				boost::remove_edge(te, g);
				//std::cerr << "Re-inserting " << tti.getLabel() << " (" << tti.getStart() << "," << tti.getEnd() << ")" << std::endl;
				boost::add_edge(tti.getStart(), tti.getEnd(), tti, g);
			    }
			    
			    //std::cerr << "Adding right " << new_label << " (" << start << "," << end << ")" << std::endl;
			    std::pair<Edge, bool> ne = boost::add_edge(start, end, ti, g);
			    head_edge = ne.first;
			}
			
			eit = boost::out_edges(*vit.first, g);
		    }
		}
	    }
	}
    }
}
 */

void ParseGraph::binarize() {
    TransitionMap map = boost::get(boost::edge_bundle, g);
    VertexIndex index = boost::get(boost::vertex_index, g);
    
    bool continuous = true;
    
    for(int i = 0; i < num_vertices()-1 && continuous; i++) {
        Vertex act = vertex(i, g);

        std::pair<OutEdgeIt,OutEdgeIt> p = boost::out_edges(act, g);
        bool step = false;
	while(p.first != p.second && !step) {
	    //std::cerr << *p.first << std::endl;
	    if(boost::target(*p.first, g) - act == 1) {
		step = true;
	    }
	    p.first++;
	}
	//std::cerr << "step: " << step << std::endl;
	if(!step)
	    continuous = false;
    }
    
    //std::cerr << continuous << std::endl;
    
    if(continuous) {
	for(std::pair<VertexIt, VertexIt> vit = boost::vertices(g); vit.first != vit.second; vit.first++) {
	    for(std::pair<OutEdgeIt,OutEdgeIt> eit = boost::out_edges(*vit.first, g); eit.first != eit.second; eit.first++) {
		Edge e = *eit.first;
		
		if(map[e].isPhrasal()) {
		    Path p = shallow_subpath_edges(e);
		    
		    //std::cerr << map[e].getLabel() << " " << e << " : " << p.size() << std::endl;
		    
		    if(p.size() > 2) {
			// Find head
			int head_idx = 0;
			for(int i = 0; i < p.size(); i++)
			    if(map[p[i]].getHead() == map[e].getHead())
				head_idx = i;
			
			Edge head_edge = p[head_idx];
			
			// Binarize from head to the right
			
			for(int i = head_idx + 1; i < p.size() && i - head_idx < p.size()-1; i++) {   
			    int start = map[head_edge].getStart();
			    int end   = map[p[i]].getEnd();
			    
			    std::string new_label = map[e].getLabel();
			    //std::string new_label = map[e].getLabel() + "_B";
			    
			    TransitionInfo ti;
			    ti.setType("group");
			    ti.setLabel(new_label);
			    ti.setStart(start);
			    ti.setEnd(end);
			    ti.setHead(map[e].getHead());
			    
			    ti.setDepth(map[head_edge].getDepth()+1);
			    
			    std::set<Edge> temp;
			    std::pair<OutEdgeIt,OutEdgeIt> op = boost::out_edges(boost::source(head_edge, g), g);
			    while(op.first != op.second) {
				if(map[*op.first].getDepth() >= map[head_edge].getDepth()+1) {
				    temp.insert(*op.first);
				}
				op.first++;
			    }
			    BOOST_FOREACH( Edge te, temp ) {
				TransitionInfo tti = map[te];
				tti.setDepth(tti.getDepth()+1);
				
				boost::remove_edge(te, g);
				//std::cerr << "Re-inserting " << tti.getLabel() << " (" << tti.getStart() << "," << tti.getEnd() << ")" << std::endl;
				boost::add_edge(tti.getStart(), tti.getEnd(), tti, g);
			    }
			    
			    //std::cerr << "Adding " << new_label << " (" << start << "," << end << ")" << std::endl;
			    std::pair<Edge, bool> ne = boost::add_edge(start, end, ti, g);
			    head_edge = ne.first;
			}
			
			// Binarize from head to the left
			for(int i = head_idx - 1; i > 0; i--) {			    
			    int start = map[p[i]].getStart();
			    int end   = map[p.back()].getEnd();
			    
			    std::string new_label = map[e].getLabel();
			    //std::string new_label = map[e].getLabel() + "_B";
			    
			    TransitionInfo ti;
			    ti.setType("group");
			    ti.setLabel(new_label);
			    ti.setStart(start);
			    ti.setEnd(end);
			    ti.setHead(map[e].getHead());
			    
			    ti.setDepth(map[p[i]].getDepth() + 1);
			    
			    std::set<Edge> temp;
			    std::pair<OutEdgeIt,OutEdgeIt> op = boost::out_edges(boost::source(p[i], g), g);
			    while(op.first != op.second) {
				if(map[*op.first].getDepth() >= map[p[i]].getDepth() + 1) {
				    temp.insert(*op.first);
				}
				op.first++;
			    }
			    BOOST_FOREACH( Edge te, temp ) {
				TransitionInfo tti = map[te];
				tti.setDepth(tti.getDepth()+1);
				
				boost::remove_edge(te, g);
				//std::cerr << "Re-inserting " << tti.getLabel() << " (" << tti.getStart() << "," << tti.getEnd() << ")" << std::endl;
				boost::add_edge(tti.getStart(), tti.getEnd(), tti, g);
			    }
			    
			    //std::cerr << "Adding " << new_label << " (" << start << "," << end << ")" << std::endl;
			    boost::add_edge(start, end, ti, g);
			}
			
			eit = boost::out_edges(*vit.first, g);
		    }
		}
	    }
	}
    }
}

std::string ParseGraph::write_tokens() {
    std::stringstream ss;
    VertexIndex index = boost::get(boost::vertex_index, g);
    TransitionMap map = boost::get(boost::edge_bundle, g);

    for(int i = 0; i < num_vertices(); i++) {
        Vertex act = vertex(i, g);

        std::pair<OutEdgeIt,OutEdgeIt> p2 = boost::out_edges(act, g);
        std::list<std::string> in;
        while(p2.first != p2.second) {
            Edge e = *p2.first;
            if(map[e].isLexical()) {
        	TransitionInfo ti = map[e];
                ss << ti.getLabel() << " ";
            }
            p2.first++;
        }
    }

    return ss.str();
}

std::string ParseGraph::write_berkeley() {
    std::stringstream ss;
    VertexIndex index = boost::get(boost::vertex_index, g);
    TransitionMap map = boost::get(boost::edge_bundle, g);

    for(int i = 0; i < num_vertices(); i++) {
        Vertex act = vertex(i, g);

        std::pair<InEdgeIt,InEdgeIt> p1 = boost::in_edges(act, g);
        while(p1.first != p1.second) {
            Edge e = *p1.first;
            if(!map[e].isLexical())
                ss << ") ";
            p1.first++;
        }

        std::pair<OutEdgeIt,OutEdgeIt> p2 = boost::out_edges(act, g);
        std::list<std::string> in;
        while(p2.first != p2.second) {
            Edge e = *p2.first;
            if(!map[e].isLexical()) {
                ss << "(";
                ss << map[e].getLabel();
                ss << " ";
            }
            else {
                ss << map[e].getLabel();
            }
            p2.first++;
        }
    }

    return ss.str();
}

std::string ParseGraph::write_constituents() {
    std::stringstream ss;
    TransitionMap map = get(boost::edge_bundle, g);
	
    std::pair<EdgeIt, EdgeIt> p = boost::edges(g);
    while(p.first != p.second) {
        Edge e = *p.first;
        
        if(map[e].isPhrasal()) {
            ss << map[e].getLabel();
            ss << "(";
            ss << map[e].getStart();
            ss << ",";
            ss << map[e].getEnd();
            ss << ") ";
        }
        
        p.first++;
    }
    return ss.str();
}

SListPtr ParseGraph::deepest_path() {
    SListPtr path( new SList() );

    if(num_vertices() == 0)
        return path;

    VertexIndex index = get(boost::vertex_index, g);
    Vertex move = start;

    while(path->size() == 0 or index[move] < index[end]) {
        if(index[move] == index[end])
            return path;

        std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
        if(p.first == p.second) {
            path->clear();
            return path;
        }
        else {    
            Edge e = *(--p.second);
            path->push_back( edge_to_symbol(e) );
            move = boost::target(e, g);
        }
    }

    return path;
}

void ParseGraph::list_deepest_paths() {
    std::pair<EdgeIt,EdgeIt> p = boost::edges(g);
    TransitionMap map = get(boost::edge_bundle, g);

    while(p.first != p.second) {
        Edge e = *p.first;
        Symbol sym( Label(map[e].getSymbol()), Range( map[e].getStart(), map[e].getEnd() ) );
        SListPtr s = deepest_subpath(sym);
        if(s->front() != sym) {
            std::cout << sym.label() << " --> "; 
            std::cout << s->str() << std::endl;
        } 
        p.first++;
    }
}

SListPtr ParseGraph::deepest_subpath(Symbol lhs) {
    SListPtr path( new SList() );

    VertexIndex index = get(boost::vertex_index, g);
    Vertex move = vertex(lhs.start(), g);

    while(path->size() == 0 or index[move] < lhs.end()) {
        if(index[move] == lhs.end() )
            return path;

        std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
        if(p.first == p.second) {
            path->clear();
            return path;
        }
        else {    
            Edge e = *(--p.second);
            path->push_back( edge_to_symbol(e) );
            move = boost::target(e, g);
        }
    }

    return path;
}


SListPtr ParseGraph::shallow_path() {
    SListPtr path( new SList() );

    VertexIndex index = get(boost::vertex_index, g);
    Vertex move = start;

    while(path->size() == 0 or index[move] < index[end]) {
        if(index[move] == index[end])
            return path;

        std::pair<OutEdgeIt, OutEdgeIt> p = out_edges(move, g);
        if(p.first == p.second) {
            return path;
        }
        else {
            while(index[target(*p.first,g)] > index[end])
                p.first++;

	    Edge e = *p.first;
	    if(boost::target(e, g) > boost::source(e,g)) {    
		path->push_back( edge_to_symbol(e) );
		move = target(e, g);
	    }
        }
    }

    return path;
}

SListPtr ParseGraph::shallow_subpath(Symbol lhs) {
    SListPtr slist( new SList() );
    
    TransitionMap map = boost::get(boost::edge_bundle, g);

    VertexIndex index = boost::get(boost::vertex_index, g);
    Vertex move = boost::vertex(lhs.start(), g);
    
    
    if(lhs.end()-lhs.start() > 1) {
        while(slist->size() == 0 or index[move] < lhs.end()) {
            if(index[move] == lhs.end())
                return slist;

            std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
            if(p.first == p.second) {
                //path->clear();
                return slist;
            }
            else {

		bool seen = 0;
		while(index[target(*p.first,g)] > lhs.end() || (index[move] == lhs.start() && index[target(*p.first,g)] == lhs.end() && !seen)) {
		    //if(index[move] == lhs.start() && index[target(*p.first,g)] == lhs.end() && lhs.label() == map[*p.first].getSymbol())
			//seen = true;
                    p.first++;
		}
		
		Edge e = *p.first;
	
		slist->push_back( edge_to_symbol(e) );
		move = target(e,g);
            }
        }
    }
    else {
        std::pair<OutEdgeIt, OutEdgeIt> p = out_edges(move, g);
        while(p.first != p.second and edge_to_symbol(*p.first).str() != lhs.str())
            p.first++;

        if(p.first != p.second) {
            int depth = map[*p.first].getDepth();
            while(p.first != p.second and map[*p.first].getDepth() >= depth)
                p.first++;
        }

        if(p.first != p.second) {
            Symbol t = edge_to_symbol(*p.first);
            Symbol s(t.label(), Range(t.start(), t.end()), t.is_nt());
            slist->push_back( s );
        }
        else {
            Symbol t = edge_to_symbol(*(--p.first));
            Symbol s(t.label(), Range(t.start(), t.end()), t.is_nt());
            slist->push_back( s );
        }
    }
    
    
    /*
    std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
    while(
	p.first != p.second && !(
	    lhs.start() == index[boost::source(*p.first, g)] && 
	    lhs.end()   == index[boost::target(*p.first, g)] && 
	    lhs.label() == map[*p.first].getSymbol()
	)
    ) {
	p.first++;
    }
            
    Path path = shallow_subpath_edges(*p.first);
    BOOST_FOREACH( Edge e, path )
	slist->push_back( edge_to_symbol(e) );
    
    */
    
    if(slist->size() == 0) {
        slist->push_back( Symbol("<Missing>", Range(-1,-1), false) );
    }

    return slist;
}

std::string ParseGraph::write_subgraphs(int n = 4, int m = 2) {
    std::stringstream ss;
    std::set<std::string> phrases = get_subgraphs(n,m);
    BOOST_FOREACH(std::string s, phrases)
	ss << s << std::endl;
    return ss.str();
}

std::set<std::string> ParseGraph::get_subgraphs(int n = 4, int m = 2) {
    TransitionMap map = boost::get(boost::edge_bundle, g);
    std::pair<EdgeIt,EdgeIt> p = boost::edges(g);
    
    std::set<std::string> phrases;
    while(p.first != p.second) {
	std::set<std::string> subphrases = get_subgraph(*p.first, 4,2);
	phrases.insert(subphrases.begin(), subphrases.end());
	p.first++;
    }
    return phrases;
}

std::set<std::string> ParseGraph::get_subgraph(ParseGraph::Edge e, int n = 4, int m = 2) {
    std::set<std::string> phrases;
    
    TransitionMap map = boost::get(boost::edge_bundle, g);

    Vertex start = boost::source(e, g);
    Vertex end = boost::target(e, g);

    int nt = 0;
    int mt = 0;
    
    std::vector<Edge> queue;
    std::vector<Edge> path;
    
    std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(start, g);
    while(p.first != p.second) {
	if(boost::target(*p.first, g) <= end && map[*p.first].getDepth() < map[e].getDepth())
	    queue.push_back(*(p.first));
	p.first++;
    }
    
    Vertex move = start;
    while( queue.size() ) {
	
	// Get new node
	Edge oe = queue.back();
	queue.pop_back();
	
	//std::cerr << "Checking edge " << oe << " : ";
	//std::cerr << map[oe].getSymbol() << "(" << map[oe].getDepth() << ")" << std::endl;
	
	while(path.size() && boost::target(path.back(), g) != boost::source(oe,g)) {
	    if(!map[path.back()].isLexical())
		mt--;
	    nt--;
	    path.pop_back();
	}
	    
	move = boost::target(oe, g);
	path.push_back(oe);
	nt++;
	if(!map[oe].isLexical())
	    mt++;
	
	// Check node for goal
	if(move == end) {
	    std::stringstream ss;
	    
	    ss << map[e].getSymbol() << "( ";
	    BOOST_FOREACH( Edge pe, path ) {
		Vertex i = boost::source(pe, g);
		std::pair<OutEdgeIt, OutEdgeIt> pi = boost::out_edges(i, g);
		while(pi.first != pi.second) {
		    if(
		       ( i == boost::source(e, g) && map[*pi.first].getDepth() < map[e].getDepth() && map[*pi.first].getDepth() > map[pe].getDepth() ) ||
		       ( i  > boost::source(e, g) && map[*pi.first].getDepth() > map[pe].getDepth() ) 
		    )
			ss << map[*pi.first].getSymbol() << "( ";
		    pi.first++;
		}
		ss << map[pe].getSymbol() << " ";
		
		Vertex j = boost::target(pe, g);
		std::pair<InEdgeIt, InEdgeIt> pj = boost::in_edges(j, g);
		while(pj.first != pj.second) {
		    if( boost::source(*pj.first, g) == i ) {
			if( map[*pj.first].getDepth() > map[pe].getDepth() ) {
			    if( i == start ) {
				if( map[*pj.first].getDepth() < map[e].getDepth() )
				ss << ") ";
			    }
			    else {
				ss << ") ";
			    }
			}
		    }
		    else {
			if( boost::source(*pj.first, g) >= start ) {
			    if(boost::source(*pj.first, g) == start) {
				if(map[*pj.first].getDepth() < map[e].getDepth()) {
				    ss << ") ";
				}
			    }
			    else {
				if(boost::source(*pj.first, g) < i)
				    ss << ") ";
			    }
			}
		    }
		    
		    
		    pj.first++;
		}
	    
	    }
	    ss << ")";
	    phrases.insert(ss.str());
	}
	
	// Expand new nodes
	std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
	while(p.first != p.second) {
	    if(boost::target(*p.first, g) <= end && nt < n && (mt < m || map[*p.first].isLexical())) 
		queue.push_back(*p.first);
	    p.first++;
	}
    }
    
    return phrases;
}


ParseGraph::Path ParseGraph::shallow_subpath_edges(ParseGraph::Edge e) {
    std::vector<Edge> path;
    TransitionMap map = boost::get(boost::edge_bundle, g);

    VertexIndex index = boost::get(boost::vertex_index, g);
    Vertex move = boost::vertex(boost::source(e, g), g);

    if(index[boost::target(e, g)] - index[boost::source(e, g)] > 1) {
        while(path.size() == 0 or index[move] < index[boost::target(e, g)]) {
            if(index[move] == index[boost::target(e, g)])
                return path;

            std::pair<OutEdgeIt, OutEdgeIt> p = boost::out_edges(move, g);
            if(p.first == p.second) {
                //path->clear();
                return path;
            }
            else {
                while(
		    index[boost::target(*p.first,g)] > index[boost::target(e, g)] or
		    (index[move] == index[boost::source(e, g)] and index[boost::target(*p.first,g)] == index[boost::target(e, g)] and map[*p.first].getDepth() >= map[e].getDepth())
		) {
                    p.first++;
		}
		
		Edge e_t = *p.first;
	
		path.push_back(e_t);
		move = index[boost::target(e_t, g)];
            }
        }
    }
    else {
        std::pair<OutEdgeIt, OutEdgeIt> p = out_edges(move, g);
        while(p.first != p.second and map[*p.first].getLabel() != map[e].getLabel())
            p.first++;

        if(p.first != p.second) {
            int depth = map[*p.first].getDepth();
            while(p.first != p.second and map[*p.first].getDepth() >= depth)
                p.first++;
        }

        if(p.first != p.second) {
            path.push_back( *p.first );
        }
        else {
            path.push_back( *(--p.first) );
        }
    }

    return path;
}


ParseGraph::Graph* ParseGraph::getBoostGraph() {
    return &g;
}
    
Symbol ParseGraph::edge_to_symbol(Edge e) {
    return Symbol(
            Label(g[e].getSymbol()),
            Range(g[e].getStart(), g[e].getEnd()),
            !g[e].isLexical() 
        );
}

void ParseGraph::load_itf(std::string serializedGraph)
{
    int i = 0;
    std::string s = "";
    while (i < serializedGraph.size())
    {
        if (serializedGraph[i] == '/')
            i ++;
        s += serializedGraph[i];
        i ++;
    }
    this->load(s);
//    std::stringstream ss;
//    ss << serializedGraph;
//    boost::archive::text_iarchive ifs(ss);
//
//    //ifs >> graph;
//    Graph tg;
//    ifs >> tg;
//    TransitionMap map = boost::get(boost::edge_bundle, tg);
//    std::pair<EdgeIt,EdgeIt> p = boost::edges(tg);
//    while(p.first != p.second) {
//        std::string label = map[*p.first].getLabel();
//        std::string s = "";
//        int i = 0;
//        while (i < label.size())
//        {
//            if (label[i] == '/')
//                i ++;
//            s += label[i];
//            i ++;
//        }
//        map[*p.first].setLabel(s);
//        for (std::vector<PosInfo>::iterator piit = map[*p.first].variants_.begin(); piit != map[*p.first].variants_.end(); piit ++)
//        {
//            std::string base = boost::get<0>(*piit);
//            std::string s = "";
//            int i = 0;
//            while (i < base.size())
//            {
//                if (base[i] == '/')
//                    i ++;
//                s += base[i];
//                i ++;
//            }
//            //boost::get<0>(map[*piit) = s;
//        }
//        boost::add_edge(source(*p.first,tg), target(*p.first,tg), map[*p.first], this->g);
//        p.first++;
//    }
//    this->end = boost::vertex(boost::num_vertices(this->g)-1,this->g);
//    this->start = 0;
}


