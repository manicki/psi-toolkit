#include <iostream>
#include <string>

#include <vector>
#include <boost/foreach.hpp>

#include "NDFSA.hpp"
#include "DFSA.hpp"
#include "BinDFSA.hpp"

#include "Algorithms.hpp"

int main(int argc, char** argv) {    
    
    std::vector<std::string> strings;
    strings.push_back("agentka");
    strings.push_back("agencja");
    
    psi::NDFSA<> ndfsa;
    BOOST_FOREACH( std::string s, strings ) {
        psi::NDFSA<> strfsa(s.begin(), s.end());
        unify(ndfsa, strfsa);
    }
    
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
        
    psi::BinDFSA<> bdfsa;
    unify(bdfsa, dfsa);
    
    bdfsa.print();
    std::cout << std::endl;
    
    psi::NDFSA<> test;
    psi::NDFSA<> str1(s2.begin(), s2.end());
    psi::NDFSA<> str2(s4.begin(), s4.end());
    unify(test, str1);
    unify(test, str2);
    
    minimize(test);
    minimize(ndfsa);
    test.print();
    
    std::cout << std::endl;
    intersect(test, ndfsa);
    
    test.print();
    
    return 0;
}