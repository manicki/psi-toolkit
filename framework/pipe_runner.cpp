#include "pipe_runner.hpp"

#include <iostream>

PipeRunner::PipeRunner(int argc, char* argv[]) {
}

int PipeRunner::run() {
    std::cout << "Hello world!" << std::endl;

    return 0;
}
