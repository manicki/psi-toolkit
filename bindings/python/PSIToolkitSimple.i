%module PSIToolkit

%{
  #include "pipe_runner.hpp"
%}

%include "std_string.i"

class PipeRunner {
 public:
  explicit PipeRunner(const std::string& pipeline);
  int run(const std::string& in, const std::string&);
  std::string run(const std::string & inputString);
};


