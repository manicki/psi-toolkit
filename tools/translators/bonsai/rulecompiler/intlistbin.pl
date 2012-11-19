use strict;

while(<STDIN>) {
    chomp;
    my @t = split(/\s+/, $_);
    print pack("i", $_) foreach(@t);
}