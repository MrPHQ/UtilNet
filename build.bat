@echo off
SET VSCMD=%VS120COMNTOOLS%../IDE/devenv
if exist "%~dp0\log.txt" (
	Del "%~dp0\log.txt" /q
)
"%VSCMD%" "%~dp0\UtilNet.sln" /Rebuild release /Project UtilNet "UtilNet\UtilNet.vcxproj" /Out "%~dp0\log.txt"

setlocal enabledelayedexpansion

set des_path=E:/install_file/common
set bin_path=%~dp0/bin
for /R %bin_path% %%i in (
	"*UtilNet.dll"
	) do (
		Xcopy "%%~fi" "%des_path%" /y /q
		)

pause
goto end
:end