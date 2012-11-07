#include "version_information.hpp"
#include "git_info.hpp"

std::string get_psi_toolkit_version_string() {
    return
        std::string("psi-toolkit v. ")
        + (std::string("GIT-NOTFOUND") != g_GIT_LAST_DATE
           ? std::string(g_GIT_LAST_DATE) + " [" + g_GIT_SHA1 + ']'
           : std::string(g_GIT_SHA1));
}

std::string get_psi_toolkit_last_date_string() {
    return (std::string("GIT-NOTFOUND") != g_GIT_LAST_DATE ? std::string(g_GIT_LAST_DATE) : "");
}

std::string get_psi_toolkit_last_commit_string() {
    return std::string(g_GIT_SHA1);
}
