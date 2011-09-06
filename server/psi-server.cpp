#include <iostream>

#include "site_hello_world.hpp"

int main(int argc, char* argv[])
{
  try {
    // Check command line arguments.
    if (argc != 5) {
      std::cerr << "Simple HTTP Server based on example 3 from the boost::asio documentation\n";
      std::cerr << "Usage: psi-server <address> <port> <threads> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    psi-server 0.0.0.0 3000 1 ./site\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    psi-server 0::0 3000 1 ./site\n";
      return 1;
    }

    PsiServer srv(argv[1], argv[2], argv[3], argv[4]);
	std::cerr << srv.info();
	SiteHelloWorld hello(srv);

	srv.run();
  }
  catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

