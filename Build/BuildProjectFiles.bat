@echo off
pushd %~dp0\..\
call Build\premake\bin\premake5.exe gmake2
popd
