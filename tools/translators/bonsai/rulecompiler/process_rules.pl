#!/usr/bin/perl

use strict;
use Getopt::Long;
use File::Temp qw(tempfile tempdir);

my $name = "rules";
my $copy = 0;

GetOptions(
    "prefix=s" => \$name,
    "copy" => \$copy,
);

$ENV{LC_ALL} = "C";

my $tempdir = tempdir( CLEANUP => 1 );
my ($src_h, $src_rule_file) = tempfile( DIR => $tempdir );
my ($trg_h, $trg_rule_file) = tempfile( DIR => $tempdir );

my ($src_sym_h, $src_sym_file) = tempfile( DIR => $tempdir );
my ($trg_sym_h, $trg_sym_file) = tempfile( DIR => $tempdir );

my %src;
my %trg;

binmode(STDIN, ":utf8");
binmode($src_h, ":utf8");
binmode($trg_h, ":utf8");
binmode($src_sym_h, ":utf8");
binmode($trg_sym_h, ":utf8");

my $c = 1;
print STDERR "Splitting rules in src and trg part\n";
while(<STDIN>) {
    chomp;
    my ($src_part, $trg_part) = split(/\s+::\s+/, $_);
    
    $src_part =~ s/\[\d\]//g;
    $src_part =~ s/\s-->\s/ /;
    $trg_part =~ s/\[([^\[\]]*)\]$/join(" ", split(m!,!, $1))/e;
    $trg_part =~ s/\s+/ /g;
    
    $src{$_} = 1 foreach(split(/\s+/, $src_part));
    $trg{$_} = 1 foreach(split(/\s+/, $trg_part));
           
    print $src_h $src_part."\n";
    print $trg_h $trg_part."\n";
    
    if($c % 100000 == 0) {
        print STDERR "$c rules splitted\n";
    }
    $c++;
}
close($src_h);
close($trg_h);

if($copy) {
    `cp $src_rule_file $name.rules.src.txt`;
    `cp $trg_rule_file $name.rules.trg.txt`;
}


print $src_sym_h join("\n", (undef, sort keys %src));
print $trg_sym_h join("\n", (undef, sort keys %trg));
close($src_sym_h);
close($trg_sym_h);

if($copy) {
    `cp $src_sym_file $name.symbols.src.txt`;
    `cp $trg_sym_file $name.symbols.trg.txt`;
}

print STDERR "Generating symbol maps\n";

`./symbolcompiler $name.symbols.src < $src_sym_file`;
`./symbolcompiler $name.symbols.trg < $trg_sym_file`;

print STDERR "Mapping rules symbols to integers\n";

my (undef, $src_nums) = tempfile( DIR => $tempdir );
my (undef, $trg_nums) = tempfile( DIR => $tempdir );

my $size = "2G";
print STDERR "Mapping source rule symbols to integers\n";
`cat $src_rule_file | sort -S $size | uniq | ./string2nums.pl --symbol_list $src_sym_file > $src_nums`;
print STDERR "Mapping target rule symbols to integers\n";
`cat $trg_rule_file | sort -S $size | uniq | ./string2nums.pl --symbol_list $trg_sym_file > $trg_nums`;

if($copy) {
    `cp $src_nums $name.src.nums`;
    `cp $trg_nums $name.trg.nums`;
}

my ($src_fsa_txt_h, $src_fsa_txt_file) = tempfile( DIR => $tempdir );
my ($trg_fsa_txt_h, $trg_fsa_txt_file) = tempfile( DIR => $tempdir );

print STDERR "Minimizing and sorting source rule automaton\n";
`cat $src_nums | ./minfsa | sort -k1,1n -k3,3n -S $size > $src_fsa_txt_file`;

print STDERR "Binarizing and resorting automaton by access counts\n";
`cat $src_fsa_txt_file | ./binfsa -o $name.src.idx`;

print STDERR "Minimizing and sorting target rule automaton\n";
`cat $trg_nums | ./minfsa | sort -k1,1n -k3,3n -S $size > $trg_fsa_txt_file`;

print STDERR "Binarizing and resorting automaton by access counts\n";
`cat $trg_fsa_txt_file | ./binfsa -o $name.trg.idx`;

if($copy) {
    `cp $src_fsa_txt_file $name.src.fsa.txt`;
    `cp $src_fsa_txt_file $name.trg.fsa.txt`;    
}

open(my $kn, "cat $src_rule_file | ./string2nums.pl --symbol_list $src_sym_file | ./hasher --dag $name.src.idx |") or die "Error";
open(my $rn, "cat $trg_rule_file | ./string2nums.pl --symbol_list $trg_sym_file | ./hasher --dag $name.trg.idx |") or die "Error";

`rm -f $name.map`;
open(my $pa, "| ./rulemapcompile --out $name.map --mapped --size 700000000");
while(1) {
    my $s = <$kn>;
    my $t = <$rn>;
    last if(not ($s and $t));
    chomp($s,$t);
    print $pa "$s $t\n";
    #print $pa "$k $r\n"
    
}
