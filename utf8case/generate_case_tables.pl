#!/usr/bin/perl

# Based on ideas from re2 library.

use strict;
use LWP::Simple;
use String::Util qw(hascontent);
use Data::Dumper;
use Clone qw(clone);

my $UNIDATA_PREFIX= q{http://unicode.org/Public/UNIDATA/};
my $OUTPUT_CPP_FILE = 'case_tables.cpp';

my @lower_case_ranges;
my @upper_case_ranges;
my @title_case_ranges;

my @lower_special_casing;
my @upper_special_casing;
my @title_special_casing;

open my $output_cpp_fh, '>', $OUTPUT_CPP_FILE;
generate_intro();
generate_standard_case_tables();
generate_special_casing_tables();

sub generate_intro {
    print $output_cpp_fh <<'END_OF_INTRO';
// GENERATED AUTOMATICALLY BY generate_case_tables.pl; DO NOT EDIT.

#include "case_tables.hpp"

END_OF_INTRO
}

sub generate_standard_case_tables {
    my @unicode_data_lines = download_unidata_file('UnicodeData.txt');

    for my $line (@unicode_data_lines) {
        append_to_case_ranges(\@upper_case_ranges, $line->[0], $line->[12]);
        append_to_case_ranges(\@lower_case_ranges, $line->[0], $line->[13]);
        append_to_case_ranges(\@title_case_ranges, $line->[0], $line->[14]);
    }

    @lower_case_ranges = compactify(\@lower_case_ranges);
    @upper_case_ranges = compactify(\@upper_case_ranges);
    @title_case_ranges = compactify(\@title_case_ranges);

    write_case_table('lower_case_ranges', \@lower_case_ranges);
    print "\n";

    write_case_table('upper_case_ranges', \@upper_case_ranges);
    print "\n";

    write_case_table('title_case_ranges', \@title_case_ranges);
    print "\n";
}

sub generate_special_casing_tables {
    my @special_casing_lines = download_unidata_file('SpecialCasing.txt');

    for my $line (@special_casing_lines) {
        if (hascontent($line->[4])) {
            print STDERR "This cannot be handled: ", join('; ', @{$line}),"\n";
        } else {
            append_to_special_casing_table(\@lower_special_casing, $line->[0], $line->[1]);
            append_to_special_casing_table(\@title_special_casing, $line->[0], $line->[2]);
            append_to_special_casing_table(\@upper_special_casing, $line->[0], $line->[3]);
        }
    }

    write_special_casing_table('lower_special_casing', \@lower_special_casing);
    print "\n";

    write_special_casing_table('title_special_casing', \@title_special_casing);
    print "\n";

    write_special_casing_table('upper_special_casing', \@upper_special_casing);
}

sub download_unidata_file {
    my ($file_name) = @_;

    my $url = $UNIDATA_PREFIX . $file_name;

    print STDERR "Downloading ${url}...\n";

    my $contents = get($url);

    return map { [ split/\s*;\s*/ ] }
           grep { /\S/ }
           map{ s/\#.*\Z//; $_}
           split/\r?\n/, $contents;
}

sub append_to_case_ranges {
    my ($case_ranges_ref, $hex_code_point, $hex_modified_code_point) = @_;

    if (!hascontent($hex_modified_code_point)) {
        return;
    }

    my $code_point = hex($hex_code_point);
    my $modified_code_point = hex($hex_modified_code_point);

    push @{$case_ranges_ref},
        [ $code_point, $code_point, delta($code_point, $modified_code_point) ];
}

sub compactify {
    my ($case_ranges_ref) = @_;

    my @new_table;

    my $current_compact_range;

    for my $range (@{$case_ranges_ref}) {
        if (!defined($current_compact_range)) {
            $current_compact_range = clone($range);
        } elsif ($range->[2] eq $current_compact_range->[2]
            && $range->[0] == $current_compact_range->[1] + 1) {
            ++$current_compact_range->[1];
        } elsif ($range->[2] eq de_skip($current_compact_range->[2])
            && $range->[0] == $current_compact_range->[1] + 2) {
            $current_compact_range->[1] += 2;
            $current_compact_range->[2] = add_skip($current_compact_range->[2]);
        } else {
            push @new_table, $current_compact_range;
            $current_compact_range = clone($range);
        }
    }

    push @new_table, $current_compact_range;

    return @new_table;
}

sub write_case_table {
    my ($name, $case_ranges_ref) = @_;

    my $table_name = uc($name);
    my $size_constant_name = $table_name . "_SIZE";
    my $table_size = $#{$case_ranges_ref} + 1;


    print $output_cpp_fh <<"END_OF_INTRO";
const size_t $size_constant_name = $table_size;
const CaseConversionRecord ${table_name}[$size_constant_name] = {
END_OF_INTRO

    my $string_to_prepend = '';

    for my $range (@{$case_ranges_ref}) {
        my $from = $range->[0];
        my $to = $range->[1];
        my $delta = $range->[2];

        print $output_cpp_fh "${string_to_prepend}    {$from, $to, $delta}";

        $string_to_prepend = ",\n";
    }


    print $output_cpp_fh "\n};\n";
}

sub append_to_special_casing_table {
    my ($special_casing_table_ref, $hex_code_point, $hex_code_point_vector) = @_;

    if (!hascontent($hex_code_point_vector)) {
        return;
    }

    my $code_point = hex($hex_code_point);
    my @code_point_vector = map { hex($_) } split/\s+/, $hex_code_point_vector;

    push $special_casing_table_ref, [$code_point, cpp_encode(@code_point_vector)];
}

sub write_special_casing_table {
    my ($name, $special_casing_table_ref) = @_;

    my $table_name = uc($name);
    my $size_constant_name = $table_name . "_SIZE";
    my $table_size = $#{$special_casing_table_ref} + 1;

    print $output_cpp_fh <<"END_OF_INTRO";
const size_t $size_constant_name = $table_size;
const SpecialCasingConversionRecord ${table_name}[$size_constant_name] = {
END_OF_INTRO

    my $string_to_prepend = '';

    for my $item (@{$special_casing_table_ref}) {
        my $code_point = $item->[0];
        my $replacement = $item->[1];

        print $output_cpp_fh "${string_to_prepend}    {$code_point, \"$replacement\"}";

        $string_to_prepend = ",\n";
    }

    print $output_cpp_fh "\n};\n";
}

sub cpp_encode {
    my (@v) = @_;

    my $s = join('', map{ chr($_) } @v);

    return join('', map { "\\x$_" } unpack("U0(H2)*", $s));
}

sub de_skip {
    my ($delta) = @_;

    if ($delta =~ /^(EVEN_ODD|ODD_EVEN)(?:_SKIP)?$/) {
        return $1;
    }

    return 'CANNOT_BE_SKIPPED';
}

sub add_skip {
    my ($delta) = @_;

    return de_skip($delta) . '_SKIP';
}


sub delta {
    my ($a, $b) = @_;

    if ($a + 1 == $b) {
        if ($a % 2 == 0) {
            return 'EVEN_ODD'
        }
        else {
            return 'ODD_EVEN';
        }
    } elsif ($a == $b + 1) {
        if ($a % 2 == 0) {
            return 'ODD_EVEN';
        }
        else {
            return 'EVEN_ODD';
        }
    }

    return $b - $a;
}
