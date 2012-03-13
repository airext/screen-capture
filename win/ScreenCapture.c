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

//------------------------------------------------------------------------

FREObject isSupported(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	FREObject result;

	uint32_t isUpportedInThisOS = 1;

	FRENewObjectFromBool(isUpportedInThisOS, &result);

	return result;
}


FREObject capture(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[])
{
	// the next code takes screenshot and retrives array of pixels (I can't test if it works fine)

	UINT32 x,y;
	x = GetSystemMetrics(SM_CXSCREEN);
	y = GetSystemMetrics(SM_CYSCREEN);

	HDC hdc = GetDC(NULL);
	HDC hdcScreen = CreateCompatibleDC(hdc);

	HBITMAP hbmp = CreateCompatibleBitmap(hdc, x, y);

	BITMAPINFO bmpInfo;

	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));

	COLORREF * bits = malloc(1920 * 1080 * sizeof(COLORREF));

	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	GetDIBits(hdcScreen, hbmp, 0, bmpInfo.bmiHeader.biHeight, &bits, &bmpInfo, DIB_RGB_COLORS);

	// Note: in this place we should have array of pixels
	
	// the next code copies pixels to AS3 BitmapData object (works fine for red pixels)

	FREObject input = argv[0];

	FREBitmapData2 bmd;

	FREAcquireBitmapData2(input, &bmd);

	int i, j;

	int offset = bmd.lineStride32 - bmd.width;

	uint32_t *bmdPixels = bmd.bits32;

	int32_t colorValue;

	for (j = 0; j < bmd.height; j++)
	{
	    for (i = 0; i < bmd.width; i++, bmdPixels++)
	    {
			// copy pixels from bits array (not working)
			/*
			int r = bits[3*((j*bmpInfo.bmiHeader.biHeight)+i)+2];
			int g = bits[3*((j*bmpInfo.bmiHeader.biHeight)+i)+1];
			int b = bits[3*((j*bmpInfo.bmiHeader.biHeight)+i)+0];

			colorValue = (r << 16) | (g << 8) | b;
			*/
		
			// make each pixel red (for testing)
			colorValue = 0xFFFF0000;

	      * bmdPixels = colorValue;
	    }

	    bmdPixels += offset;
	  }

	FREReleaseBitmapData(input);

	// the next code returns debug information

	FREObject result;

	FRENewObjectFromUint32((uint32_t) colorValue, &result);

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
