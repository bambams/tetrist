#!/usr/bin/env perl

use v5.022;
use strict;
use utf8;
use warnings;

use Data::Dump qw/dd pp/;
use Encode;

my %allocated = ();
my %destroyers = ();

my $destroyed_address = 0;

while(my $line = <>) {
    chomp $line;

    next if $line =~ /^\s*$/;

    if ($line =~ /
            (
                ^0x
                [0-9a-f]{6,7} |
                \(nil\)
            )
            [ ]
            (destroy|free|malloc)
            [ ]
            :t
            [ ]
            (
                collision | game-board | list |
                map-data | map[-_]string | piece |
                tile-map
            )
            (?:
                [ ]
                :f
                [ ]
                '#func<0x([0-9a-f]{6})> |
                $
            )?/x) {
        my ($addr, $op, $type, $lambda) = ($1, $2, $3, $4);

        unless ($addr eq '(nil)') {
            if ($op eq 'destroy' && $addr ne '(nil)') {
                $destroyed_address = 1;
            }

            if($op eq 'destroy' && defined $lambda) {
                $destroyers{$lambda}{$type} = 1;
            } elsif ($op eq 'free') {
                if (exists $allocated{$addr}) {
                    delete $allocated{$addr};
                } else {
                    warn "Double free of $addr?!\n";
                }
            } elsif ($op eq 'malloc') {
                $allocated{$addr} = { type => $type };
            }
        }
    } else {
        say STDERR "Unrecognized: $line";
    }
}

warn "Leaked $allocated{$_}{type} = $_\n" for keys %allocated;

if ($destroyed_address) {
    warn "At least one destroy operation was to an actual address! \\o/\n";
} else {
    warn "Why are all of the 'destroy' operations to (nil)?!\n";
}

1;
