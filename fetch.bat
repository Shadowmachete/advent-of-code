@ECHO OFF

REM Usage: fetch.bat <day> <year>
REM If no day is given, the current day is used (based on system date)
REM If no year is given, the current year is used (based on system date)
REM The session cookie value must be set in the script below

set "SESSION=browser_session_here"
if "%2"=="" (set year=%date:~10,4%) else (set year=%2)
if "%1"=="" (set day=%date:~7,2%) else (set day=%1)

if "%day:~0,1%"=="0" set day=%day:~1%
set /a day=%day%

set "OUTPUTFILE=%day%.txt"
set "url=https://adventofcode.com/%year%/day/%day%/input"

curl %url% --cookie "session=%SESSION%" -o %OUTPUTFILE%

cat %OUTPUTFILE%
