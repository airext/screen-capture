/*
 * SampleANE.c
 *
 *  Created on: Feb 7, 2012
 *      Author: max
 */

#include "FlashRuntimeExtensions.h"
#include "Stdlib.h"
#include "ScreenCapture.h"
#include "String.h"
#include "windows.h"
#include "winuser.h"
#include "wingdi.h"
#include "stdio.h"

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

FREObject isSupported(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	FREObject result;

	uint32_t isUpportedInThisOS = 1;

	FRENewObjectFromBool(isUpportedInThisOS, &result);

	return result;
}


FREObject capture(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[])
{
	// the next code takes screenshot and retrieves array of pixels (I can't test if it works fine)

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	HWND window = GetDesktopWindow();
	HDC hdcSource = GetDC(window);
	HDC hdcMemory = CreateCompatibleDC(hdcSource);
	HBITMAP hbmp =CreateCompatibleBitmap(hdcSource, width, height);

	SelectObject(hdcMemory,hbmp);

	BitBlt(hdcMemory, 0, 0, width, height, hdcSource, 0, 0, SRCCOPY|CAPTUREBLT);

	BITMAPINFO MemoryLookup[4]; //Create a buffer zone for possible pallete
	BITMAPINFO* bmpInfo = MemoryLookup;
	memset(bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	GetDIBits(hdcMemory, hbmp, 0, GetDeviceCaps(hdcMemory, VERTRES), NULL, bmpInfo, DIB_RGB_COLORS);

	size_t dataSize = bmpInfo->bmiHeader.biSizeImage;//bmInfoHeader.bmiHeader.biCompression == BI_RGB ? (bmInfoHeader.bmiHeader.biHeight * bmInfoHeader.bmiHeader.biWidth * bmInfoHeader.bmiHeader.biBitCount / 8) : bmInfoHeader.bmiHeader.biSizeImage;

	unsigned char* bytes = malloc(dataSize);
	bmpInfo->bmiHeader.biCompression = BI_RGB;
	bmpInfo->bmiHeader.biSizeImage = 0;

	int scanLinesNum = GetDIBits(hdcMemory, hbmp, 0, bmpInfo->bmiHeader.biHeight, bytes, bmpInfo, DIB_RGB_COLORS);

	// DEBUG
	printf("scanLinesNum = %d\n", scanLinesNum);
	printf("dataSize = %d\n", dataSize);
	// DEBUG

	// the next code copies pixels to AS3 BitmapData object (works fine for red pixels)

	FREObject input = argv[0];

	FREBitmapData bmd;

	FREAcquireBitmapData(input, &bmd);

	int i, j;

	int offset = bmd.lineStride32 - bmd.width;

	uint32_t *bmdPixels = bmd.bits32;

	for (j = 0; j < bmd.height; j++)
	{
	    for (i = 0; i < bmd.width; i++, bmdPixels++)
	    {
	      * bmdPixels = 0xFF000000 | ((DWORD*) bytes)[j * bmpInfo->bmiHeader.biWidth + i];
	    }

	    bmdPixels += offset;
	  }

	FREReleaseBitmapData(input);

	ReleaseDC(window, hdcSource);
	DeleteDC(hdcMemory);
	DeleteObject(hbmp);

	// the next code returns debug information

	FREObject result;

	FRENewObjectFromUint32((uint32_t) 1, &result);

	return result;
}

//------------------------------------------------------------------------

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
{
	*numFunctions = 2;

	FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * (*numFunctions));

	func[0].name = (const uint8_t*)"capture";
	func[0].functionData = NULL;
	func[0].function = &capture;

	func[1].name = (const uint8_t*) "isSupported";
	func[1].functionData = NULL;
	func[1].function = &isSupported;


	*functions = func;
}

void contextFinalizer(FREContext ctx)
{
	return;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------

void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
{
	*ctxInitializer = &contextInitializer;
	*ctxFinalizer = &contextFinalizer;
}

void finializer (void* extData)
{
	return;
}
