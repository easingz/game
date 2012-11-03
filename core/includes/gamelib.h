#include "../../PI/gameCorePI.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

// Only surport 32bit depth buffer temporaryly
#define setColor(buffer, x, y, color) { \
	if(buffer->depth == 4) \
		((int32_t*)(buffer->buf))[x + y*buffer->pitch/4] = color; \
}
#define ARGB(a, r, g, b) (a << 24 | r << 16 | g << 8 | b)

void handleInput(virtualDevice*);
void doGameLogic(void);
void onDrawFrame(Surface*);
