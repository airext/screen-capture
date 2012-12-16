//
//  ScreenCapture.h
//  ScreenCapture
//
//  Created by Max Rozdobudko on 15.12.12.
//  Copyright (c) 2012 Max Rozdobudko. All rights reserved.
//

#ifndef SampleANE_SampleANE_h
#define SampleANE_SampleANE_h

#define EXPORT __attribute__((visibility("default")))

#include <Adobe AIR/Adobe AIR.h>


FREObject isSupported(FREContext context, void *data, uint32_t, FREObject args[]);


FREObject capture(FREContext context, void *data, uint32_t, FREObject args[]);

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions);

void contextFinalizer(FREContext ctx);


EXPORT
void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);

EXPORT
void finalizer(void* extData);

#endif