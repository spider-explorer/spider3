@echo off
if "%1"=="" (
  start notepad3.exe
  exit /b
)
for %%n in (%*) do (
  start notepad3.exe %%n
)
