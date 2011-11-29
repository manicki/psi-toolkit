#include <iostream>
#include <string>

#include "DFSA.hpp"
#include "Algorithms.hpp"

int main(int argc, char** argv) {    
    psi::DFSA<> fsa;
    
    psi::State q0 = fsa.addState();
    fsa.setStartState(q0);
    
    psi::State p, q;
    
    std::string s = "agentka";
    p = q = q0;
    for(size_t i = 0; i < s.size(); i++) {
        p = q;
        q = fsa.addState();
        
        fsa.addArc(p, psi::Arc<>(s[i], q));
    }
    fsa.setEndState(q);
    
    s = "agencja";
    p = q = q0;
    for(size_t i = 0; i < s.size(); i++) {
        p = q;
        q = fsa.addState();
        
        fsa.addArc(p, psi::Arc<>(s[i], q));
    }
    fsa.setEndState(q);
    
    fsa.print();
    
    std::cout << std::endl;
    
    minimize(fsa);
    fsa.print();
    
    return 0;
}