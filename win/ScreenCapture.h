/*
 * SampleANE.h
 *
 *  Created on: Feb 7, 2012
 *      Author: max
 */

#ifndef SAMPLEANE_H_
#define SAMPLEANE_H_

#include "FlashRuntimeExtensions.h" // import the Adobe Headers

	__declspec(dllexport) void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);
	__declspec(dllexport) void finalizer(void* extData);


#endif /* SAMPLEANE_H_ */
