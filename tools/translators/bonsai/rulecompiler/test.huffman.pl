use Data::Dumper;

use Huffman qw(count_frequencies generate_huff huff dehuff);

open(my $in, "<:utf8", "plfr.rules.trg.txt");

my $symbols = {};
my $counts = {};
while(<$in>) {
    chomp;
    
    my @tokens = split(/\s+/, $_);
    foreach my $tok (@tokens) {
        if(not exists($symbols->{$tok})) {
            $symbols->{$tok} = scalar keys %$symbols;
        }
    }
    
    my $string = pack("I*", (scalar @tokens, map { $symbols->{$_} } @tokens));
    $counts->{$_}++ foreach(split(//,$string));
}

close($in);

print STDERR Dumper($counts);

my $code = generate_huff($counts);

print STDERR Dumper($code);

binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");

open(OUT, ">test");
binmode(OUT);

my $last = "";
my @targets;
while(<STDIN>) {
    chomp;
    my ($src, $trg) = split(/\s+::\s+/, $_);
    $trg =~ s/\[([^\[\]]*)\]$/join(" ", split(m!,!, $1))/e;
    $trg =~ s/\s+/ /g;
    
    if($last and $src ne $last) {
        print STDERR "$last : ";
        print STDERR tell(OUT);
        print STDERR "\n";
        
        my $data = join("", @targets);
        my $encoded_data = huff($data, $code);
        print OUT pack("I",length($encoded_data)).$encoded_data;
        @targets = ();
    }
    my @tokens = split(/\s+/, $trg);
    my $trgstring = pack("I*", (scalar @tokens, map { exists($symbols->{$_}) ? $symbols->{$_} : 0 } @tokens));
    push(@targets, $trgstring);
    
    $last = $src;
}

close(OUT);
open(OUT, "<test");

my $start = "";
read(OUT, $start, 4);
my $length = unpack("I", $start);
my $record = "";
read(OUT, $record, $length);
my $decoded = dehuff($record, $code);
my @rules = unpack("I*", $decoded);

my $revsymbols = { reverse %$symbols };
while(@rules) {
    my $symbolc = shift @rules;
    my $c = 1;
    foreach($c .. $symbolc) {
        my $next = shift @rules;
        print STDERR $revsymbols->{$next};
        print STDERR " ";
    }
    print STDERR "\n";
}
