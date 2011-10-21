%module "PSIToolkit::Simple"

%{
  #include "pipe_runner.hpp"
%}

%include "std_string.i"

class PipeRunner {
 public:
    explicit PipeRunner(const std::string& pipeline);
    int run(const std::string& in, const std::string&);
    std::string run(const std::string & inputString);

    %extend {

        SV * run_with_array_return(const std::string & inputString) {
            AV * resultArray = newAV();

            int number_of_elements = 7;
            std::string elements [] = {
                "Pan", "prof.", "dr", "hab.", "Jan", "Nowak", "."
            };

            for (int i = 0; i < number_of_elements; ++i) {
                SV * element_sv = newSVpv(elements[i].c_str(), 0);
                av_push( resultArray, element_sv);
            }
            
            return newRV_noinc((SV *) resultArray);
        }
    }
};


