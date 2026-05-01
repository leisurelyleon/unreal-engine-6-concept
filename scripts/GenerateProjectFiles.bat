@echo off
REM ==============================================================================
REM UNREAL ENGINE 6 - PROJECT FILE GENERATION SCRIPT
REM ==============================================================================

echo [UE6] Initiating Unreal Build Tool (UBT)...

REM Locate the local UE6 Installation (Mock path for conceptual repo)
set UE6_ROOT="C:\Program Files\Epic Games\UE_6.0"
set UBT_EXE="%UE6_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"

if not exist %UBT_EXE% (
    echo [ERROR] Unreal Engine 6 Core Installation not found.
    echo Please verify your Epic Games Launcher installation.
    pause
    exit /b 1
)

echo [UE6] Generating Visual Studio 2028 Solution Files...
%UBT_EXE% -projectfiles -project="%CD%\UE6_Simulation.uproject" -game -rocket -progress

echo [UE6] Successfully generated Project Files.
echo You may now open the solution in Visual Studio or JetBrains Rider.
pause
