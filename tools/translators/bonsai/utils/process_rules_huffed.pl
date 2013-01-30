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
my $size = "4G";

my $tempdir = tempdir( CLEANUP => 1 );

my ($rules_h, $rules_file) = tempfile( DIR => $tempdir );

my ($src_h, $src_rule_file) = tempfile( DIR => $tempdir );
my ($src_bytes_h, $src_bytes_file) = tempfile( DIR => $tempdir );
my ($src_byteshuffed_h, $src_byteshuffed_file) = tempfile( DIR => $tempdir );

my ($trg_h, $trg_rule_file) = tempfile( DIR => $tempdir );

my ($src_sym_h, $src_sym_file) = tempfile( DIR => $tempdir );
my ($trg_sym_h, $trg_sym_file) = tempfile( DIR => $tempdir );

my ($log_sv_h, $log_sv) = tempfile( DIR => $tempdir );

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
    s/\s*::\s/\0/g;
    print $rules_h $_;
}

open(my $sorter, "cat $rules_file | sort -t '\\0' -k1,1 -S $size | ");
print STDERR "Resorting and splitting rules in src and trg part\n";

while(<$sorter>) {
    chomp;
    s/[\r\n]+//s;
        
    my ($src_part, $trg_part) = split(/\0/, $_);
    $src_part =~ s/\[\d\]//g;
    $src_part =~ s/\s-->\s/ /;
    
    print $log_sv_h $trg_part, "\n";

    
    my ($trg, $probs, $aln) = $trg_part =~ /^(.+)\s\[(\S+)\]\s\[(\S*)\]$/;
    
    #print $trg, "\n";
    #print $probs, "\n";
    #print $aln, "\n";
    
    my @trg = split(/\s/, $trg);
    my @probs = split(/\,/, $probs);
    my @aln = $aln =~ /(\d+)/g;
    
    foreach my $token (@trg, @probs) {
        if(not exists($trg{$token})) {
            $trg{$token} = scalar keys %trg;
        }
    }
    
    #print STDERR $trg_part, "\n";
    
    print $src_h $src_part."\n";
    
    if($last and $src_part ne $last) {
        my $pos = tell($trg_h);
        print $src_bytes_h "$pos\n";
        
        print $trg_h $_ foreach(@targets);
        @targets = ();
    }
    
    my $targetstr = pack("C", scalar @trg);
    $targetstr .= pack("C", scalar @probs);
    $targetstr .= pack("C", scalar @aln);
    
    print $log_sv_h scalar @trg, " ", scalar @probs, " ", scalar @aln, " ";
    
    foreach my $tok (@trg, @probs) {
        if(not exists($trg{$tok})) {
            die "Missing symbol $tok\n";
        }
        print $log_sv_h $trg{$tok}, " ";
        $targetstr .= pack("I", $trg{$tok});
    }
    foreach my $aln (@aln) {
        print $log_sv_h $aln, " ";
        $targetstr .= pack("C", $aln);
    }
    push(@targets, $targetstr);
    print $log_sv_h "\n";
    
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
    `cp $log_sv $name.sv.txt`;
}

print STDERR "Combining elements to packed StringVector\n";

my ($tempMonotonic_h, $tempMonotonic) = tempfile( DIR => $tempdir );
`./txt2monotonic $tempMonotonic < $src_bytes_file`;

`perl -e 'print pack("C", 0)' > $name.trg.huf`;
`cat $tempMonotonic >> $name.trg.huf`;
`perl -e 'print pack("Q", -s "$trg_rule_file")' >> $name.trg.huf`;
`cat $trg_rule_file >> $name.trg.huf`;

if($copy) {
    `cp $src_bytes_file $name.map.txt`;
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

`./txt2stringvector $name.src.sym < $src_sym_file`;
`./txt2stringvector $name.trg.sym < $trg_sym_file`;

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
