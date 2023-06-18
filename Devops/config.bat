@echo off

rem Engine params
set EnginePath=C:\Program Files\Epic Games\UE_5.2
set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Editor path
set EditorPath=%EnginePath%\Engine\Binaries\Win64\UnrealEditor.exe

rem Project params
set ProjectRoot=C:\Unreal Projects\Shooter_Game
set ProjectPureName=Shooter_Game
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build Documentation
set filesToRemove=*.sln

rem Tests
set TestOutputLogPath=%ProjectRoot%\Build\Tests\Tests.log
set ReportOutputPath=%ProjectRoot%\Build\Tests
set UEAutomationContentPath=%EnginePath%\Engine\Content\Automation
set OpenCPPCoveragePath=C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe
set TestNames=Shooter_Game
