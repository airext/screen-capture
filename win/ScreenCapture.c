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
	// the next code takes screenshot and retrieves array of pixels (I can't test if it works fine)

	HDC hDc = GetWindowDC(NULL);

	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));

	bmFileHeader.bfType = 0x4D42;
	bmFileHeader.bfSize = 0; //Need to be filled in future
	bmFileHeader.bfOffBits = 0; //Need to be filled in future

	BITMAPINFO MemoryLookup[4]; //Create a buffer zone for possible pallete
	BITMAPINFO* pbmInfoHeader = MemoryLookup;
	memset(pbmInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	pbmInfoHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HBITMAP hBmp = (HBITMAP) GetCurrentObject(hDc, OBJ_BITMAP);
	if (hBmp == NULL)
	        return NULL;

	LONG lRes = GetDIBits(hDc, hBmp, 0, GetDeviceCaps(hDc, VERTRES), NULL, pbmInfoHeader, DIB_RGB_COLORS);

	//Capture code till line 55
	FILE* fLog = fopen ("capture.log", "wt");
	printf("Base format:\n");
	printf("biSize = %X\n", pbmInfoHeader->bmiHeader.biSize);
	//Print all propertys...
	printf("biSize = %X\n\n", pbmInfoHeader->bmiHeader.biClrImportant);
	//Capture code till line 74....
	printf("Image format:\n");
	printf("biSize = %X\n", pbmInfoHeader->bmiHeader.biSize);
	//Print all propertys...
	printf("biSize = %X\n\n", pbmInfoHeader->bmiHeader.biClrImportant);
	fclose(fLog);

	if (lRes == 0)
	        return NULL;

	WORD  biBitCount = pbmInfoHeader->bmiHeader.biBitCount;
	DWORD biClrImportant = pbmInfoHeader->bmiHeader.biClrImportant;
	DWORD biClrUsed = pbmInfoHeader->bmiHeader.biClrUsed;
	DWORD biCompression = pbmInfoHeader->bmiHeader.biCompression;
	LONG  biHeight = pbmInfoHeader->bmiHeader.biHeight;
	WORD  biPlanes = pbmInfoHeader->bmiHeader.biPlanes;
	DWORD biSize = pbmInfoHeader->bmiHeader.biSize;
	DWORD biSizeImage = pbmInfoHeader->bmiHeader.biSizeImage;
	LONG  biWidth = pbmInfoHeader->bmiHeader.biWidth;
	LONG  biXPelsPerMeter = pbmInfoHeader->bmiHeader.biXPelsPerMeter;
	LONG  biYPelsPerMeter = pbmInfoHeader->bmiHeader.biYPelsPerMeter;

	size_t nDataSize = pbmInfoHeader->bmiHeader.biSizeImage;//bmInfoHeader.bmiHeader.biCompression == BI_RGB ? (bmInfoHeader.bmiHeader.biHeight * bmInfoHeader.bmiHeader.biWidth * bmInfoHeader.bmiHeader.biBitCount / 8) : bmInfoHeader.bmiHeader.biSizeImage;

	unsigned char* pBmp = malloc(nDataSize);
	pbmInfoHeader->bmiHeader.biCompression = BI_RGB;
	pbmInfoHeader->bmiHeader.biSizeImage = 0;
	lRes = GetDIBits(hDc, hBmp, 0, pbmInfoHeader->bmiHeader.biHeight, pBmp, pbmInfoHeader, DIB_RGB_COLORS);

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
	      * bmdPixels = 0xFF000000 | ((DWORD*) pBmp)[j * pbmInfoHeader->bmiHeader.biWidth + i];
	    }

	    bmdPixels += offset;
	  }

	FREReleaseBitmapData(input);

	// the next code returns debug information

	FREObject result;

	FRENewObjectFromUint32((uint32_t) biYPelsPerMeter, &result);

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
