#include <iostream>

#include "index_site.hpp"
#include "pipe_site.hpp"

int main(int argc, char* argv[])
{
  try {
    // Check command line arguments.
    if (argc != 5) {
		std::cerr << "PSI SERVER version 0.1\n";
		std::cerr << "Simple multithreading web server based on Boost Asio server3 and C++ WEBEM\n";
		std::cerr << "Usage: psi-server <address> <port> <threads> <doc_root>\n";
		std::cerr << "  For IPv4, try:\n";
		std::cerr << "    psi-server 0.0.0.0 3000 1 path_to_psi-toolkit/server/website\n";
		std::cerr << "  For IPv6, try:\n";
		std::cerr << "    psi-server 0::0 3000 1 path_to_psi-toolkit/server/website\n";
      return 1;
    }

    PsiServer srv(argv[1], argv[2], argv[3], argv[4]);
	std::cerr << srv.info();
	IndexSite index(srv);
	PipeSite pipe(srv);

	srv.run();
  }
  catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

