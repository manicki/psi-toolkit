#include "psi_server.hpp"

#ifndef PSI_SERVER_HANDLER_HDR
#define PSI_SERVER_HANDLER_HDR

#include "server/request_handler.hpp"

class PsiServer;

class PsiRequestHandler : public http::server3::request_handler
{
public:
    /// Construct with a directory containing files to be served.
    PsiRequestHandler(const std::string& doc_root, PsiServer* psi_server) :
        http::server3::request_handler(doc_root), psi_server_(psi_server) { }

    void handle_request(const http::server3::request& req, http::server3::reply& rep);

private:
    PsiServer* psi_server_;

    static std::map<std::string, std::string> fileContentTypes;
    std::string getContentType(const std::string & uri);
};

#endif
