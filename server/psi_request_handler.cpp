#include "psi_request_handler.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

#include <iostream>

std::map<std::string, std::string> PsiRequestHandler::fileContentTypes =
    boost::assign::map_list_of
        ("htm", "text/html")
        ("html", "text/html")
        ("shtml", "text/html")
        ("css", "text/css")
        ("js", "text/html")
        ("jpg", "image/png")
        ("jpeg", "image/png")
        ("png", "image/png")
        ("gif", "image/png");

void PsiRequestHandler::handle_request(
    const http::server3::request& req, http::server3::reply& rep
) {
    http::server3::request req_modified = req;

    psi_server_->checkForAction(req_modified);

    http::server3::request_handler::handle_request(req_modified, rep);

    psi_server_->include(rep.content);

    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].value = getContentType(req.uri);
}

std::string PsiRequestHandler::getContentType(const std::string & uri) {
    std::string str = "text/html";

    size_t is_extension = uri.find_last_of('.');

    if (is_extension != std::string::npos) {
        std::string extension = uri.substr(is_extension + 1);

        std::map<std::string, std::string>::iterator type;
        type = fileContentTypes.find(extension);

        if (type != fileContentTypes.end()) {
            str = type->second;
        }
    }

    str += ";charset=UTF-8";

    return str;
}
