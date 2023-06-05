@echo off

call "%~dp0..\config.bat"

set TestDataDir=%~dp0data

rem remove previous data
rmdir /s /q "%TestDataDir%"

rem copy automation content
robocopy /e "%UEAutomationContentPath%" "%TestDataDir%"

rem instal bower
call npm install -g bower

rem instal bower packages
pushd "%TestDataDir%"
call bower install "%TestDataDir%\bower.json"
popd

rem instal server
call npm install http-server bower -g
