#!/usr/bin/perl

use IO::Socket;
my $sock = new IO::Socket::INET ( 
    #PeerAddr => 'gelbaugenpinguin', 
    PeerAddr => 'localhost', 
    PeerPort => '7100', 
    Proto => 'tcp');

die "Could not create socket: $!\n" unless $sock; 

while (<>) {
    print $sock $_;
}
shutdown($sock, 1);  # finished writing

while (<$sock>) {
    print;
}

close $sock;
