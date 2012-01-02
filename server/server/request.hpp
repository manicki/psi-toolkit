// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_REQUEST_HPP
#define HTTP_SERVER3_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
namespace server3 {

/// A request received from a client.
struct request
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;
    std::string post_data;

    std::string get_header_value(const char * str) {
        for (unsigned int i = headers.size(); i > 0; i--) {
            if (headers[i-1].name == str) {
                return headers[i-1].value;
            }
        }
        return std::string("");
    }
};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_REQUEST_HPP
