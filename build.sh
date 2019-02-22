#!/bin/bash

src_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_dir="comms"

cd ~
mkdir $build_dir
cd $build_dir

cmake $src_dir

