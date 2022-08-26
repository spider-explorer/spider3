@echo off
setlocal
set file=%1
for %%i IN ("%file%") DO (
	set ext=%%~xi
)
if "%ext%"==".bat" (
  @echo on
  call "%file%"
  @echo off
) else if "%ext%"==".cmd" (
  @echo on
  call "%file%"
  @echo off
) else if "%ext%"==".sh" (
  @echo on
  rem busybox bash -c "%file:\=/%"
  busybox bash "%file%"
  @echo off
)
del /f %file%
pause
endlocal
