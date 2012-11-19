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
my $size = "2G";

my $tempdir = tempdir( CLEANUP => 1 );

my ($rules_h, $rules_file) = tempfile( DIR => $tempdir );

my ($src_h, $src_rule_file) = tempfile( DIR => $tempdir );
my ($src_bytes_h, $src_bytes_file) = tempfile( DIR => $tempdir );
my ($src_byteshuffed_h, $src_byteshuffed_file) = tempfile( DIR => $tempdir );

my ($trg_h, $trg_rule_file) = tempfile( DIR => $tempdir );

my ($src_sym_h, $src_sym_file) = tempfile( DIR => $tempdir );
my ($trg_sym_h, $trg_sym_file) = tempfile( DIR => $tempdir );

#my ($src_sym_h, $src_sym_file) = tempfile( DIR => $tempdir );
#my ($trg_sym_h, $trg_sym_file) = tempfile( DIR => $tempdir );

my %src;
my %trg;

binmode($src_h);
binmode($trg_h);

my $c = 1;

my $last;
my @targets;

print STDERR "Replacing separator in rules\n";

while(<STDIN>) {
    s/\s*::\s*/\0/g;
    print $rules_h $_;
}

open(my $sorter, "cat $rules_file | sort -t '\\0' -k1,1 -S $size | ");
print STDERR "Resorting and splitting rules in src and trg part\n";

while(<$sorter>) {
    chomp;
    s/[\r\n]+//s;
    
    #print STDERR $_."\n";
    
    my ($src_part, $trg_part) = split(/\0/, $_);
    
    $src_part =~ s/\[\d\]//g;
    $src_part =~ s/\s-->\s/ /;
    $trg_part =~ s/\[([^\[\]]*)\]$/join(" ", split(m!,!, $1))/e;
    $trg_part =~ s/\s+/ /g;
    
    print $src_h $src_part."\n";
    
    if($last and $src_part ne $last) {
        my $pos = tell($trg_h);
        print $src_bytes_h "$pos\n";
        
        print $trg_h pack("L" , length(join("", @targets)));
        print $trg_h $_ foreach(@targets);
        @targets = ();
    }
    
    my @tokens = split(/\s+/,$trg_part);
    my $targetstr = pack("C", scalar @tokens);
    foreach my $tok (@tokens) {
        if(not exists($trg{$tok})) {
            $trg{$tok} = (scalar keys %trg) + 1;
        }
        $targetstr .= substr(pack("I", $trg{$tok}), 0, 3);  # I - unsigned integer - first 3 bytes
    }
    push(@targets, $targetstr);
    
    $src{$_} = 1 foreach(split(/\s+/, $src_part));
           
    if($c % 100000 == 0) {
        print STDERR "$c rules split\n";
    }
    
    $last = $src_part;
    $c++;
}

my $pos = tell($trg_h);
print $src_bytes_h "$pos";

my $data = join("", @targets);
print $trg_h $data;

close($src_h);
close($trg_h);

if($copy) {
    `cp $src_rule_file $name.rules.src.txt`;
    `cp $trg_rule_file $name.rules.trg.txt`;
}

print $src_sym_h join("\n", (undef, sort keys %src));
print $trg_sym_h join("\n", (undef, sort { $trg{$a} <=> $trg{$b} } keys %trg));
close($src_sym_h);
close($trg_sym_h);

if($copy) {
    `cp $src_sym_file $name.symbols.src.txt`;
    `cp $trg_sym_file $name.symbols.trg.txt`;
}


print STDERR "Generating symbol maps\n";

`./symbolcompiler $name.src.sym < $src_sym_file`;
`./symbolcompiler $name.trg.sym < $trg_sym_file`;

print STDERR "Huffing\n";

`./huffmanenc --in_src $src_bytes_file --in_trg $trg_rule_file --out_src $src_byteshuffed_file --out_trg $name.trg.huf`;


print STDERR "Creating source to target map\n";

`./arraycompiler -i $src_byteshuffed_file -o $name.map`;

if($copy) {
    `cp $src_bytes_file $name.src.bytes`;
    `cp $src_byteshuffed_file $name.src.byteshuffed`;
}


print STDERR "Mapping rules symbols to integers\n";

my (undef, $src_nums) = tempfile( DIR => $tempdir );
my (undef, $trg_nums) = tempfile( DIR => $tempdir );

print STDERR "Mapping source rule symbols to integers\n";
`cat $src_rule_file | uniq | ./string2nums.pl --symbol_list $src_sym_file > $src_nums`;

if($copy) {
    `cp $src_nums $name.src.nums`;
}

my ($src_fsa_txt_h, $src_fsa_txt_file) = tempfile( DIR => $tempdir );

print STDERR "Minimizing and sorting source rule automaton\n";
`cat $src_nums | ./minfsa | sort -k1,1n -k3,3n -S $size > $src_fsa_txt_file`;

print STDERR "Binarizing and resorting automaton by access counts\n";
`cat $src_fsa_txt_file | ./binfsa -o $name.src.idx`;

if($copy) {
    `cp $src_fsa_txt_file $name.src.fsa.txt`;
}
