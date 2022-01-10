@echo off

setlocal

set keep_build=

:parse_arguments
if "%~1"=="" goto end_parse
if "%~1"=="-keep-build" (set keep_build="-keep-build")
shift
:end_parse

pushd tests
call test_string_builder.bat %keep_build%
popd tests
