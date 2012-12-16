//
//  ScreenCapture.m
//  ScreenCapture
//
//  Created by Max Rozdobudko on 15.12.12.
//  Copyright (c) 2012 Max Rozdobudko. All rights reserved.
//

#import "ScreenCapture.h"


//------------------------------------------------------------------------
//
//  API
//
//------------------------------------------------------------------------

FREObject isSupported(FREContext ctx, void *data, uint32_t argc, FREObject args[])
{
    FREObject result;
    
    uint32_t isUpportedInThisOS = 1;
    
    FRENewObjectFromBool(isUpportedInThisOS, &result);
    
    return result;
}


FREObject capture(FREContext ctx, void *data, uint32_t argc, FREObject argv[])
{
    FREObject input = argv[0];
    
    FREBitmapData bmd;
    
    FREAcquireBitmapData(input, &bmd);
    
    CGImageRef imageRef = CGDisplayCreateImage(CGMainDisplayID());
    
    NSUInteger width = CGImageGetWidth(imageRef);
    NSUInteger height = CGImageGetHeight(imageRef);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    // Pixel color values will be written here
    unsigned char *rawData = malloc(height * width * 4);
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    
    // Pixels are now in rawData in the format RGBA8888
    // We'll now loop over each pixel write them into the AS3 bitmapData memory
    int x, y;
    // There may be extra pixels in each row due to the value of
    // bmd.lineStride32, we'll skip over those as needed
    int offset = bmd.lineStride32 - bmd.width;
    int offset2 = bytesPerRow - bmd.width*4;
    int byteIndex = 0;
    uint32_t *bmdPixels = bmd.bits32;
    
    // NOTE: In this example we are assuming that our AS3 bitmapData and our native UIView are the same dimensions to keep things simple.
    for(y=0; y<bmd.height; y++) {
        for(x=0; x<bmd.width; x++, bmdPixels ++, byteIndex += 4) {
            // Values are currently in RGBA8888, so each colour
            // value is currently a separate number
            int red   = (rawData[byteIndex]);
            int green = (rawData[byteIndex + 1]);
            int blue  = (rawData[byteIndex + 2]);
            int alpha = (rawData[byteIndex + 3]);
            // Combine values into ARGB32
            * bmdPixels = (alpha << 24) | (red << 16) | (green << 8) | blue;
        }
        
        bmdPixels += offset;
        byteIndex += offset2;
    }
    
    // free the the memory we allocated
    free(rawData);
    
    // Tell Flash which region of the bitmapData changes (all of it here)
    FREInvalidateBitmapDataRect(argv[0], 0, 0, bmd.width, bmd.height);
    // Release our control over the bitmapData
    FREReleaseBitmapData(argv[0]);
    
    return NULL;
}


//------------------------------------------------------------------------
//
//  Context Initializer/Finalizer
//
//------------------------------------------------------------------------

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
{
    *numFunctions = 2;
    FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * (*numFunctions));
    
    func[0].name = (const uint8_t*) "isSupported";
    func[0].functionData = NULL;
    func[0].function = &isSupported;
    
    func[1].name = (const uint8_t*) "capture";
    func[1].functionData = NULL;
    func[1].function = &capture;
    
    *functions = func;
}

void contextFinalizer(FREContext ctx)
{
    return;
}

//------------------------------------------------------------------------
//
//  External
//
//------------------------------------------------------------------------

void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
{
    *ctxInitializer = &contextInitializer;
    *ctxFinalizer = &contextFinalizer;
}

void finalizer(void *extData)
{
    return;
}