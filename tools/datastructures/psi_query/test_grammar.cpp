#include <iostream>
#include <string>

#include "QueryGrammars.hpp"

int main(int argc, char** argv) {    
    typedef psi::CharGrammar<std::string::const_iterator, psi::NDFSA<> > Grammar;
    
    std::cout << "Enter automaton:" << std::endl;
    std::string regex;
    std::getline(std::cin, regex);
    
    std::string::const_iterator iter = regex.begin();
    std::string::const_iterator end = regex.end();
    
    Grammar g;
    psi::NDFSA<> automaton;
    
    bool r = parse(iter, end, g, automaton);
    if(r && iter == end) {
        std::cout << "Parsing successful" << std::endl;
        
        std::cout << automaton.size() << std::endl;
        automaton.print();
        std::cout << std::endl;
        
        psi::epsRemove(automaton);
        automaton.print();
        std::cout << std::endl;
        
        psi::determinize(automaton);
        automaton.print();
        std::cout << std::endl;
        
        psi::minimize(automaton);
        automaton.print();
        std::cout << std::endl;
        
        
        std::cout << "Enter words:" << std::endl;
        
        std::string word;
        while(std::getline(std::cin, word))
            std::cout << automaton.in(word.begin(), word.end()) << std::endl;
    }
    return 0;
}