@echo off
REM Usage: run this from a Visual Studio Developer Command Prompt where lib.exe is in PATH
setlocal
pushd "%~dp0\..\libs"
if not exist DataSource.def (
  echo DataSource.def not found in libs\
  popd
  exit /b 1
)
lib /def:DataSource.def /out:DataSource.lib
if %errorlevel% neq 0 (
  echo Failed to create import library
  popd
  exit /b %errorlevel%
)
echo DataSource.lib created in libs\
popd
endlocal
exit /b 0
