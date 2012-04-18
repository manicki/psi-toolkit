#include "antiword_adapter_impl.hpp"

#include <cstdlib>


void AntiwordAdapterImpl::convertWordToText(
    std::string sourceFileName,
    std::string targetFileName
) {
    std::string command("antiword " + sourceFileName + " 1>" + targetFileName);
    system(command.c_str());
}
