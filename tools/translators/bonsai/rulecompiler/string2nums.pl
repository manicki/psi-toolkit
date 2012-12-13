#!/usr/bin/perl

use strict;
use Encode qw(encode_utf8);
use Getopt::Long;

my $symbols = "";
my $verbose = 0;

GetOptions(
    "symbol_list=s" => \$symbols,
    "verbose|v" => \$verbose,
);

my %src;
if($symbols) {
    open(SRC, "<:utf8", $symbols);
    my $i = 0;
    %src = map{ chomp; [split]->[0] => $i++ } <SRC>;
    print STDERR "Loading symbols from '$symbols'\n";
    print STDERR "Loaded " . (scalar keys %src) . " symbols\n";
}

binmode(STDIN, ":utf8");
binmode(STDOUT, ":utf8");
binmode(STDERR, ":utf8");

while(<STDIN>) {
    chomp;
    my @input;
    foreach my $symbol (split(/\s+/,$_)) {
	if(!exists($src{$symbol})) {
	    $src{$symbol} = (scalar keys %src) + 1;
	}
	push(@input, $src{$symbol});
    }
    print "# $_ (" . join(" ", @input) . ")\n" if($verbose);
    print join(" ", @input)."\n";
}
