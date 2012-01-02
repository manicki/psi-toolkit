#!/usr/bin/perl

use strict;
use File::Find;
use Cwd;

my $GCOV_OUTPUT_DATA = 'gcov.output.data';
`mkdir "../$GCOV_OUTPUT_DATA"`;

my @files;
my @source_files;
find(\&wanted, '.');

for my $file_name (@files) {
    process_file($file_name);
}

my $args = join(' ', @source_files);

my $psi_toolkit_dir = getcwd();
$psi_toolkit_dir =~ s/\/[^\/]+$//;

`gcov -o "$psi_toolkit_dir/$GCOV_OUTPUT_DATA" $args`;

sub wanted {
    /\.cpp.gcno$/
    && add_file($File::Find::name);
}

sub add_file {
    my ($file_name) = @_;

    push @files, $file_name;
}

sub process_file {
    my ($file_name) = @_;

    if ($file_name =~ /^\.\/(.*)\/CMakeFiles\/.*\/(.*)\.cpp\.gcno$/) {
        my $dir = $1;
        my $base_name = $2;

        push @source_files, "${dir}/${base_name}.cpp";

        `cp "$file_name" "../$GCOV_OUTPUT_DATA/${base_name}.gcno"`;

        my $gcda_file_name = $file_name;
        $gcda_file_name =~ s/\.gcno$/.gcda/;

        if (-r $gcda_file_name) {
            `cp "$gcda_file_name" "../$GCOV_OUTPUT_DATA/${base_name}.gcda"`;
        } else {
            print STDERR "unknown $gcda_file_name\n";
        }
    }

    print "$file_name\n";
}
