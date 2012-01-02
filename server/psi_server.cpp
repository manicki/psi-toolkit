#include "psi_server.hpp"
#include "mpfd-parser/Parser.h"
#include "logging.hpp"
#include "session_manager.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

typedef std::multimap <std::string, std::string> psis_mmp_name_value;
typedef std::multimap <std::string, std::string>::iterator psis_iter_name_value;

PsiServer::PsiServer(
    const std::string& address, const std::string& port,
    const std::string& threads, const std::string& doc_root)
    : websiteRoot(doc_root),
    port_(port), psi_request_handler_(doc_root, this),
    server_(address, port, psi_request_handler_, boost::lexical_cast<std::size_t>(threads)) { }

std::string& PsiServer::info() {
    static std::string inf = "PSI SERVER version 0.1\n"
        "Simple multithreading web server based on Boost Asio server3 and C++ WEBEM\n"
        "Point browser at http://"+ boost::asio::ip::host_name() + ":"
        + boost::lexical_cast<std::string>(port_) + "/\n\n";
    return inf;
}

void PsiServer::run(){
    // Block all signals for background thread.
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigset_t old_mask;
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

    // Run server in background thread.
    this->info();
    boost::thread t(boost::bind(&http::server3::server::run, &server_));

    // Restore previous signals.
    pthread_sigmask(SIG_SETMASK, &old_mask, 0);

    // Wait for signal indicating time to shut down.
    sigset_t wait_mask;
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGINT);
    sigaddset(&wait_mask, SIGQUIT);
    sigaddset(&wait_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
    int sig = 0;
    sigwait(&wait_mask, &sig);

    // Stop the server.
    server_.stop();
    t.join();
}

void PsiServer::registerIncludeCode(const char* idname, psis_include_function fun) {
    includes_.insert(std::pair<std::string, psis_include_function >(
        std::string(idname), fun)
    );
}

void PsiServer::registerActionCode(const char* idname, psis_action_function fun) {
    actions_.insert(std::pair<std::string, psis_action_function >(
        std::string(idname), fun)
    );
}

void PsiServer::include(std::string& reply) {
    int p = 0;
    while (1) {
        // find next request for generated text
        p = reply.find("<!--#psis", p);
            if (p == -1) { break; }

        int q = reply.find("-->", p);
            if (q == -1) { break; }
        q += 3;
        int reply_len = reply.length();

        // code identifying text generator
        std::string code = reply.substr(p + 10, q - p - 14);

        // delete psis markup
        reply.replace(p, q - p, "");

        // find the function associated with this code
        std::map<std::string, psis_include_function>::iterator pf = includes_.find(code);
        if (pf != includes_.end()) {
            reply.insert(p, pf->second());
        }

        // adjust pointer into text for insertion
        p = q + reply.length() - reply_len;
    }
}

void PsiServer::checkForAction(http::server3::request& req) {

    // look for psis form action request
    std::string uri = req.uri;

    int q = 0;
    if (req.method != "POST") {
        q = uri.find(".psis?");
        if (q == -1) return;
    } else {
        q = uri.find(".psis");
        if (q == -1) return;
    }

    // find function matching action code
    std::string code = uri.substr(1, q-1);
    std::map < std::string, psis_action_function >::iterator pfun = actions_.find(code);
    if (pfun == actions_.end()) {
        return;
    }

    // decode the values
    if (req.method == "POST") {
        uri = req.post_data;
        q = 0;
    } else {
        q += 6;
    }

    name_values_.clear();
    if (req.get_header_value("Content-Type").find("multipart/form-data;") == 0) {
        parseMultipartFormData(req);
    }
    else {
        parseNamesAndValues(uri, q);
    }

    // call the function
    req.uri = pfun->second(this);
    return;
}

void PsiServer::parseMultipartFormData(http::server3::request& req) {
    try {
        MPFD::Parser parser;

        parser.SetTempDirForFileUpload("/tmp");
        parser.SetUploadedFilesStorage(MPFD::Parser::StoreUploadedFilesInMemory);
        parser.SetContentType(req.get_header_value("Content-Type"));

        parser.AcceptSomeData(req.post_data.c_str(), req.post_data.size());

        std::map<std::string, MPFD::Field *> fields = parser.GetFieldsMap();
        std::map<std::string, MPFD::Field *>::iterator it;

        for (it = fields.begin(); it != fields.end(); ++it) {
            if (it->second->GetType() == MPFD::Field::FileType) {

                // add file name
                addDataToCurrentSession(
                    it->first + std::string("-filename"), it->second->GetFileName()
                );

                // add file content
                std::size_t size = it->second->GetFileContentSize();
                addDataToCurrentSession(it->first, std::string(it->second->GetFileContent(), size));
            }
            else {
                addDataToCurrentSession(it->first, it->second->GetTextTypeContent());
            }
        }

    } catch (MPFD::Exception & e) {
        ERROR(e.GetError());
    }
}

void PsiServer::parseNamesAndValues(std::string uri, int q) {
    std::string name;
    std::string value;

    int p = q;
    int flag_done = 0;

    while (!flag_done) {
        q = uri.find("=", p);
        name = uri.substr(p, q-p);
        p = q + 1;
        q = uri.find("&", p);

        if (q != -1) {
            value = uri.substr(p, q-p);
        } else {
            value = uri.substr(p);
            flag_done = 1;
        }

        // the browser sends blanks as +
        while (1) {
            int p = value.find("+");
            if (p == -1) break;
            value.replace(p, 1, " ");
        }

        addDataToCurrentSession(name, urlDecode(value));
        //ame_values_.insert(std::pair<std::string, std::string> (name, urlDecode(value)));
        p = q + 1;
    }
}

void PsiServer::addDataToCurrentSession(std::string key, std::string value) {
    SessionManager::Instance()->currentSession()->setData(key, value);
}

std::string PsiServer::urlDecode(std::string & encodedString) {
    std::string decodedString;

    char ch;
    unsigned int i;
    int j;

    for (i = 0; i < encodedString.length(); i++) {

        if (int(encodedString[i]) == 37) {
            sscanf(encodedString.substr(i+1, 2).c_str(), "%2x", &j);

            ch = static_cast<char>(j);
            decodedString += ch;
            i = i + 2;

        } else {
            decodedString += encodedString[i];
        }
    }

    return (decodedString);
}

Session * PsiServer::session() {
    return SessionManager::Instance()->currentSession();
}
