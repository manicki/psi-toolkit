// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_MIME_TYPES_HPP
#define HTTP_SERVER3_MIME_TYPES_HPP

#include <string>

namespace http {
namespace server3 {
namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_MIME_TYPES_HPP
