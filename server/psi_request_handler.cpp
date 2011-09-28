#include "psi_request_handler.hpp"
#include "session_manager.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

std::map<std::string, std::string> PsiRequestHandler::fileContentTypes =
    boost::assign::map_list_of
        ("htm", "text/html")
        ("html", "text/html")
        ("shtml", "text/html")
        ("css", "text/css")

        ("js", "application/javascript")
        ("pdf", "application/pdf")
        ("ps", "application/postscript")
        ("eps", "application/postscript")
        ("ai", "application/postscript")
        ("doc", "application/msword")
        ("xdoc", "application/msword")
        ("rtf", "application/rtf")
        ("zip", "application/zip")

        ("jpg", "image/jpeg")
        ("jpeg", "image/jpeg")
        ("jpe", "image/jpeg")
        ("png", "image/png")
        ("gif", "image/gif")
        ("tiff", "image/tiff")
        ("tiff", "image/tif");

void PsiRequestHandler::handle_request(
    const http::server3::request& req, http::server3::reply& rep
) {
    http::server3::request req_modified = req;

    // session management
    SessionManager * sesMng = SessionManager::Instance();
    std::string id = SessionManager::SessionId(req_modified.get_header_value("Cookie"));

    bool hasId = sesMng->isSession(id);

    Session * session;
    if (hasId) {
        session = sesMng->getSession(id);
    } else {
        session = sesMng->newSession();
    }

    psi_server_->checkForAction(req_modified);
    http::server3::request_handler::handle_request(req_modified, rep);
    psi_server_->include(rep.content);

    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].value = getContentType(req.uri);

    //set cookie with session id for browser
    if (!hasId) {
        rep.headers.push_back(createCookieHeader(session->getId()));
    }

    //debug
    for (unsigned int i = 0; i < rep.headers.size(); i++) {
        DEBUG(i << ": " << rep.headers[i].name << " => " << rep.headers[i].value);
    }
    DEBUG("--------------");

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

http::server3::header PsiRequestHandler::createCookieHeader(std::string id) {
    http::server3::header cookie;
    cookie.name = "Set-Cookie";
    cookie.value = SessionManager::CookieHeaderValue(id);

    return cookie;
}
