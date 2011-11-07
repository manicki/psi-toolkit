%module "PSIToolkit::Simple"

%{
  #include "pipe_runner.hpp"
%}

%include "std_string.i"

%feature("shadow") PipeRunner::run(const std::string&) %{
    use Encode;

    sub run {
        my ($self, $input_string) = @_;
        
        my $result_string = PSIToolkit::Simplec::PipeRunner_run($self, $input_string);
        my $decoded = Encode::decode('utf-8', $result_string);
        return $decoded;
    }
%}

%feature("shadow") PipeRunner::run_for_perl(const std::string&) %{
    use Encode;

    sub run_for_perl {
        my ($self, $input_string) = @_;
        
        my $result_array = PSIToolkit::Simplec::PipeRunner_run_for_perl($self, $input_string);

        my $decoded_array = _decode_array($result_array);
        return $decoded_array;
    }

    sub _decode_array {
        my ($array) = @_;

        if (defined $array) {
            if (ref $array) {
                my $decoded_array = [];
                for my $element (@$array) {
                    my $decoded = _decode_array($element);
                    if (defined $decoded) {
                        push @$decoded_array, $decoded;
                    }
                }
                return $decoded_array;
            } else {
                return Encode::decode('utf-8', $array);
            }
        } else {
            return;
        }
    }
%}

class PipeRunner {
 public:
    explicit PipeRunner(const std::string& pipeline);
    int run(const std::string& in, const std::string&);
    std::string run(const std::string & inputString);

    SV * run_for_perl(const std::string& inputString);
};

