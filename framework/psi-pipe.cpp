#include "pipe_runner.hpp"

int main(int argc, char* argv[]) {
    PipeRunner pipeRunner(argc, argv);

    return pipeRunner.run();
}
