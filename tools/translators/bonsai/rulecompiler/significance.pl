#!/usr/bin/perl
use strict;
use Getopt::Long;
use File::Temp qw(tempfile tempdir);
use IPC::Open2;
use Data::Dumper;

my $BIN_DIR = ".";
my $SCRIPT_DIR = ".";

my ($RULES, $SRC_CORPUS, $TRG_CORPUS);

GetOptions(
    "bin_dir=s" => \$BIN_DIR,
    "script_dir=s" => \$SCRIPT_DIR,
    "rules=s" => \$RULES,
    "src=s" => \$SRC_CORPUS,
    "trg=s" => \$TRG_CORPUS,
);

$ENV{LC_ALL} = "C";
my $size = "1G";

my $tempdir = tempdir( CLEANUP => 1 );
my (undef, $name) = tempfile( DIR => $tempdir );

my ($src_h, $src_rule_file) = tempfile( DIR => $tempdir );
my ($trg_h, $trg_rule_file) = tempfile( DIR => $tempdir );
my ($src_trg_h, $src_trg_file) = tempfile( DIR => $tempdir );

my %src;
my %trg;

{
    my $c = 0;
    
    open(my $src_sorter, "| sort -S $size > $src_rule_file");
    open(my $trg_sorter, "| sort -S $size > $trg_rule_file");
    
    open(my $rh, "<$RULES") or die;
    
    while(<$rh>) {
        next if(/^#/ or !/\p{L}/);
        chomp;
        s/\t.*$//g;
        s/[\r\n]+//s;
        
        $c++;
        my ($src_part, $trg_part) = split(/\s+::\s+/, $_);
        
        $src_part =~ s/\[\d\]//g;
        $src_part =~ s/\s-->\s/ /;
        
        $trg_part =~ s/\[\d\]//g;
        
        print $src_sorter $src_part, "\n";
        print $trg_sorter $trg_part, "\n";
        
        $src{$_} = 1 foreach(split(/\s+/, $src_part));
        $trg{$_} = 1 foreach(split(/\s+/, $trg_part));
        
        print $src_trg_h "$src_part :: $trg_part\n";
        
        print STDERR "Splitting $c rules\n" if($c % 100000 == 0);
    }
    
    close($src_sorter);
    close($trg_sorter);
    close($src_trg_h);
    close($rh);
}

my ($src_sym_h, $src_sym_file) = tempfile( DIR => $tempdir );
my ($trg_sym_h, $trg_sym_file) = tempfile( DIR => $tempdir );
print $src_sym_h join("\n", (undef, sort keys %src));
print $trg_sym_h join("\n", (undef, sort keys %trg));
close($src_sym_h);
close($trg_sym_h);

print STDERR "Generating symbol maps\n";

`$BIN_DIR/symbolcompiler $name.src.sym < $src_sym_file`;
`$BIN_DIR/symbolcompiler $name.trg.sym < $trg_sym_file`;

print STDERR "Compiling source language rules\n";
`cat $src_rule_file | uniq | $SCRIPT_DIR/compile_string_number --symbol_list $src_sym_file | $BIN_DIR/minfsa | sort -k1,1n -k3,3n -S $size | $BIN_DIR/binfsa -o $name.src.idx`;

print STDERR "Compiling target language rules\n";
`cat $trg_rule_file | uniq | $SCRIPT_DIR/compile_string_number --symbol_list $trg_sym_file | $BIN_DIR/minfsa | sort -k1,1n -k3,3n -S $size | $BIN_DIR/binfsa -o $name.trg.idx`;

my ($hash_h, $hash_f) = tempfile( DIR => $tempdir );

{
    print STDERR "Hashing combined rules\n";

    my ($src_read_from, $src_write_to);
    open2($src_read_from, $src_write_to, "./hasher --dag $name.src.idx --sym $name.src.sym --hash");
    
    my ($trg_read_from, $trg_write_to);
    open2($trg_read_from, $trg_write_to, "./hasher --dag $name.trg.idx --sym $name.trg.sym --hash");
    
    open(my $src_trg, "< $src_trg_file") or die;
    while(<$src_trg>) {
        chomp;
        my ($src_part, $trg_part) = split(/\s+::\s+/, $_);
        
        print $src_write_to $src_part, "\n";
        print $trg_write_to $trg_part, "\n";
    
        my $src_num = <$src_read_from>;
        my $trg_num = <$trg_read_from>;
        
        chomp($src_num, $trg_num);
        print $hash_h "$src_num\t$trg_num\n";
        
        print STDERR "Hashed $. rules\n" if($. % 100000 == 0); 
    }
}
close($hash_h);

print STDERR "Calculating log-ratios for rules\n";
open(my $rh, "<$RULES") or die;
open(my $logh, "./rulefinder --hashed_rules $hash_f --src_corpus $SRC_CORPUS --trg_corpus $TRG_CORPUS --src_dag $name.src.idx --src_sym $name.src.sym --trg_dag $name.trg.idx --trg_sym $name.trg.sym -p 4 |") or die;

while(defined(my $r = <$rh>) and defined(my $l = <$logh>)) {
    chomp($r, $l);
    print "$r\t$l\n";
}
