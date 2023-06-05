@echo off

call "%~dp0..\config.bat"

pushd "%SourceCodePath%"

FOR /R %%f IN (*.h, *.cpp, *.cs) DO 
(
    clang-format -i "%%f"
    echo %%f 
)

popd
