echo Building DLL (shared library for windows) file...

set MicrosoftSDK=C:/Program Files/Microsoft SDKs/Windows/v7.0A

gcc -O3 -Wall -c -fmessage-length=0 -o ../win/Release/ScreenCapture.o ../win/ScreenCapture.c
gcc -shared -o ScreenCapture.dll ../win/Release/ScreenCapture.o ../win/libs/FlashRuntimeExtensions.lib "%MicrosoftSDK%/Lib/Gdi32.Lib"