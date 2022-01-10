#!/bin/bash

set -eu

positional=()
keep_build=""

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -keep-build)
            keep_build="true"
            shift # past argument.
            ;;
        *) #unknown option
            positional+=("$1")
            shift # past argument.
            ;;
    esac
done

mkdir -p ../build

sources_c=test_string_builder.c
sources_cpp=test_string_builder.cpp

exe_name_c=test_string_builder_c
exe_name_cpp=test_string_builder_cpp

compiler_flags="-g -Wall"

cp $sources_c $sources_cpp

pushd ../build > /dev/null
echo "Compiling tests for string_builder.h with GCC..."
gcc $compiler_flags -o ${exe_name_c}_gcc ../tests/$sources_c
g++ $compiler_flags -o ${exe_name_cpp}_gcc ../tests/$sources_cpp
echo "Compiling tests for string_builder.h with Clang..."
clang $compiler_flags -o ${exe_name_c}_clang ../tests/$sources_c
clang $compiler_flags -o ${exe_name_cpp}_clang ../tests/$sources_cpp

echo -e "\nRunning GCC-compiled C tests for string_builder.h.."
./test_string_builder_c_gcc
echo -e "\nRunning GCC-compiled C++ tests for string_builder.h.."
./test_string_builder_cpp_gcc
echo -e "\nRunning Clang-compiled C tests for string_builder.h.."
./test_string_builder_c_clang
echo -e "\nRunning Clang-compiled C++ tests for string_builder.h.."
./test_string_builder_cpp_clang
popd > /dev/null

rm $sources_cpp
if [ -z "$keep_build" ]; then rm -r ../build; fi
