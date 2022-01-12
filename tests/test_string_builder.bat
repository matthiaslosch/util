@echo off

setlocal

where /q cl || (
    echo Error: 'cl' not found. Run this batch file from a MSVC x64 native tools command prompt.
    exit /b 1
)

if "%Platform%" neq "x64" (
    echo Error: Platform is not 'x64'. Run this batch file from a MSVC x64 native tools command prompt.
    exit /b 1
)

set keep_build=

:parse_arguments
if "%~1"=="" goto end_parse
if "%~1"=="-keep-build" (set keep_build=true)
shift
:end_parse

if not exist ..\build mkdir ..\build

set sources_c=test_string_builder.c
set sources_cpp=test_string_builder.cpp

set libraries=kernel32.lib
set exe_name_c=test_string_builder_c.exe
set exe_name_cpp=test_string_builder_cpp.exe

set compiler_flags=/Zi /W4

copy %sources_c% %sources_cpp% >NUL 2>NUL
pushd ..\build
echo Compiling tests for string_builder.h...
cl /nologo %compiler_flags% %warning_flags% /Fe%exe_name_c% ..\tests\%sources_c% /link %libraries%
cl /nologo %compiler_flags% %warning_flags% /Fe%exe_name_cpp% ..\tests\%sources_cpp% /link %libraries%

echo.
echo Running C tests for string_builder.h...
%exe_name_c%
echo.
echo Running C++ tests for string_builder.h...
%exe_name_cpp%
popd

del %sources_cpp%
if not defined keep_build (rmdir /s /q ..\build)
