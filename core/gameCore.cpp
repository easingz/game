#include "includes/gamelib.h"

// alias to virtualDevice's surface
static Surface* surface= NULL;
BOOL running = FALSE;

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
	running = TRUE;
}

void gameCore_shutdown()
{
	surface = NULL;
}

void gameCore_main(virtualDevice* VD)
{
	gameCore_init(VD);
	while(running)
	{
		if(!VD->platformMessageHandling())
			break;
		if(!VD->paused)
			gameCore_drawFrame(VD);
		VD->platformInputHandling();
	}
	gameCore_shutdown();
}

void gameCore_drawFrame(virtualDevice* VD)
{
	int64_t startTime = VD->timer_tik();
	if (!(handleInput(VD->input)))
		running = FALSE;
	if (!(doGameLogic()))
		running = FALSE;
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