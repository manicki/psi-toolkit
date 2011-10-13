#!/usr/bin/perl

use Test::More;

use strict;
use warnings;
use utf8;

use Data::Dumper;

BEGIN { use_ok( 'PSIToolkit::' ); }
require_ok( 'PSIToolkit::' );

# ===================
# Initialization

# ====================================
# Main tests

binmode(STDOUT, ":utf8");
binmode(STDIN, ":utf8");
binmode(STDERR, ":utf8");


END:
done_testing();


# =====================================
# Helper functions

1;
