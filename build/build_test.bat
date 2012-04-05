echo Building AIR Test application

echo Set FLEX_HOME before running

"%FLEX_HOME%"\bin\amxmlc -output ScreenshotTest.swf -- ../test/src/ScreenshotTest.mxml
"%FLEX_HOME%"\bin\adt -package -target native -storetype pkcs12 -keystore log5f.p12 ScreenCapture ../test/src/ScreenshotTest-app.xml