#include "server_runner.hpp"

int main(int argc, char* argv[]) {

    ServerRunner serverRunner(argc, argv);
    return serverRunner.run();
}
