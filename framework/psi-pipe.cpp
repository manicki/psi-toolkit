#include "pipe_runner.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "try this: ./psi-pipe txt-reader ! tp-tokenizer ! psi-writer\n";
        exit(1);
    }

    PipeRunner pipeRunner(argc, argv);

    return pipeRunner.run();
}
