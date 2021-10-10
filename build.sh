#!/bin/bash

set -eu

mkdir -p build

sources_c=string_builder_tests.c
sources_cpp=string_builder_tests.cpp

exe_name_c=sb_test_c
exe_name_cpp=sb_test_cpp

compiler_flags="-g -Wall"

cp $sources_c $sources_cpp

echo "Compiling tests for string_builder.h..."
pushd build > /dev/null
echo "Compile with GCC..."
gcc $compiler_flags -o ${exe_name_c}_gcc ../$sources_c
g++ $compiler_flags -o ${exe_name_cpp}_gcc ../$sources_cpp
echo "Compile with Clang..."
clang $compiler_flags -o ${exe_name_c}_clang ../$sources_c
clang $compiler_flags -o ${exe_name_cpp}_clang ../$sources_cpp

echo -e "\nRunning tests for string_builder.h..."
echo "Run tests for GCC..."
./sb_test_c_gcc
./sb_test_cpp_gcc
echo "Run tests for Clang..."
./sb_test_c_clang
./sb_test_cpp_clang
popd > /dev/null

echo -e "\nDeleting C++ test source and building directory..."
rm $sources_cpp
rm -r build
