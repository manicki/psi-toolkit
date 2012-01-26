#include <string>

#include "pipe_runner.hpp"

#include "configurator.hpp"

void checkIfRunAsInstalled(char* arg0);

int main(int argc, char* argv[]) {
    checkIfRunAsInstalled(argv[0]);

    int exit_code = 1;

    try {
        PipeRunner pipeRunner(argc, argv);
        exit_code = pipeRunner.run(std::cin, std::cout);
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl
                  << "ABORTED!" << std::endl;

        return 1;
    }

    return exit_code;
}

void checkIfRunAsInstalled(char* arg0) {
    std::string programName(arg0);
    if (programName.find("framework/") == 0
        || programName.find("./framework/") == 0)
        Configurator::getInstance().setRunAsInstalled(false);
}
