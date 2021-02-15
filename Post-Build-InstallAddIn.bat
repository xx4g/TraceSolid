echo.
set _POST_BUILD_=%0
echo %_POST_BUILD_:.bat=%
echo Installing Add-In files to -^> "%~1chooks\"
copy "x64\%~2\CHookProject1.dll" "%~1chooks\CHookProject1.dll"
copy "CHookProject1.ft" "%~1chooks\CHookProject1.ft"