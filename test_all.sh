#!/bin/bash

set -eu

positional=()
keep_build=""

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -keep-build)
            keep_build="-keep-build"
            shift # past argument.
            ;;
        *) #unknown option
            positional+=("$1")
            shift # past argument.
            ;;
    esac
done

pushd tests > /dev/null
./test_string_builder.sh $keep_build
popd > /dev/null
