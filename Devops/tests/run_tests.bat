@echo off

call "%~dp0..\config.bat"

rem bild sources
call "%RunUATPath%" ^
BuildCookRun ^
-project="%ProjectPath%" ^
-platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-build -cook

rem run tests
set TestRunner="%EditorPath%" "%ProjectPath%" -ExecCmds="Automation RunTests %TestNames%;Quit" -log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem tests coverage
"%OpenCPPCoveragePath%" --modules "%ProjectRoot%" --sources "%SourceCodePath%" --excluded_sources "%SourceCodePath%\%ProjectPureName%\Public\Tests" --excluded_sources "%SourceCodePath%\%ProjectPureName%\Private\Tests" ^
--export_type html:"%ReportOutputPath%\Coverage" -v -- %TestRunner%

del /q LastCoverageResults.log

rem copy test artifacts
robocopy /e "%~dp0data" "%ReportOutputPath%"

rem start local server and show report
set Port=8081
set Localhost=http://127.0.0.1:%Port%

start "" "%Localhost%"
start "" "%Localhost%\Coverage\index.html"

pushd "%ReportOutputPath%"
call http-server -p="%Port%"
popd
