#!/usr/bin/perl

use strict;
use warnings;
use utf8;

use TAP::Harness;
use TAP::Formatter::JUnit;

my $TESTS_DIRECTORY = 't';

my $fixed_test_regexp;

my $run_directory = $ARGV[0];
my $current_tests_dir = $run_directory . '/' . $TESTS_DIRECTORY;
# =====================

main();

# =====================

sub main {
    my $tests_array = _find_all_tests();
    _run_tests($tests_array);
}

# =======================

sub _find_all_tests {
    my $tests_array = [];

    for my $fp (split /\n/, `find $current_tests_dir -regex '.*[.]t'`) {
        next if $fp !~ /[.]t$/;

        if ($fixed_test_regexp) {
            next if not $fp =~ $fixed_test_regexp;
        }
        push @$tests_array, $fp;
    }

    @$tests_array = sort @$tests_array;
    return $tests_array;
}

sub _run_tests {
    my ($tests_array) = @_;

    my $harness = TAP::Harness->new( {
        verbosity => 'verbose',
        formatter_class => 'TAP::Formatter::JUnit',
    } );


    $harness->runtests(@$tests_array);
}

1;
