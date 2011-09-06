#include "psi_request_handler.hpp"

#include <boost/lexical_cast.hpp>

void PsiRequestHandler::setPsiServer(PsiServer* psi_server) { 
	psi_server_ = psi_server;
}

void PsiRequestHandler::handle_request(
	const http::server3::request& req, http::server3::reply& rep
) {
	http::server3::request req_modified = req;
		
	psi_server_->checkForAction(req_modified, rep);
		
	http::server3::request_handler::handle_request(req_modified, rep);
		
	psi_server_->include(rep.content);
		
	rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
	rep.headers[1].value = "text/html;charset=UTF-8"; 
}

