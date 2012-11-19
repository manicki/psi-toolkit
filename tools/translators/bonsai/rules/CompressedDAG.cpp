#include "CompressedDAG.hpp"

namespace poleng
{

namespace bonsai
{

using namespace rules;

/*
@todo (sugestie):
 - destruktory
 - ogólna kosmetyka, czyli obsługa wyjątków, komunikatów o błędach We/Wy itp.
 - dynamiczne mapowanie/wczytywanie fragmentów plików ( pamiętać o czyszczeniu pamięci )
 - częśćiowe przechowywanie w pamięci tablicy arcs, np. 100 MB najczęściej wykorzystywanych przejść
 - byłoby fajnie, gdyby można było określać maksymalną liczbę zaadresowanej pamięci, 
   np. wykorzystaj 250 MB tak dobrze jak możesz. Chodzi o to, że nawet jak korzystamy z mapowanej
   pamięci, to musimy uwzględniać ograniczanie do 3GB dla całego tłumacza, który jeszcze ładuje/mapuje 
   model języka (lub kilka) i sam potrzebuje sporo pamięci w przypadku długich zdań. 
*/

// ########### Konstruktory ############

CompressedDAG::CompressedDAG() : sn(0), an(0), d_sn(0), da_start(0), da_length(0), states(NULL), arcs(NULL), sf(NULL), af(NULL), mode(0) {}

CompressedDAG::CompressedDAG(unsigned int sn_, unsigned int an_) : sn(sn_), an(an_), d_sn(0), da_start(0), da_length(0), sf(NULL), af(NULL), mode(0)  {
    assert(sn < std::numeric_limits<int>::max());  // maksmalnie 2^31 stanów, dla bezpieczeństwa
    assert(an < std::numeric_limits<int>::max());  // mozna zindeksować maksymalnie 2^31 przejść,
                                                   // ostatni bit jest zarezerwowany dla stanów końcowych
    states = new Position[sn];
    arcs = new Arc[an];
}

CompressedDAG::CompressedDAG(std::string prefix, int mode_ = 0) : mode(mode_), d_sn(0), da_start(0), da_length(0) {
    open(prefix, mode);
}

void CompressedDAG::open(std::string prefix, int mode_) {
    mode = mode_;
    
    sf = fopen((prefix + ".s").c_str(), "r");
    af = fopen((prefix + ".a").c_str(), "r");
    
    long ssize = filesize(sf);
    long asize = filesize(af);
    
    sn = ssize / sizeof(Position);
    da_start = an = asize / sizeof(Arc);
    
    if(mode == 0) {
        std::cerr << "Reading " << sn << " states to memory ... ";
        states = read_states(0, sn);
        std::cerr << "finished" << std::endl;
        
        std::cerr << "Reading " << an << " arcs to memory ... ";
        arcs = read_arcs(0, an);
	std::cerr << "finished" << std::endl;
    }
    else if(mode == 2) {
        std::cerr << "Reading " << sn << " states to memory ... ";
        states = read_states(0, sn);
        std::cerr << "finished" << std::endl;
        
        std::cerr << "Mapping " << an << " arcs into memory ... ";
        arcs = map_arcs(0, an);
        std::cerr << "finished" << std::endl;        
    }
    else if(mode == 1){
        std::cerr << "Mapping " << sn << " states into memory ... ";
        states = map_states(0, sn);
        std::cerr << "finished" << std::endl;        
        
        std::cerr << "Reading " << an << " arcs to memory ... ";
        arcs = read_arcs(0, an);
        std::cerr << "finished" << std::endl;
    }
    else if(mode == 3){
        std::cerr << "Mapping " << sn << " states into memory ... ";
        states = map_states(0, sn);
        std::cerr << "finished" << std::endl;        
        
        std::cerr << "Mapping " << an << " arcs into memory ... ";
        arcs = map_arcs(0, an);
        std::cerr << "finished" << std::endl;        
    }
    else if(mode == 4) {
	states = read_states(0, sn);
	
	int i=0;
	while(arc_no(states[i]) < an*0.4)
	    i++;
	
	da_start = arc_no(states[i]);
	arcs = read_arcs(0, da_start);
	
	std::cerr << "Read " << ((ssize + (da_start * sizeof(Arc)))/(1024*1024))  << " M from " << ((ssize + asize)/(1024*1024)) << " M to memory..." << std::endl;	
    }    
}

// ########### Funkcja sprawdzająca przynależność do automatu, funkcja haszująca, funkcja dehaszująca ############

bool CompressedDAG::in(Word &w) { // sprawdza czy dany wyraz (ciąg liczb) jest elementem języka automatu
    assert(sn > 0);
    
    State current_state = 0;
    for(int i = 0; i < w.size(); i++) {
        State next_state = delta(current_state, w[i]);    
        if(next_state != -1) {
            current_state = next_state;
        }
        else {
            return false;
        }
    }
    return is_end_state(current_state);
}

bool CompressedDAG::prefix(Word &w) {
    assert(sn > 0);
    
    State current_state = 0;
    for(int i = 0; i < w.size(); i++) {
        State next_state = delta(current_state, w[i]);    
        if(next_state != -1) {
            current_state = next_state;
        }
        else {
            return false;
        }
    }
    return true;
}

int CompressedDAG::hash(Word &w) {
    assert(sn > 0);
    
    int index = 0;
    State current_state = 0;
    for(int i = 0; i < w.size(); i++) {
        Arc* arc = find(current_state, w[i]);
        if(arc != arcs+an) {
            index += arc->w;
            current_state = arc->q;
            if(is_end_state(current_state))
                index++;
        }
        else return -1;
    }
    if(is_end_state(current_state)) {
        return index;
    }
    else {
        return -1;
    }
}

Word CompressedDAG::unhash(int index) {
    assert(sn > 0);
    
    Word w;
    State current_state = 0;
    while(index > 0) {
        Arc* arc = find_num(current_state, index);
	if(arc != arcs + an) {
            current_state = arc->q;
	    w.push_back(arc->a);
            index -= arc->w;
            
            if(is_end_state(current_state)) {
	        index--;
            }
        }
        else {
            return Word();
        }
    }
    return w;
}

// ########### Funkcje do nawigacji po automacie ###########

inline State CompressedDAG::delta(State qs, Symbol as) {  // funkcja przejścia
    if(sn == 0)
        return -1;
    Arc* arc = find(qs,as);
    if(arc == arcs + an)
        return -1;
    else
        return arc->q; 
}

inline ArcIt CompressedDAG::find(State i, Symbol j) {
    ArcIt arc = arcs + an;
    
    Range r = row(i);
    int dist = (r.second - r.first);

    if(dist > 10)
        return arc = binsearch(r.first, r.second-1, j);
    else if(dist > 0)
        return arc = linsearch(r.first, r.second-1, j);
    
    return arc; 
}

inline ArcIt CompressedDAG::find_num(State i, int count) {
    ArcIt arc = arcs + an;
    
    Range r = row(i);
    int dist = (r.second - r.first);

    if(dist > 10)
        arc = binsearch_num(r.first, r.second-1, count);
    else if(dist > 0)
        arc = linsearch_num(r.first, r.second-1, count);
    return arc;
}

Range CompressedDAG::row(State i) {  // pewnie gdzieś tutaj w tle musi się odbyć dynamiczne ładowanie/mapowanie
    if(i+1 < sn) {
	Position s = arc_no(states[i]);
	Position e = arc_no(states[i+1]);
	
	if(s > da_start) {
	    unmap_arcs();
	    da_length = e-s;
	
	    d_arcs = map_arcs(s, e);
	    return Range(d_arcs, d_arcs + da_length);
	}    
	else {
	    return Range(arcs + s, arcs + e);
	}
    }
    else if(i+1 == sn) {
	Position s = arc_no(states[i]);
        if(s > da_start) {
	    unmap_arcs();
	    da_length = an - s;
	    d_arcs = map_arcs(s, an);
    	    return Range(d_arcs, d_arcs + da_length);
	}
	else
	    return Range(arcs + s, arcs + an);
    }
    else 
        return Range(arcs + an, arcs + an);                                    // zerowy przedział, awaryjny*/
}


inline ArcIt CompressedDAG::linsearch(Arc* start, Arc* end, Symbol value) {
    for(ArcIt it = start; it <= end; it++) {
        if(it->a == value) {
            return it;
        }
    }
    return arcs + an;
}

inline ArcIt CompressedDAG::binsearch(ArcIt start, ArcIt end, Symbol value) {
    ArcIt L = start;
    ArcIt R = end; 
    while (L <= R) {
        ArcIt m = L + ((R - L)/2);
        if (m->a < value)
            L = m + 1;
        else if (m->a > value)
            R = m - 1;
        else 
            return m;
    }
    return arcs + an;  
}

inline ArcIt CompressedDAG::linsearch_num(ArcIt start, ArcIt end, Weight value) {
    for(ArcIt it = start; it <= end; it++) {
        if(it->w < value and (it == end or (it+1)->w >= value)) {
            return it;
        }
    }
    return arcs+an;
}

inline ArcIt CompressedDAG::binsearch_num(ArcIt start, ArcIt end, Weight value) {
    ArcIt L = start;
    ArcIt R = end; 
    while (L <= R) {
        ArcIt m = L + ((R - L)/2);
        if(m->w < value and (m == end or (m+1)->w >= value))
            return m;
        else if(m->w < value)
            L = m + 1;
        else
            R = m - 1;
    }
    return arcs+an;  
}

// ########### Manipulacje bitami do zaznaczanie stanów końcowych ###########

inline void CompressedDAG::set_end_state(State p) {             // funkcja, która ustawia bit stanu końcowego
    states[p] |= (1 << (sizeof(Position)*8-1));
}

bool CompressedDAG::is_end_state(State p) {               // funkcja, która sprawdza bit stanu końcowego
    //std::cerr << "test: " << p << " - " << states[p] << std::endl;
    return (bool)(states[p] & (1 << (sizeof(Position)*8-1)));
}

Position CompressedDAG::arc_no(Position pos) { // funkcja, która usuwa bit z informacją
    return (pos &= ~(1 << (sizeof(Position)*8-1)));          // o stanie końcowym, jeśli jest ustawiony
}                                                            // stad konwersja z unsigned int na int ze znakiem

// ########### Funkcje wejścia/wyjścia ###########

long CompressedDAG::filesize(FILE* f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind (f);
    return size;
}

Position* CompressedDAG::read_states(int start, int end) {
    int count = (end - start);
    int size = count * sizeof(Position);
    
    Position* state_address = (Position*) malloc(size);
    fread(state_address, sizeof(Position), count, sf);
    return state_address;
}

Arc* CompressedDAG::read_arcs(int start, int end) {
    int count = end-start;
    int size = count * sizeof(Arc);
    
    Arc* arc_address = (Arc*)malloc(size);
    fread(arc_address, sizeof(Arc), count, af);
    return arc_address;
}

Position* CompressedDAG::map_states(int start, int end) {
    int size = (end - start) * sizeof(Position);
    int offset = start * sizeof(Position);
    
    // @todo:
    // offset musi być wielokrotnością getpagesize() = 4096 (teoretycznie size również)
    // stąd trzeba tutaj jeszcze troche policzyc i przesunąć zwracany wskaźnik,
    // gdy start != 0
    
    return (Position*) mmap((caddr_t)0, size, PROT_READ, MAP_SHARED, fileno(sf), offset);
}

void CompressedDAG::unmap_states() {
    int size = sn * sizeof(Position);
    munmap(states, size);
}

Arc* CompressedDAG::map_arcs(int start, int end) {
    int size = (end - start) * sizeof(Arc);
    
    int ps = getpagesize();
    
    int offset = ((int)((start * sizeof(Arc))/ps))*ps;
    int preced = (start * sizeof(Arc)) % ps;
    
    //std::cerr << "start: " << start << " - end: " << end  << " - ps: " << ps << std::endl;
    //std::cerr << "offset: " << offset << " - preced: " << preced << std::endl;
    achars_size = preced + size;    
    achars = (char*)mmap((caddr_t)0, achars_size, PROT_READ, MAP_SHARED, fileno(af), offset);
    return (Arc*)(achars + preced);
}

void CompressedDAG::unmap_arcs() {
    if(achars != NULL and achars_size > 0)
        munmap(achars, achars_size);
}

WordListHashed CompressedDAG::multiunhash(std::set<int> &indeces) {
    typedef std::map<State, WordListHashed> StateLanguage;

    StateLanguage sl;
    sl[State(0)] = WordListHashed();
    for(std::set<int>::iterator it = indeces.begin(); it != indeces.end(); it++) {
        sl[State(0)].push_back(WordIndexHashed(Word(0),*it,*it));
    }
    
    WordListHashed language;
    
    std::list<State> queue;
    queue.push_back(State(0));
    
    int c = 0;
    
    while(!queue.empty()) {
        State ns = queue.front();
        queue.pop_front();
        c++;
        
        //std::cerr << "Entering " << ns << std::endl;
        
        WordListHashed::iterator wit = sl[ns].begin();
        WordListHashed::iterator wend = sl[ns].end();
        
        Range r = row(ns);
        ArcIt astr = r.first;
        ArcIt aend = r.second;
        
        /*if(wit->get<2>() == 43731794) {
            std::cerr << "beeep3" << std::endl;
            std::cerr << ns << " - " << astr << " " << aend << std::endl; 
            std::cerr << std::endl;
        }
        */
        ArcIt ait = astr;
        while(wit != wend and ait < aend) {
            //if(std::distance(ait, aend) > 20)
                ait = binsearch_num(astr, aend-1, wit->get<1>());
            //else
            //    ait = linsearch_num(ait, aend-1, wit->get<1>());
            
            if(ait < aend) {
                //std::cerr << ns << " --> " << ait->a << " ( i:" << wit->get<1>() << " > w:" << ait->w << " ) --> " << ait->q << std::endl;
                //
                //if(wit->get<2>() == 43731794) {
                //    std::cerr << "beeep1" << std::endl;
                //    std::cerr << "rest: " << wit->get<1>() << std::endl;
                //    Word t = wit->get<0>();
                //    for(Word::iterator it = t.begin(); it != t.end(); it++) {
                //        std::cerr << *it << " ";
                //    }
                //    std::cerr << std::endl;
                //    std::cerr << "sl.count(): " << sl.count(ait->q) << std::endl;
                //}
                
                if(sl.count(ait->q) == 0) {
                    //std::cerr << "Adding to queue: " << ait->q << std::endl;
                    queue.push_back(ait->q);
                    sl[ait->q] = WordListHashed();
                }
                
                WordIndexHashed nwi(wit->get<0>(), wit->get<1>(), wit->get<2>());
                nwi.get<0>().push_back(ait->a);
                nwi.get<1>() -= ait->w;
                
                /*if(nwi.get<2>() == 43731794) {
                    std::cerr << "beeep2" << std::endl;
                    std::cerr << "rest: " << nwi.get<1>() << std::endl;
                    Word t = nwi.get<0>();
                    for(Word::iterator it = t.begin(); it != t.end(); it++) {
                        std::cerr << *it << " ";
                    }
                    std::cerr << std::endl;
                }
                */
                
                if(is_end_state(ait->q)) {
                    //std::cerr << ait->q << " is an end state" << std::endl;
                    nwi.get<1>()--;
                }
                
                if(nwi.get<1>() > 0) {
                    sl[ait->q].push_back(nwi);
                }
                else {
                    //std::cerr << "Adding word with : " << nwi.get<1>() << std::endl;
                    language.push_back(nwi);
                }
            }
            wit++;
        }
    }
    
    //std::cerr << "all states: " << c << std::endl;
    return language;
}

WordList CompressedDAG::multihash(SimpleDAG &in) {
    typedef std::vector<WordList> StateLanguage;

    StateLanguage sl(1);
    sl[0].push_back(WordIndex(Word(0),0));
    
    SimpleDAG ndag;
    typedef std::pair<State,State> SPair;
    
    std::vector<SPair> queue;
    std::map<SPair, State> mapper;
    
    std::set<State> endstates;
    
    SPair start(0,0);
    mapper[start] = ndag.new_state(true);
    
    queue.push_back(start);
    
    int c = 0;
    
    while(!queue.empty()) {
        SPair ns = queue.back();
        queue.pop_back();
        c++;
        
        if(is_end_state(ns.first) and in.is_end_state(ns.second)) {
            ndag.set_end_state(mapper[ns]);
            endstates.insert(mapper[ns]);
        }
        
	Range r1 = row(ns.first);
        Range r2 = in.row(ns.second);
        
        ArcIt ait1 = r1.first;
        ArcIt ait2 = r2.first;

        while(ait1 != r1.second and ait2 != r2.second) {
            if(ait1->a == ait2->a) {
                SPair t(ait1->q, ait2->q);
                if(mapper.count(t) == 0) {
                    queue.insert(queue.begin(),t);
                    mapper[t] = ndag.new_state(false);    
                    sl.resize(mapper[t]+1);
                }
                ndag.new_arc(mapper[ns], mapper[t], ait1->a, ait1->w);
                
                State s = mapper[ns];
                State e = mapper[t];
                for(WordList::iterator it = sl[s].begin(); it != sl[s].end(); it++) {
                    WordIndex wi = *it;
                    wi.get<0>().push_back(ait1->a);
                    wi.get<1>() += ait1->w;
                    if(is_end_state(ait1->q))
                        wi.get<1>()++;
                    
                    sl[e].push_back(wi);
                }
                
                ait1++;
                ait2++;
            }
            else if(ait1->a < ait2->a) {
                ait1++;
            }
            else if(ait1->a > ait2->a) {
                ait2++;
            }
        }
    }
    
    WordList language;
    for(std::set<State>::iterator it = endstates.begin(); it != endstates.end(); it++) {
        language.insert(language.end(), sl[*it].begin(), sl[*it].end());
    }

    return language;
}

//WordList CompressedDAG::multihash(SimpleDAG &in) {
//    typedef std::vector<WordList> StateLanguage;
//
//    SimpleDAG intersection = intersect(in);
//    return intersection.wordlist();
//}



SimpleDAG CompressedDAG::intersect(SimpleDAG &in) {
    SimpleDAG ndag;
    
    typedef std::pair<State,State> SPair;
    
    std::vector<SPair> queue;
    std::map<SPair, rules::State> mapper;
    
    SPair start(0,0);
    mapper[start] = ndag.new_state(true);
    
    queue.push_back(start);
    
    int c = 0;
    
    while(!queue.empty()) {
        SPair ns = queue.back();
        queue.pop_back();
        c++;
        
        if(is_end_state(ns.first) and in.is_end_state(ns.second)) {
            ndag.set_end_state(mapper[ns]);
        }
        
	Range r1 = row(ns.first);
        Range r2 = in.row(ns.second);
        
	ArcIt ait1 = r1.first;
	ArcIt ait2 = r2.first;

	while(ait1 != r1.second and ait2 != r2.second) {
	    if(ait1->a == ait2->a) {
		SPair t(ait1->q, ait2->q);
		if(mapper.count(t) == 0) {
		    queue.push_back(t);
		    mapper[t] = ndag.new_state(false);
		}
		    
		ndag.new_arc(mapper[ns], mapper[t], ait1->a, ait1->w);
		ait1++;
		ait2++;
	    }
	    else if(ait1->a < ait2->a) {
		ait1++;
	    }
	    else if(ait1->a > ait2->a) {
		ait2++;
	    }
	}
    }
    
    return ndag;	
}

}

}
