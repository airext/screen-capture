echo Packaging ANE file...

set Path=%Path%;./utils;./air-sdk/bin;

unzip -o ScreenCapture.swc
adt -package -target ane ScreenCapture.ane extension.xml -swc ScreenCapture.swc -platform Windows-x86 library.swf ScreenCapture.dll