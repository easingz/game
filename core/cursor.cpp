#include "includes/cursor.h"

// low performance, try canceling the multiply
#define setSufColor(suf, x, y, color) { \
	if(suf->depth == 4) \
		((int32_t*)(suf->buf))[x + y*(suf->pitch>>2)] = color; \
}

static int cursorTheme = 0x00000000;
static int shape = SQUARE;

void paintCursor(pSurface);

STATUS drawCursor(pSurface suf)
{
	BOOL isEnabled = FALSE;
	cursorProp* prop = NULL;

	RETURN_IF(settingManager_getBool("cursor", &isEnabled));
	if (isEnabled)
	{
		RETURN_IF(settingManager_getProperty("cursor", (void**)(&prop)));
		cursorTheme = prop->ThemeColor;
		shape = prop->shape;
		paintCursor(suf);
	}

	return OK;
}

// now just square ...
static void paintCursor(pSurface suf)
{
	int x = coreInput_getMouse_X();
	int y = coreInput_getMouse_Y();
	switch(shape)
	{
	case SQUARE:
	default:
		//square of 5 pixels wide
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if ( x>= suf->w - 5)
		{
			x = suf->w - 5;

		}
		if (y >= suf->h - 5)
		{
			y = suf->h - 5;
		}
		for (int i = x; i < x + 5; i++)
			for (int j = y; j < y + 5; j++)
				setSufColor(suf, i, j, cursorTheme);
		break;
	}
}