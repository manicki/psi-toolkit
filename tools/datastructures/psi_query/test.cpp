#include <iostream>
#include <string>

#include "NDFSA.hpp"
#include "DFSA.hpp"
#include "Algorithms.hpp"

int main(int argc, char** argv) {    
    psi::NDFSA<> ndfsa;

    psi::State q0 = ndfsa.addState();
    ndfsa.setStartState(q0);
    
    psi::State p, q;
    
    std::string s = "agentka";
    p = q = q0;
    for(size_t i = 0; i < s.size(); i++) {
        p = q;
        q = ndfsa.addState();
        
        ndfsa.addArc(p, psi::Arc<>(s[i], q));
    }
    ndfsa.setEndState(q);
    
    s = "agencja";
    p = q = q0;
    for(size_t i = 0; i < s.size(); i++) {
        p = q;
        q = ndfsa.addState();
        
        ndfsa.addArc(p, psi::Arc<>(s[i], q));
    }
    ndfsa.setEndState(q);
    
    ndfsa.print();
    
    std::cout << std::endl;
    
    std::string s1 = "agencja";
    std::string s2 = "agentka";
    std::string s3 = "agent";
    std::string s4 = "agentowski";
    
    std::cout << s1 << " : " << ndfsa.in(s1.begin(), s1.end()) << std::endl;
    std::cout << s2 << " : " << ndfsa.in(s2.begin(), s2.end()) << std::endl;
    std::cout << s3 << " : " << ndfsa.in(s3.begin(), s3.end()) << std::endl;
    std::cout << s4 << " : " << ndfsa.in(s4.begin(), s4.end()) << std::endl;
    
    std::cout << std::endl;
    
    minimize(ndfsa);
    
    psi::DFSA<> dfsa;
    
    dfsa = ndfsa;
    dfsa.print();

    std::cout << std::endl;

    std::cout << s1 << " : " << dfsa.in(s1.begin(), s1.end()) << std::endl;
    std::cout << s2 << " : " << dfsa.in(s2.begin(), s2.end()) << std::endl;
    std::cout << s3 << " : " << dfsa.in(s3.begin(), s3.end()) << std::endl;
    std::cout << s4 << " : " << dfsa.in(s4.begin(), s4.end()) << std::endl;
        
    return 0;
}