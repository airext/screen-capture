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

	HDC hdcScr, hdcMem;
	HBITMAP hbmScr;
	RECT rc;

	HWND desktopWnd = GetDesktopWindow();
	GetWindowRect(desktopWnd, &rc);

	int iXRes,
		iYRes;

	iXRes = rc.right - rc.left;
	iYRes = rc.bottom - rc.top;

	hdcScr = GetDC(desktopWnd);
	hdcMem = CreateCompatibleDC(hdcScr);
	hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);

	SelectObject(hdcMem, hbmScr);

//	BITMAPINFO bmpInfo;
//	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
//
//	GetDIBits(hdcMem, hbmScr, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

	BitBlt(hdcMem, 0, 0, iXRes, iYRes, hdcScr, rc.left, rc.top, SRCCOPY);

	BITMAP bmp;

	GetObject(hdcMem, sizeof(BITMAP), &bmp);

	FREObject result;
	FRENewObjectFromInt32((int32_t) bmp.bmWidthBytes, &result);

//	int w, h;
//	w = GetSystemMetrics(SM_CXSCREEN);
//	h = GetSystemMetrics(SM_CYSCREEN);
//
//	HDC hdc = GetDC(NULL);
//	HDC hdcScreen = CreateCompatibleDC(hdc);
//
//	HBITMAP hbmp = CreateCompatibleBitmap(hdc, 1920, 1080);
//
//	BITMAPINFO bmpInfo;
//	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
//
//	GetDIBits(hdcScreen, hbmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
//
//	FREObject result;
//	FRENewObjectFromInt32((int32_t) bmpInfo.bmiHeader.biHeight, &result);
//
//	BYTE* bits = malloc(1920 * 1080 * sizeof(BYTE));
//
//	// set up the structure of the buffer to receive by setting up the BITMAPINFOHEADER structure
//	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
//	bmpInfo.bmiHeader.biBitCount = 32;  // no stride necessary, although you won't use the alpha value provided
//	bmpInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to parse
//	bmpInfo.bmiHeader.biHeight = (bmpInfo.bmiHeader.biHeight < 0) ? (-bmpInfo.bmiHeader.biHeight) : (bmpInfo.bmiHeader.biHeight);  // correct the bottom-up ordering of lines
//
//	GetDIBits(hdcScreen, hbmp, 0, bmpInfo.bmiHeader.biHeight, (LPVOID) bits, &bmpInfo, DIB_RGB_COLORS);

	// Note: in this place we should have array of pixels
	
	// test

//	int x, y;
//
//	uint32_t p = 0;
//
//	char red;
//
//	FREObject array;
//
//	FRENewObject((const uint8_t*) "Array", 0, NULL, &array, NULL);
//	FRESetArrayLength(array, (uint32_t) bmp.bmWidth * bmp.bmHeight);
//
//	char* pCurrPixel = (char*)bmp.bmBits;
//	for (y = 0; y < bmp.bmHeight; y++)
//	{
//	    for (x = 0; x < bmp.bmWidth; x++)
//	    {
//
//	        red = pCurrPixel[0];
////	        green = pCurrPixel[1];
////	        blue = pCurrPixel[2];
//
//	        pCurrPixel += 4;
//
//	        FREObject value;
//
//	        FRENewObjectFromInt32((int32_t) red, &value);
//
//	        FRESetArrayElementAt(&array, p, value);
//	        p++;
//	    }
//	}

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
