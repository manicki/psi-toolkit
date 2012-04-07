#!/usr/bin/perl

use Test::More;

use strict;
use warnings;
use utf8;

use Data::Dumper;
use JSON;
use Encode;

BEGIN { use_ok( 'PSIToolkit::Simple' ); }
require_ok( 'PSIToolkit::Simple' );

# ===================
# Initialization

my $show_output_for_debug = 0;
# ====================================
# Main tests

binmode(STDOUT, ":utf8");
binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");

_start_simple_json_test();

_start_json_test_with_polish_letters();
_start_json_test_with_simple_tokenized_hash();

_start_json_test_with_alternatives();

_start_json_test_nasty_examples();

# @ignore (compilation with java is needed)
#_start_json_test_with_morfologik_hash();

END:
done_testing();


# =====================================
# Helper functions

sub _start_simple_json_test {
    _run_json_test("tp-tokenizer --lang pl", "ą");
    _run_json_test("tp-tokenizer --lang pl", "Ala ma kota");
    _run_json_test("tp-tokenizer --lang pl", "Ala ma \" \\ kota");

    _run_json_test("srx-segmenter --lang pl", "Ala ma kota. Kot jest dobry.", "--tag segment");
}

sub _start_json_test_with_polish_letters {
    _run_json_test("tp-tokenizer --lang pl", "ążśźęćółń ĄŻŚŹĘĆÓŁŃ 123456789 ! @ # $ % ^ & * ( ) [ ]");
    _run_json_test("srx-segmenter --lang pl", "Ala ma kota. Kot ma Alę.", "--tag segment");
}

sub _start_json_test_with_simple_tokenized_hash {
    _run_json_test_as_with_args("tp-tokenizer --lang pl", "Ala ma kota.", "--tag token");
    _run_json_test_as_with_args("tp-tokenizer --lang pl", "Chrząszcz będzie śmiesznym prezentem w Łodzi!!!", "--tag token");
}

sub _start_json_test_with_alternatives {
    _run_json_test("tp-tokenizer --lang pl", "ąż ółń", "--tag symbol --spec token");
}

sub _start_json_test_nasty_examples {
    _run_json_test("tp-tokenizer --lang pl", "");
    _run_json_test("tp-tokenizer --lang pl", "", "--tag symbol --spec token");
}

sub _start_json_test_with_morfologik_hash {
    _run_json_test_as_with_args("tp-tokenizer --lang pl ! morfologik", "Ala ma kota.", "--tag form");
}

sub _run_json_test_as_with_args {
    my ($pipe_command, $input_text, $writer_params) = @_;

    if (not $writer_params) {
        $writer_params = '';
    }

    $writer_params .= " --with-args";
    _run_json_test($pipe_command, $input_text, $writer_params);
}

sub _run_json_test {
    my ($pipe_command, $input_text, $writer_params) = @_;

    if ($writer_params) {
        $writer_params = ' ' . $writer_params;
    } else {
        $writer_params = '';
    }

    my $msg = "_run_json_test('$pipe_command', '$input_text')";

    my $full_json_command = $pipe_command . ' ! json-simple-writer' . $writer_params;
    my $full_perl_command = $pipe_command . ' ! perl-simple-writer' . $writer_params;

    my $actual_perl_result = _run_on_pipe_runner_run_for_perl($full_perl_command, $input_text);
    my $actual_json_result = _run_on_pipe_runner($full_json_command, $input_text);

    if ($show_output_for_debug) {
        print STDERR "Perl: " . Dumper($actual_perl_result) . "\n";
        print STDERR "JSON: " . Dumper($actual_json_result) . "\n";
        print STDERR "JSON(TEXT): " . $actual_json_result . "\n";
    }
    my $actual_result = _parse_json($actual_json_result);

    is_deeply($actual_result, $actual_perl_result, $msg);
}

sub _run_on_pipe_runner {
    my ($command, $input_text) = @_;

    my $runner = PSIToolkit::Simple::PipeRunner->new($command);
    my $actual_result = $runner->run($input_text);

    return $actual_result;
}

sub _run_on_pipe_runner_run_for_perl {
    my ($command, $input_text) = @_;

    my $runner = PSIToolkit::Simple::PipeRunner->new($command);
    my $actual_result = $runner->run_for_perl($input_text);

    return $actual_result;
}

sub _parse_json {
    my ($actual_json_result) = @_;

    my $json_bytes = encode('UTF-8', $actual_json_result);

    my $json = new JSON;
    my $actual_result = $json
        ->allow_nonref
            ->utf8
                ->relaxed
                    ->decode($json_bytes);
    return $actual_result;
}

1;
