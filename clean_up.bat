@ECHO OFF
setlocal enabledelayedexpansion

REM Probably not needed, but just in case
REM I used this to clean up my folders after solving Advent of Code 2024 problems
REM because I didnt arrange my files properly at the start

if "%1"=="" (
  echo Usage: clean_up.bat ^<path^>
  exit /b
)

set "folder=%1"

if not exist "%folder%\" (
  echo Folder does not exit
  exit /b
)

echo Cleaning up %folder%

for %%i in (%folder%\*.*) do (
  set "name=%%~ni"
  set "file=%%i"
  echo !name! | findstr /R "^[0-9]" > nul
  if !errorlevel! equ 0 (
    for /f "tokens=1 delims=_ " %%a in ("!name!") do (
      set "num=%%a"
    )

    if !num! lss 10 (
      set "num=0!num!"
    )

    if not exist "%folder%\Day_!num!\" (
      mkdir "%folder%\Day_!num!"
    )

    move "!file!" "%folder%\Day_!num!"
  )
)
