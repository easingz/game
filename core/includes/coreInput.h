#ifndef _CORE_INPUT_H_
#define _CORE_INPUT_H_

#include "gamelib.h"

int coreInput_getMouse_X();
int coreInput_getMouse_Y();
BOOL coreInput_mouseDown(VD_MOUSE_BUTTON);
STATUS coreInput_refreshState(pInputDevice);
BOOL coreInput_keyDown(VD_KEY);

#endif /* _CORE_INPUT_H_ */