#include "psi_request_handler.hpp"
#include "session_manager.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

std::map<std::string, std::string> PsiRequestHandler::filesToContentTypes =
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
        ("tif", "image/tif");

std::map<std::string, std::string> PsiRequestHandler::fileExceptionsToContentTypes =
    boost::assign::map_list_of
        ("json.psis", "text/javascript");

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
    sesMng->setCurrentSession(session->getId());

    DEBUG("Request headers: ");
    for (unsigned int i = 0; i < req_modified.headers.size(); i++) {
        DEBUG(i << ": " << (std::string)req_modified.headers[i].name << " => "
            << (std::string)req_modified.headers[i].value);
    }
    DEBUG("--------------");

    //handle request
    psi_server_->checkForAction(req_modified);
    http::server3::request_handler::handle_request(req_modified, rep);
    psi_server_->include(rep.content);

    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].value = getContentType(req.uri);

    //set cookie with session id for browser
    if (!hasId) {
        rep.headers.push_back(createCookieHeader(session->getId()));
    }

    DEBUG("Answer headers: ");
    for (unsigned int i = 0; i < rep.headers.size(); i++) {
        DEBUG((unsigned long)i << ": " << (std::string)rep.headers[i].name << " => "
            << (std::string)rep.headers[i].value);
    }
    DEBUG("--------------");
}

std::string PsiRequestHandler::getContentType(const std::string & uri) {
    std::string content = "";
    std::map<std::string, std::string>::iterator type;

    std::string file = getFileName(uri);
    type = fileExceptionsToContentTypes.find(file);

    if (type != fileExceptionsToContentTypes.end()) {
        content = type->second;
    }

    if (content.empty()) {
        std::string extension = getFileExtension(file);
        type = filesToContentTypes.find(extension);

        if (type != filesToContentTypes.end()) {
            content = type->second;
        }
    }

    if (content.empty()) {
        content = "text/html";
    }
    content += ";charset=UTF-8";

    return content;
}

std::string PsiRequestHandler::getFileName(const std::string & uri) {
    size_t startFile = uri.find_last_of('/');
    size_t endFile = uri.find_last_of('?');

    std::string file = "";

    if (startFile != std::string::npos) {
        if (endFile != std::string::npos) {
            file = uri.substr(startFile + 1, endFile - startFile - 1);
        }
        else {
            file = uri.substr(startFile + 1);
        }
    }

    return file;
}

std::string PsiRequestHandler::getFileExtension(std::string fileName) {
    size_t startExtension = fileName.find_last_of('.');

    std::string extension = "";

    if (startExtension != std::string::npos) {
        extension = fileName.substr(startExtension + 1);
    }

    return extension;
}

http::server3::header PsiRequestHandler::createCookieHeader(std::string id) {
    http::server3::header cookie;
    cookie.name = "Set-Cookie";
    cookie.value = SessionManager::CookieHeaderValue(id);

    return cookie;
}
