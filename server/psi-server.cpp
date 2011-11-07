#include <string>

#include "server_runner.hpp"
#include "configurator.hpp"

void checkIfRunAsInstalled(char* arg0);

int main(int argc, char* argv[]) {

    checkIfRunAsInstalled(argv[0]);

    ServerRunner serverRunner(argc, argv);
    return serverRunner.run();
}


void checkIfRunAsInstalled(char* arg0) {
    std::string programName(arg0);
    if (programName.find("framework/") == 0
        || programName.find("./framework/") == 0)
        Configurator::getInstance().setRunAsInstalled(false);
}
