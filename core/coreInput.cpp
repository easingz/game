#include "includes/coreInput.h"

#define MOUSE_ACCEL 2
static int mouse_x = 0;
static int mouse_y = 0;
static int8_t keystate[256];
static pInputDevice ID = NULL;
STATUS coreInput_refreshState(pInputDevice pid)
{
	ID = pid;
	ID->getInputState();
	mouse_x += ID->mouse_X() * MOUSE_ACCEL;
	mouse_y += ID->mouse_Y() * MOUSE_ACCEL;
	for (int i = 0;i < CORE_KEYMAP_SIZE; i++)
		keystate[i] = ID->keyDown((VD_KEY)i);
	return OK;
}

int coreInput_getMouse_X()
{
	return mouse_x;
}

int coreInput_getMouse_Y()
{
	return mouse_y;
}

BOOL coreInput_keyDown(VD_KEY code)
{
	return keystate[code];
}

BOOL coreInput_mouseDown(VD_MOUSE_BUTTON mb)
{
	return ID->buttonDown(mb);
}