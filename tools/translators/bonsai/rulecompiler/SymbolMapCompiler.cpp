#include <boost/program_options.hpp>
#include "RuleSymbolNumberMap.hpp"

namespace po = boost::program_options;

int main (int argc, char* args[]) {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
	("help", "Display this message")	
	("text", "Binary to text")	
    ;
			    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
					
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }	
    
    poleng::bonsai::rules::RuleSymbolNumberMap sm;
    if(!vm.count("text")) {
        sm.create_from_text(std::cin);    
        std::cerr << "Size: " << sm.size() << std::endl;
        sm.save(args[1]);
    }
    else {
        sm.open(std::cin);
        sm.write_to_text(std::cout);
    }
}

/*
 
 * podziel plik regul na czesc zrodlowa i czesc docelowa (perl)
 * posortuj czesc zrodlowa (LC_ALL=C) (perl)
 * posortuj czesc docelowa (LC_ALL=C) (perl)

 * zamien symbole w czesci zrodlowej na kolejne liczby zgodnie z kolejnoscia wystepowania,
   zapisz liste symboli w danej kolejnosci. (perl)
 
 * zamien symbole w czesci docelowej na kolejne liczby zgodnie z kolejnoscia wystepowania,
   zapisz liste symboli w danej kolejnosci. (perl)
   
 * wygeneruj mape symboli zrodlowych (c++)
 * wygeneruj mape symboli docelowych (c++)

 * zamien czesc zrodlowa na minimalny automat skonczony (c++)
 * zamien czesc docelowa na minimalny automat skonczony (c++)
 
 * wygeneruj plik transfer.ini z danymi
 
*/
