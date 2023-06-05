@echo off

call "%~dp0..\config.bat"

pushd "%ProjectRoot%"

FOR %%f IN (%dirsToRemove%) DO (
    echo %%f 
    rd /s /q %%f 
)

FOR %%f IN (%filesToRemove%) DO (
    echo %%f 
    del /q %%f 
)

popd
