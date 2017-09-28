#!/bin/bash

set -e;

pre-build() {
    mkdir -p .build;
}

post-build() {
    true;
}

pre-build;
/bin/bash -e "$@";
post-build;
