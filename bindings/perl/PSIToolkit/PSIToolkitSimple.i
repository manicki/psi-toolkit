#if defined(SWIGPERL)
%module "psitoolkit_perl_simple"
#endif

%{
  #include "pipe_runner.hpp"
%}

%include "std_string.i"
%include "exception.i"
%include "std_except.i"

%feature("shadow") PipeRunner::run(const std::string&) %{
    use Encode;

    sub run {
        my ($self, $input_string) = @_;
        
        my $result_string = psitoolkit_perl_simplec::PipeRunner_run($self, $input_string);
        my $decoded = Encode::decode('utf-8', $result_string);
        return $decoded;
    }
%}

%feature("shadow") PipeRunner::run_for_perl(const std::string&) %{
    use Encode;

    sub run_for_perl {
        my ($self, $input_string) = @_;
        
        my $result_array = psitoolkit_perl_simplec::PipeRunner_run_for_perl($self, $input_string);

        my $decoded_array = _decode_array($result_array);
        return $decoded_array;
    }

    sub _decode_array {
        my ($array) = @_;

        if (defined $array) {
            if (ref $array eq 'ARRAY') {
                my $decoded_array = [];
                for my $element (@$array) {
                    my $decoded = _decode_array($element);
                    if (defined $decoded) {
                        push @$decoded_array, $decoded;
                    }
                }
                return $decoded_array;
            } elsif (ref $array eq 'HASH') {
                my $decoded_hash = {};
                for my $key (keys %$array) {
                    my $value = $array->{$key};
                    my $decoded_value = _decode_array($value);
                    if (defined $decoded_value) {
                        $decoded_hash->{$key} = $decoded_value;
                    }
                }
                return $decoded_hash;
            } else {
                return Encode::decode('utf-8', $array);
            }
        } else {
            return;
        }
    }
%}


%exception {
  try {
    $action
  }
  catch(PsiException pe){
    croak(pe.what());
  }
  catch (...) {
    croak("unknown exception");
  }
}

class PipeRunner {
 public:
    explicit PipeRunner(const std::string& pipeline);
    int run(const std::string& in, const std::string&);
    std::string run(const std::string & inputString);

    SV * run_for_perl(const std::string& inputString);
};

class PsiException : public std::exception {
 public:
  PsiException(const std::string& msg);
  virtual const char* what() const throw();
};
