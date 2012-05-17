#include "active_elements_printer.hpp"


#include <sstream>

#include <boost/foreach.hpp>


ActiveElementsPrinter ActiveElementsPrinter::Manager::getPrinter(std::string pattern) {
    return ActiveElementsPrinter(
        pattern,
        tagSeparator_
    );
}


std::string ActiveElementsPrinter::print(
    std::list<std::string> tags
) {
    std::stringstream result;
    size_t i = 0;
    while (i < pattern_.length()) {
        if (pattern_[i] == '%' && i+1 < pattern_.length()) {
            switch (pattern_[i+1]) {
            case 'T' :
                {
                    std::stringstream tagSs;
                    BOOST_FOREACH(std::string tag, tags) {
                        if (!tagSs.str().empty()) {
                            tagSs << tagSeparator_;
                        }
                        tagSs << tag;
                    }
                    result << tagSs.str();
                    i += 2;
                }
                break;
            default :
                {
                    result << '%';
                    i++;
                }
                break;
            }
        } else {
            result << pattern_[i];
            i++;
        }
    }
    return result.str();
}
