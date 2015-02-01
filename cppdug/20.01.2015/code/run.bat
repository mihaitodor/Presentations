@ECHO OFF

::Compile
::Use -fno-elide-constructors to disable RVO and NRVO for the MoveSemantics.cpp sample
START /B /WAIT g++.exe -Wall -Wextra -Wno-unused-variable -Wno-unused-value -Wno-unused-parameter -Wno-unused-but-set-variable -pedantic -std=c++14 -g %1

::Quit if compilation fails
IF ERRORLEVEL 1 EXIT /B

::Run test
START /B /WAIT a.exe

::Clean up
DEL a.exe