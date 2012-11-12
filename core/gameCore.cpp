#include "includes/gamelib.h"

// alias to virtualDevice's surface
static Surface* surface= NULL;
BOOL running = FALSE;
coreBitmap* backbuffer = NULL;

void gameCore_init(virtualDevice*);
void gameCore_drawFrame(virtualDevice*);
void gameCore_flush();
void gameCore_shutdown();
void initSurface(virtualDevice*);

void initSurface(virtualDevice* VD)
{
	surface = VD->surface;
}
void gameCore_init(virtualDevice* VD)
{
	int tmpW=0, tmpH=0, tmpDepth=0;
	initSurface(VD);
	running = TRUE;
	tmpW = VD->surface->w;
	tmpH = VD->surface->h;
	tmpDepth = VD->surface->depth;
	backbuffer = (pCoreBitmap)malloc(sizeof(coreBitmap));
	backbuffer->buf = (void *)malloc(sizeof(char)* tmpDepth * tmpW * tmpH);
	backbuffer->w = tmpW;
	backbuffer->h = tmpH;
	backbuffer->depth = tmpDepth;
	if(!settingManager_init())
		running = FALSE;
	//game part core settings
	if(!initGameSetting())
		running = FALSE;
}

void gameCore_shutdown()
{
	surface = NULL;
	free(backbuffer->buf);
	free(backbuffer);
}

void gameCore_main(virtualDevice* VD)
{
	gameCore_init(VD);
	//main game loop
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

//flush backbuffer onto surface, now only surport 32bit color
void gameCore_flush()
{
	char* src = (char*)(backbuffer->buf);
	//gameCore_drawFrame makes sure the buf is ready
	char* dest = (char*)(surface->buf);
	if (surface->depth == 4)

		for (int i = 0; i < backbuffer->h; i++)
		{
			int destoffset = i * surface->pitch;
			int srcoffset = i * backbuffer->w * 4;
			for (int j = 0; j < backbuffer->w * 4; j+=4)
			{
				int k = j + destoffset;
				int l = j + srcoffset;
				dest[k++] = src[l++];
				dest[k++] = src[l++];
				dest[k++] = src[l++];
				dest[k] = src[l];
			}
		}
}

void gameCore_drawFrame(virtualDevice* VD)
{
	int64_t startTime = VD->timer_tik();
	// get current input state
	if (!coreInput_refreshState(VD->input))
		running = FALSE;

	// game part
	if (!(handleInput(VD->input)))
		running = FALSE;
	if (!(doGameLogic()))
		running = FALSE;
	if (!renderScene())
		running = FALSE;

	// flush to screen
	if(surface->beginFrame())
	{
		surface->lockSurface();
		gameCore_flush();
		drawCursor(surface);
		surface->unlockSurface();
		surface->endFrame();
	}
	surface->present();

	//sync to 30fps
	while( (VD->timer_tik() - startTime) < 16 );
}