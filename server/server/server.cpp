// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

#include <iostream>

#include "logging.hpp"

namespace http {
namespace server3 {

server::server(const std::string& address, const std::string& port,
    request_handler& user_request_handler, std::size_t thread_pool_size)
  : thread_pool_size_(thread_pool_size),
    acceptor_(io_service_),
    request_handler_(user_request_handler),
    new_connection_(new connection(io_service_, request_handler_))
{
  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query(address, port);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
  acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&server::handle_accept, this,
        boost::asio::placeholders::error));
}

void server::run()
{
  INFO("Starting server...");

  // Create a pool of threads to run all of the io_services.
  std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    boost::shared_ptr<boost::thread> thread(new boost::thread(
          boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }

  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i)
  {
    threads[i]->join();
  }
  
  INFO("Stopped!");
}

void server::stop()
{
  INFO("Stopping server...");
  io_service_.stop();
}

void server::handle_accept(const boost::system::error_code& e)
{
  if (!e)
  {
    new_connection_->start();
    new_connection_.reset(new connection(io_service_, request_handler_));
    acceptor_.async_accept(new_connection_->socket(),
        boost::bind(&server::handle_accept, this,
          boost::asio::placeholders::error));
  }
}

} // namespace server3
} // namespace http
