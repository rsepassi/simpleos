#!/usr/bin/env sh

set -ex

sh do/build_limine.sh
sh do/build_kernel.sh
sh do/build_boot.sh
