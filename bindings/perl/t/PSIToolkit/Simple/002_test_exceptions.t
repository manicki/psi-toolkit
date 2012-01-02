#!/usr/bin/perl

use Test::More;

use strict;
use warnings;
use utf8;

use Data::Dumper;

BEGIN { use_ok( 'PSIToolkit::Simple' ); }
require_ok( 'PSIToolkit::Simple' );

use Encode;
use Carp;
use English;
# ===================
# Initialization

# ====================================
# Main tests

binmode(STDOUT, ":utf8");
binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");

_test_excpetions_unknown_pipe_command();
_test_excpetions_bad_arguments();

END:
done_testing();


# =====================================
# Helper functions

sub _test_excpetions_unknown_pipe_command {
    my $command = "tp-tokenizer --lang pl ! unknown-perl-simple-writer --tag token";

    my $text_to_process = 'Ala ma kota.';
    my $expected_exception = "unknown processor `unknown-perl-simple-writer`";

    _run_test_on_command_run_for_perl_with_exception(
        $command, $text_to_process, $expected_exception);
}

sub _test_excpetions_bad_arguments {
    my $command = "tp-tokenizer --unknown-option pl ! perl-simple-writer --tag token";
    my $text_to_process = 'Ala ma kota.';

    my $expected_exception = "unknown exception";
    _run_test_on_command_run_for_perl_with_exception(
        $command, $text_to_process, $expected_exception);
}

sub _run_test_on_command_run_for_perl_with_exception {
    my ($command, $text_to_process, $expected_exception) = @_;

    my $msg = "PipeRunner::run_for_perl($text_to_process) for command($command)";

    eval {
        my $runner = PSIToolkit::Simple::PipeRunner->new($command);
        my $actual_result = $runner->run_for_perl($text_to_process);
    };

    if ($EVAL_ERROR) {
        my $actual_exception = $EVAL_ERROR;
        $actual_exception =~ s{ at .*$}{}m;
        $actual_exception =~ s{\n}{}g;
        is($actual_exception, $expected_exception, $msg);
    } else {
        ok(0, $msg . " exception ($expected_exception) should be thrown");
    }
}


1;
