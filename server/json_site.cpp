#include <string>
#include <boost/bind.hpp>

#include "json_site.hpp"
#include "pipe_runner.hpp"
#include "session_manager.hpp"

std::string JsonSite::PIPE_PARAMETER = "pipe";
std::string JsonSite::INPUT_PARAMETER = "input";

JsonSite::JsonSite(PsiServer& server) : TemplateSite(server),
    jsonOutputAsString("{}")
{
    psiServer_.registerIncludeCode(
        "json_site_output", boost::bind(&JsonSite::jsonOutput, this));

    psiServer_.registerActionCode(
        "json", boost::bind(&JsonSite::actionJson, this));
}

char * JsonSite::jsonOutput() {
    return stringToChar(jsonOutputAsString);
}

char * JsonSite::actionJson() {
    jsonOutputAsString = "{ ";

    std::string pipe = psiServer_.session()->getData(PIPE_PARAMETER);
    jsonOutputAsString += "\"pipe\": \"" + pipe + "\", ";

    std::string input = psiServer_.session()->getData(INPUT_PARAMETER);
    jsonOutputAsString += "\"input\": \"" + input + "\", ";

    std::stringstream iss(input);
    std::ostringstream oss;

    try {
        PipeRunner p(pipe);
        p.run(iss, oss);

        jsonOutputAsString += "\"output\": \"" + oss.str() + "\", ";
        jsonOutputAsString += "\"error\": null ";
    }
    catch (std::exception& e) {
        oss << "There are some problems: " << e.what()
            << "Check the pipe-line specification and try once again.";

        jsonOutputAsString += "\"output\": null, ";
        jsonOutputAsString += "\"error\": \"" + oss.str() + "\" ";
    }

    jsonOutputAsString += " }";

    return stringToChar(std::string("/json.html"));
}
