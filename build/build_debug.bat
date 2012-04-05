echo Launching Test Application

set Path=%Path%;./utils;

xcopy "..\test\src\ScreenCaptureTest-app.xml" /Y

unzip -o ScreenCapture.ane -d extensions/ScreenCapture.ane

"%FLEX_HOME%\bin\adl" ScreenCaptureTest-app.xml -extdir extensions