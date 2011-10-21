#include "pipe_runner.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "try this: cd bin; framework/psi-pipe txt-reader ! tp-tokenizer --lang pl ! psi-writer\n";
        exit(1);
    }

    int exit_code = 1;

    try {
        PipeRunner pipeRunner(argc, argv);
        exit_code = pipeRunner.run(std::cin, std::cout);
    }
    catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl
                  << "ABORTED!" << std::endl;

        return 1;
    }

    return exit_code;
}
