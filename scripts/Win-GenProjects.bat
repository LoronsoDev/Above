@echo off
IF exist ..\vendor\premake\bin\release (echo Premaka5 binaries found!) ELSE ( pushd ..\ && pushd vendor\premake && call Bootstrap.bat && popd)
popd
pushd ..\
echo Building ABOVE project files...
call vendor\premake\bin\release\premake5.exe vs2022
popd
PAUSE