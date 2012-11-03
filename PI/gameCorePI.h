#ifndef GAMECORE_PI_H
#define GAMECORE_PI_H

#include <stdint.h>
#include "inputDevicePI.h"

struct _surface
{
	void* buf;
	int32_t w ,h; // pixel
	int8_t depth; // bytes
	int32_t pitch; // bytes
	void (*lockSurface) (void);
	void (*unlockSurface) (void);
	int32_t (*beginFrame) (void);
	void (*endFrame) (void);
	void (*present)(void);
};
typedef struct _surface Surface, *pSurface;
struct _virtualDevice
{
	Surface* surface;
	inputDevice* input;
	// ms
	int64_t (*timer_tik)(void);
	int32_t (*platformMessageHandling)(void);
	void (*platformKeyHandling) (void);
};
typedef struct _virtualDevice virtualDevice, *pVirtualDevice;

void gameCore_main(pVirtualDevice pVD);

#endif /* GAMECORE_PI_H */