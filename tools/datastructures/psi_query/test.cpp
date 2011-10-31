#include <iostream>

#include "DFSA.hpp"
#include "Algorithms.hpp"

int main(int argc, char** argv) {    
    psi::DFSA<> fsa;
    psi::State q0 = fsa.addState();
    psi::State q1 = fsa.addState();
    psi::State q2 = fsa.addState();
    psi::State q3 = fsa.addState();
    
    fsa.addArc(q0, psi::Arc<>('a', q1));
    fsa.addArc(q1, psi::Arc<>('b', q2));
    fsa.addArc(q2, psi::Arc<>('c', q3));
    
    fsa.setStartState(q0);
    fsa.setEndState(q3);
    
    std::string test1 = "abc";
    std::string test2 = "cba";
    
    std::cout << fsa.in(test1.begin(), test1.end()) << std::endl;
    std::cout << fsa.in(test2.begin(), test2.end()) << std::endl;
    
    std::cout << "Reversing" << std::endl;
    psi::reverse(fsa);
    
    std::cout << fsa.in(test1.begin(), test1.end()) << std::endl;
    std::cout << fsa.in(test2.begin(), test2.end()) << std::endl;
    
    return 0;
}