#!/usr/bin/env perl
#
# Tetrist is a falling block game.
# Copyright (C) 2018 Brandon McCaig
#
# This file is part of Tetrist.
#
# Tetrist is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Tetrist is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libbam.  If not, see <http://www.gnu.org/licenses/>.
#

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
