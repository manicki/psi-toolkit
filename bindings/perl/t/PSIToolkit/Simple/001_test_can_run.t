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

_test_run_pipe_from_string();

_test_run_pipe_run_for_perl();
_test_run_pipe_run_for_perl_with_alternatives1();
_test_run_pipe_run_for_perl_with_alternatives2();

_test_run_pipe_from_string_with_polish_letters();
_test_run_pipe_run_for_perl_with_polish_letters();
_test_run_pipe_run_for_perl_with_polish_letters_with_alternatives();
_test_run_pipe_run_for_perl_with_tokenizer_and_segmenter();
_test_run_pipe_run_for_perl_with_lang_guesser();

# @ignore (compilation with java is needed)
#_test_run_pipe_run_for_perl_with_alternatives_with_morfologik();

_test_run_pipe_run_for_perl_with_alternatives_empty_text();


END:
done_testing();


# =====================================
# Helper functions


sub _test_run_pipe_from_string {
    my $command = "tp-tokenizer --lang pl";
    my $text_to_process = 'Pan prof. dr hab. Jan Nowak.';
    my $expected_result = <<'ENDEXPECTED';
Pan
prof.
dr
hab.
Jan
Nowak
.
ENDEXPECTED

    _run_test_on_command_run($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_from_string_with_polish_letters {
    my $command = "tp-tokenizer --lang pl";
    my $text_to_process = 'ĄŻŚŹĘĆÓŁŃ ążśźęćółń';
    my $expected_result = <<'ENDEXPECTED';
ĄŻŚŹĘĆÓŁŃ
ążśźęćółń
ENDEXPECTED

    _run_test_on_command_run($command, $text_to_process, $expected_result);
}

sub _run_test_on_command_run {
    my ($command, $text_to_process, $expected_result) = @_;
    my $runner = PSIToolkit::Simple::PipeRunner->new($command);

    my $actual_result = $runner->run($text_to_process);

    is($actual_result, $expected_result, "PipeRunner::run($text_to_process) for command($command)");
}

sub _test_run_pipe_run_for_perl {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer";
    my $text_to_process = 'Pan prof. dr hab. Jan Nowak.';
    my $expected_result = [
        'Pan',
        'prof.',
        'dr',
        'hab.',
        'Jan',
        'Nowak',
        '.',
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_polish_letters {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer";
    my $text_to_process = 'ĄŻŚŹĘĆÓŁŃ ążśźęćółń';
    my $expected_result = [
        'ĄŻŚŹĘĆÓŁŃ',
        'ążśźęćółń',
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_polish_letters_with_alternatives {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer --tag symbol --spec token";
    my $text_to_process = 'ąż ółń';
    my $expected_result = [
        [ 'ą', 'ż',],
        [ 'ó', 'ł', 'ń'],
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}


sub _test_run_pipe_run_for_perl_with_alternatives1 {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer --tag symbol --spec token";
    my $text_to_process = 'A';
    my $expected_result = [
        [ 'A' ],
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}


sub _test_run_pipe_run_for_perl_with_alternatives2 {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer --tag symbol --spec token";
    my $text_to_process = 'Ale ile';
    my $expected_result = [
        [ 'A', 'l', 'e' ],
        ['i', 'l', 'e'],
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_alternatives_empty_text {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer --tag symbol --spec token";
    my $text_to_process = '';
    my $expected_result = [];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}


sub _test_run_pipe_run_for_perl_with_alternatives_with_morfologik {
    my $command = "tp-tokenizer --lang pl ! srx-segmenter --lang pl ! morfologik  ! perl-simple-writer --tag lemma ";
    my $text_to_process = 'Ala i kot.';
    my $expected_result = [
        ['Al', 'Ala'],
        'i',
        ['kot', 'kota'],
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_tokenizer_and_segmenter {
    my $command = "tp-tokenizer --lang pl ! srx-segmenter --lang pl ! perl-simple-writer --tag token --spec segment";
    my $text_to_process = 'Ala ma kota. Kot ma mysz.';
    my $expected_result = [
        ['Ala', 'ma', 'kota','.'],
        ['Kot', 'ma', 'mysz','.'],
    ];

    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_lang_guesser {
    my $command = "lang-guesser ! perl-simple-writer --tag text";
    my $text_to_process = 'Źdźbło';
    my $expected_result = [
        'Źdźbło',
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
