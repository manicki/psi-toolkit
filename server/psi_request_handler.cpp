#include "psi_request_handler.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>

void PsiRequestHandler::handle_request(
    const http::server3::request& req, http::server3::reply& rep
) {
    http::server3::request req_modified = req;

    psi_server_->checkForAction(req_modified);

    http::server3::request_handler::handle_request(req_modified, rep);

    psi_server_->include(rep.content);

    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].value = getContentType(req.headers);
}

std::string PsiRequestHandler::getContentType(
    const std::vector<http::server3::header>& headers
) {

    std::string str = headers[2].value;
    size_t found = str.find(',');

    if (found != std::string::npos) {
        str = str.substr(0, found);
    } else {
        str = "text/html";
    }

    str += ";charset=UTF-8";

    return str;
}
