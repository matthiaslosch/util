#!/bin/bash

set -eu

pushd tests > /dev/null
./test_string_builder.sh
popd > /dev/null
