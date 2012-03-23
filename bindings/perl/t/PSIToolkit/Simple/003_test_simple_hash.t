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

_test_run_pipe_run_for_perl_with_args_tokenizer();
# @ignore (compilation with java is needed)
_test_run_pipe_run_for_perl_with_args_morfologik();

END:
done_testing();


# =====================================
# Helper functions

sub _test_run_pipe_run_for_perl_with_args_tokenizer {
    my $command = "tp-tokenizer --lang pl ! perl-simple-writer --with-args --tag token";
    my $text_to_process = 'Ala ma kota.';
    my $expected_result = [
        {
            'text' => 'Ala',
            'category' => 'T',
            'values' => {}
        },
        {
            'text' => 'ma',
            'category' => 'T',
            'values' => {}
        },
        {
            'text' => 'kota',
            'category' => 'T',
            'values' => {}
        },
        {
            'text' => '.',
            'category' => 'I',
            'values' => {}
        }
    ];
    _run_test_on_command_run_for_perl($command, $text_to_process, $expected_result);
}

sub _test_run_pipe_run_for_perl_with_args_morfologik {
    my $command = "tp-tokenizer --lang pl ! morfologik ! perl-simple-writer --with-args --tag form";
    my $text_to_process = 'Ala ma kota.';
    my $expected_result = [
        [
            {
                'text' => 'Ala',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'acc',
                    'gender' => 'm1'
                }
            },
            {
                'text' => 'Ala',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'gen',
                    'gender' => 'm1'
                }
            },
            {
                'text' => 'Ala',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'nom',
                    'gender' => 'f'
                }
            }
        ],
        [

            {
                'text' => 'ma',
                'category' => 'verb',
                'values' => {
                    'tense' => 'fin',
                    'number' => 'sg',
                    'person' => 'ter',
                    'aspect' => 'imperf'
                }
            },
            {
                'text' => 'ma',
                'category' => 'adj',
                'values' => {
                    'number' => 'sg',
                    'degree' => 'pos',
                    'case' => 'nom',
                    'gender' => 'f'
                }
            },
            {
                'text' => 'ma',
                'category' => 'adj',
                'values' => {
                    'number' => 'sg',
                    'degree' => 'pos',
                    'case' => 'voc',
                    'gender' => 'f'
                }
            }
        ],
        [

            {
                'text' => 'kota',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'acc',
                    'gender' => 'm2'
                }
            },
            {
                'text' => 'kota',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'gen',
                    'gender' => 'm2'
                }
            },
            {
                'text' => 'kota',
                'category' => 'subst',
                'values' => {
                    'number' => 'sg',
                    'case' => 'nom',
                    'gender' => 'f'
                }
            }
        ]
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
