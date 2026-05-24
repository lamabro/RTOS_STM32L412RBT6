@echo off
rem buildall.bat - Headless build for STM32CubeIDE projects with CMake fallback
rem Usage:
rem   buildall.bat            - auto-detect STM32CubeIDE and build Debug+Release for found projects
rem   set STM32CUBEIDE_PATH=C:\Path\to\stm32cubeide.exe && buildall.bat

setlocal enabledelayedexpansion
set "IDE=%STM32CUBEIDE_PATH%"
if not defined IDE (
  if exist "%ProgramFiles%\STMicroelectronics\STM32CubeIDE\stm32cubeide.exe" set "IDE=%ProgramFiles%\STMicroelectronics\STM32CubeIDE\stm32cubeide.exe"
  if not defined IDE if exist "%ProgramFiles(x86)%\STMicroelectronics\STM32CubeIDE\stm32cubeide.exe" set "IDE=%ProgramFiles(x86)%\STMicroelectronics\STM32CubeIDE\stm32cubeide.exe"
)
if not defined IDE (
  for /f "usebackq delims=" %%i in (`where stm32cubeide.exe 2^>nul`) do (
    set "IDE=%%i"
    goto :FOUND_IDE
  )
)
:FOUND_IDE
if not defined IDE (
  echo Error: STM32CubeIDE executable not found. Set environment variable STM32CUBEIDE_PATH to the full path of stm32cubeide.exe and retry.
  exit /b 1
)
echo Using STM32CubeIDE: %IDE%
set "WORKSPACE=%~dp0"
echo Workspace directory: %WORKSPACE%
echo Importing projects (may show warnings)...
"%IDE%" -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%WORKSPACE%" -import "%WORKSPACE%" >nul 2>&1 || echo Import step returned non-zero, continuing
echo Scanning for Eclipse projects (.project files)...
set "BUILT=0"
for /r "%WORKSPACE%" %%f in (.project) do (
  for /f "usebackq tokens=2 delims=><" %%a in ('findstr /R "<name>.*</name>" "%%f"') do (
    set "PN=%%a"
    echo ------------------------------------------------------------------
    echo Building project: !PN! (Debug)
    "%IDE%" -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%WORKSPACE%" -build "!PN!/Debug"
    echo Building project: !PN! (Release)
    "%IDE%" -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%WORKSPACE%" -build "!PN!/Release"
    set /a BUILT+=1
  )
)

if "%BUILT%"=="0" (
  echo No Eclipse projects detected. Falling back to CMake presets + Ninja (Linux/WSL or Windows with appropriate toolchain).
  if exist "CMakePresets.json" (
    echo Running: cmake --preset=Debug-Application && ninja -C build/Target/application/Debug && cmake --preset=Release-Application && ninja -C build/Target/application/Release
    cmake --preset=Debug-Application && ninja -C build/Target/application/Debug
    cmake --preset=Release-Application && ninja -C build/Target/application/Release
  ) else (
    echo No CMakePresets.json found. Nothing to build.
    exit /b 2
  )
)
echo Build script finished.
endlocal
exit /b 0
