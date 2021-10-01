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

if not exist build mkdir build

set sources_c=string_builder_tests.c
set sources_cpp=string_builder_tests.cpp

set libraries=kernel32.lib
set exe_name_c=sb_test_c.exe
set exe_name_cpp=sb_test_cpp.exe

set compiler_flags=/Zi /W4

rem Warning 4267: Conversion from size_t to int
rem Warning 4706: Assignment within conditional expression
rem Warning 4996: CRT deprecation warnings
set warning_flags=/wd4267 /wd4706 /wd4996

echo Compiling tests for string_builder.h...
copy %sources_c% %sources_cpp%
pushd build
cl /nologo %compiler_flags% %warning_flags% /Fe%exe_name_c% ..\%sources_c% /link %libraries%
cl /nologo %compiler_flags% %warning_flags% /Fe%exe_name_cpp% ..\%sources_cpp% /link %libraries%

echo.
echo Running tests for string_builder.h...
sb_test_c.exe
sb_test_cpp.exe
popd build

echo.
echo Deleting C++ test source and build directory...
del %sources_cpp%
rmdir /s /q build
