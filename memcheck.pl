#!/usr/bin/env perl

use v5.022;
use strict;
use utf8;
use warnings;

use Data::Dumper;
use Encode;

$Data::Dumper::Sortkeys = 1;
$Data::Dumper::Indent = 1;

my %pointers;

while(<>) {
    chomp;
    /^(0x[0-9a-f]+) (free|malloc|destroy) :t (\w+)/ or next;
    $pointers{$1}{$2} = 1;
    $pointers{$1}{type} = $3;
}

while (my ($pointer, $properties) = each %pointers) {
    if($properties->{malloc} && !$properties->{free}) {
        say "$properties->{type} $pointer was not freed.";
    }
}

1;
