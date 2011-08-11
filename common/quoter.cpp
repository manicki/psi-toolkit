#include "quoter.hpp"


std::string Quoter::escape(std::string str) {
    return doEscape(str);
}

std::string Quoter::unescape(std::string str) {
    return doUnescape(str);
}

Quoter::~Quoter() {
}
