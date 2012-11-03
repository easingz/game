#include "includes/gamelib.h"

// alias to virtualDevice's surface
static Surface* surface= NULL;

void gameCore_init(virtualDevice*);
void gameCore_drawFrame(virtualDevice*);
void gameCore_shutdown();
void initSurface(virtualDevice*);

void initSurface(virtualDevice* VD)
{
	surface = VD->surface;
}
void gameCore_init(virtualDevice* VD)
{
	initSurface(VD);
}

void gameCore_shutdown()
{
	surface = NULL;
}

void gameCore_main(virtualDevice* VD)
{
	gameCore_init(VD);
	while(1)
	{
		if(!VD->platformMessageHandling())
			break;
		gameCore_drawFrame(VD);
		VD->platformKeyHandling();
	}
	gameCore_shutdown();
}

void gameCore_drawFrame(virtualDevice* VD)
{
	int64_t startTime = VD->timer_tik();
	handleInput(VD);
	doGameLogic();
	if(surface->beginFrame())
	{
		surface->lockSurface();

		onDrawFrame(surface);

		surface->unlockSurface();
		surface->endFrame();
	}
	surface->present();
	while( (VD->timer_tik() - startTime) < 33 );
}