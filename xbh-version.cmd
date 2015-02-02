@echo off
setlocal
set XBH_VERSION_LOCATION=%XBHDIR%\atmega644\source\XBH_version.h
set XBH_TMP=%XBHDIR%\atmega644\source\XBH_version.h.tmp
set LC_ALL=C
set TZ=UTC
svn help 2>&1 > NUL
if ERRORLEVEL 1 exit %ERRORLEVEL%
if NOT EXIST .svn exit 1
FOR /F "usebackq tokens=1,2,3,4,5,6,7,8,9 delims=,-: " %%a IN (`svn info %XBHDIR%`) DO (
	if "%%c" == "Rev" echo #define XBH_REVISION "%%d" > %XBH_TMP%
	if "%%c" == "Date" ( 
		echo #define XBH_VERSION_DATE "%%d%%e%%f" >> %XBH_TMP%
		echo #define XBH_VERSION_TIME "%%g%%h" >> %XBH_TMP%
	)
)

IF NOT EXIST %XBH_VERSION_LOCATION% (
	move %XBH_TMP% %XBH_VERSION_LOCATION% > NUL
	goto END
)

FC %XBH_VERSION_LOCATION% %XBH_TMP% > NUL
IF ERRORLEVEL 1 (
	del %XBH_VERSION_LOCATION% > NUL
	move %XBH_TMP% %XBH_VERSION_LOCATION% > NUL
)
IF EXIST %XBH_TMP% del %XBH_TMP% > NUL
:END
