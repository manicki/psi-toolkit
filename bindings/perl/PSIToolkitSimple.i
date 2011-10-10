%module "PSIToolkit::Simple"

%{
  #include "pipe_runner.hpp"
%}

%include "std_string.i"

class PipeRunner {
 public:
  explicit PipeRunner(const std::string& pipeline);
  int run(const std::string& in, const std::string&);
};


