#!/usr/bin/perl

use utf8;

# Simple script for converting vera++ output into cppcheck output format (so that
# vera++ can used with Jenkins).

print_intro();

while (my $line=<STDIN>) {
    chomp $line;

    if (my ($file_path, $line_number, $error_code, $comment) =
            ($line =~ m{ ^ \./ ([^:]+)
                   : (\d+)
                   : \s* \( ([^\)]+) \)
                   \s+ (.*) $}x)) {
        generate($file_path, $line_number, $error_code, $comment);
    } else {
        generate('vera++2cppcheck.pl', 1, 'X000', 'vera++2cppcheck.pl failed');
    }
}

print_outro();

sub generate {
    my ($file_path, $line_number, $error_code, $comment) = @_;

    $comment = quote($comment);

    print qq{<error file="$file_path" line="$line_number" id="$error_code" severity="style" msg="$comment"/>\n};
}

sub quote {
    my ($t) = @_;

    $t =~ s/\&/\&amp;/g;
    $t =~ s/</\&lt;/g;
    $t =~ s/"/\&qt;/g;

    return $t;
}

sub print_intro {
    print <<'END_OF_INTRO';
<?xml version="1.0" encoding="UTF-8"?>
<results>
END_OF_INTRO
}

sub print_outro {
    print << 'END_OF_OUTRO';
</results>
END_OF_OUTRO
}
