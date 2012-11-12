#ifndef _GAME_LIB_H_
#define _GAME_LIB_H_

#include "../../PI/gameCorePI.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#undef BOOL
#define BOOL int8_t
#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE 1

typedef enum
{
	ERROR = 0,
	OK = 1,
}STATUS;

// depends on the previous defination
#include "settingmanager.h"
#include "coreInput.h"
#include "settingdef.h"
#include "cursor.h"

#define RETURN_IF(x) {if(!(x)) return ERROR;}
#define BREAK_IF(x) {if(!(x)) break;}
struct _coreBitmap
{
	void* buf;
	int w,h,depth;
};
typedef struct _coreBitmap coreBitmap, *pCoreBitmap;
// Only surport 32bit depth buffer temporaryly
#define setColor(buffer, x, y, color) { \
	if(buffer->depth == 4) \
		((int32_t*)(buffer->buf))[x + y*buffer->w] = color; \
}
#define ARGB(a, r, g, b) (a << 24 | r << 16 | g << 8 | b)

// only backbuffer provided for game
extern coreBitmap* backbuffer;

// game interface, what game part should implement
STATUS initGameSetting(void);
BOOL handleInput(inputDevice*);
BOOL doGameLogic(void);
//game should use backbuffer as final render target
BOOL renderScene(void);

#endif /* _GAME_LIB_H_ */