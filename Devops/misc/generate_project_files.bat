@echo off

call "%~dp0..\config.bat"

pushd "%ProjectRoot%"

"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"

popd
