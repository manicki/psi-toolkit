#!/usr/bin/perl

use Test::More;

use strict;
use warnings;
use utf8;

use Data::Dumper;

BEGIN { use_ok( 'PSIToolkit::Simple' ); }
require_ok( 'PSIToolkit::Simple' );

# ===================
# Initialization

# ====================================
# Main tests

binmode(STDOUT, ":utf8");
binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");

#_test_run_pipe_from_string();
#_test_run_pipe_from_string_return_list();

_test_run_pipe_run_for_perl();

END:
done_testing();


# =====================================
# Helper functions


sub _test_run_pipe_from_string {
    my $command = "tp-tokenizer --lang pl";
    my $runner = PSIToolkit::Simple::PipeRunner->new($command);

    my $text_to_process = 'Pan prof. dr hab. Jan Nowak.';
    my $actual_result = $runner->run($text_to_process);
    my $expected_result = <<'ENDEXPECTED';
Pan
prof.
dr
hab.
Jan
Nowak
.
ENDEXPECTED
    is($actual_result, $expected_result, "_test_run_pipe_from_string($text_to_process)");
}

sub _test_run_pipe_from_string_return_list {
    my $command = "tp-tokenizer --lang pl";
    my $runner = PSIToolkit::Simple::PipeRunner->new($command);

    my $text_to_process = 'Pan prof. dr hab. Jan Nowak.';
    my $actual_result = $runner->run_with_array_return($text_to_process);
    my $expected_result = [
        'Pan',
        'prof.',
        'dr',
        'hab.',
        'Jan',
        'Nowak',
        '.',
    ];

    is_deeply($actual_result, $expected_result, "_test_run_pipe_from_string_return_list($text_to_process)");
}

sub _test_run_pipe_run_for_perl {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer";
    my $runner = PSIToolkit::Simple::PipeRunner->new($command);

    my $text_to_process = 'Pan prof. dr hab. Jan Nowak.';
    my $actual_result = $runner->run_for_perl($text_to_process);
    my $expected_result = [
        'Pan',
        'prof.',
        'dr',
        'hab.',
        'Jan',
        'Nowak',
        '.',
    ];

    is_deeply($actual_result, $expected_result, "_test_run_pipe_run_for_perl($text_to_process)");
}

1;
