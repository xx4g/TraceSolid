
echo.
set _PREBUILD_=%0
echo %_PREBUILD_:.bat=%
echo Checking folder-^> %~1
if not exist "%~1" (
 echo =============================================
 echo *ERROR* - SDK folder does not exist! -^>
 echo %1
 echo Check the 'SDKDir' path in MastercamSDK.props
 echo =============================================
 echo.
 exit 1)

set _SDK_DIR_=%~1
set _LAST_CHAR_=%_SDK_DIR_:~-1%
if not "%_LAST_CHAR_%"=="\" (
 echo =================================================
 echo *ERROR* - SDK folder path does not end with a '\'
 echo %1
 echo Check the 'SDKDir' path in MastercamSDK.props
 echo =================================================
 echo.
 exit 1)

 if not exist "%_SDK_DIR_%m_mastercam.h" (
 echo ====================================================================
 echo *ERROR* - SDK folder path does not contain 'm_mastercam.h'
 echo %1
 echo Check the 'SDKDir' path in MastercamSDK.props points to a valid SDK.
 echo ====================================================================
 echo.
 exit 1)



