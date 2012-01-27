#!/usr/bin/perl

use Test::More;

use strict;
use warnings;
use utf8;

use Data::Dumper;

BEGIN { use_ok( 'PSIToolkit::Simple' ); }
require_ok( 'PSIToolkit::Simple' );

use Encode;
# ===================
# Initialization

# ====================================
# Main tests

binmode(STDOUT, ":utf8");
binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");

_test_run_pipe_run_for_perl_with_args1();


END:
done_testing();


# =====================================
# Helper functions

sub _test_run_pipe_run_for_perl_with_args1 {
    my $command = "tp-tokenizer --lang pl  ! srx-segmenter --lang pl ! perl-simple-writer --with-args --tag token";
    my $text_to_process = 'Ala ma kota.';
    my $expected_result = [
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _run_test_on_command_run_for_perl {
    my ($command, $text_to_process, $expected_result) = @_;
    my $runner = PSIToolkit::Simple::PipeRunner->new($command);

    my $actual_result = $runner->run_for_perl($text_to_process);

    is_deeply($actual_result, $expected_result, "PipeRunner::run_for_perl($text_to_process) for command($command)");
}


1;
