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

    //ndfsa.print();
    //std::cout << std::endl;

    //std::string s1 = "agencja";
    //std::string s2 = "agentka";
    //std::string s3 = "agent";
    //std::string s4 = "agentowski";
    //
    //std::cout << s1 << " : " << ndfsa.in(s1.begin(), s1.end()) << std::endl;
    //std::cout << s2 << " : " << ndfsa.in(s2.begin(), s2.end()) << std::endl;
    //std::cout << s3 << " : " << ndfsa.in(s3.begin(), s3.end()) << std::endl;
    //std::cout << s4 << " : " << ndfsa.in(s4.begin(), s4.end()) << std::endl;
    //
    //std::cout << std::endl;

    minimize(ndfsa);

    psi::DFSA<> dfsa;

    dfsa = ndfsa;
    dfsa.print();

    //std::cout << std::endl;
    //
    //std::cout << s1 << " : " << dfsa.in(s1.begin(), s1.end()) << std::endl;
    //std::cout << s2 << " : " << dfsa.in(s2.begin(), s2.end()) << std::endl;
    //std::cout << s3 << " : " << dfsa.in(s3.begin(), s3.end()) << std::endl;
    //std::cout << s4 << " : " << dfsa.in(s4.begin(), s4.end()) << std::endl;

    std::cerr << "Mapping" << std::endl;

    psi::MapBinDFSA bdfsa1;
    psi::MemBinDFSA bdfsa2;
    unify(bdfsa1, dfsa);
    unify(bdfsa1, dfsa);
    bdfsa1.print();
    std::cout << std::endl;
    unify(bdfsa2, dfsa);
    bdfsa2.print();
    std::cout << std::endl;

    //psi::NDFSA<> test;
    //psi::NDFSA<> str1(s2.begin(), s2.end());
    //psi::NDFSA<> str2(s4.begin(), s4.end());
    //unify(test, str1);
    //unify(test, str2);

    //minimize(test);
    
    std::cerr << "Minimizing" << std::endl;
    std::cerr << bdfsa1.size() << std::endl;
    minimize(bdfsa1);
    std::cerr << bdfsa1.size() << std::endl;
    //reverse(bdfsa);
    //reverse(bdfsa);
    //std::cerr << bdfsa.size() << std::endl;
    //bdfsa.print();
    //
    //std::cout << std::endl;
    //intersect(test, bdfsa);

    bdfsa1.print();

    return 0;
}
