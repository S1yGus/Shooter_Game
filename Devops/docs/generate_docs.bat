@echo off

call "%~dp0..\config.bat"

pushd "%ProjectRoot%"
rmdir /s /q "%ProjectRoot%\Documentation"
doxygen "%ProjectRoot%\Devops\docs\Doxyfile"
start "" "%ProjectRoot%\Documentation\html\index.html"
popd
