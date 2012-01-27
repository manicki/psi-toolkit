#include <iostream>
#include <string>

#include "QueryGrammars.hpp"

int main(int argc, char** argv) {    
    typedef psi::NDFSAGrammar<std::string::const_iterator, psi::NDFSA<> > Grammar;
    
    Grammar g;
    
    using boost::spirit::ascii::space;
    
    std::string text;
    while(std::getline(std::cin, text)) {
        std::string::const_iterator iter = text.begin();
        std::string::const_iterator end = text.end();
        
        psi::NDFSA<> query; 
        bool r = phrase_parse(iter, end, g, space, query);
        
        if(r && iter == end) {
            std::cout << "Parsing successful" << std::endl;
            
            std::cout << query.size() << std::endl;
            query.print();
            std::cout << std::endl;
            
            psi::epsRemove(query);
            query.print();
            std::cout << std::endl;
            
            psi::determinize(query);
            query.print();
            std::cout << std::endl;
            
            psi::minimize(query);
            query.print();
            std::cout << std::endl;
            
        }
    }
    return 0;
}