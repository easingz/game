#include "../../core/includes/gamelib.h"
#define MAX_ITER 20

static BOOL isDrawn = FALSE;

tchar* gameName = _STR("Mandelbrot");
struct _mRange
{
	float re_min, re_max, img_min, img_max;
	struct _mRange* pre;
};

typedef struct _mRange mRange, *pmRange;

mRange head = {-2.0, 1.0, -1.25, 1.25, NULL};

static pmRange renderState = &head;
/////////////complex number define//////////////////////////
typedef struct _mycomplex
{
    float real;
    float imag;
    float length;
}complex, *pComplex;

float complexLength(pComplex const a)
{
    return sqrt(a->real * a->real + a->imag * a->imag);
}

void complexMultiply(pComplex const a, pComplex const b, pComplex result)
{
    complex tmp;
    tmp.real = a->real * b->real - a->imag * b->imag;
    tmp.imag = a->real * b->imag + a->imag * b->real;
    result->real = tmp.real;
    result->imag = tmp.imag;
    result->length = complexLength(result);
}

void complexAdd(pComplex const a, pComplex const b, pComplex result)
{
    complex tmp;
    tmp.real = a->real + b->real;
    tmp.imag = a->imag + b->imag;
    result->real = tmp.real;
    result->imag = tmp.imag;
    result->length = complexLength(result);
}


////////////////////////////////////////////////////////////
void drawMandelbrot()
{
    int i,j,count;
    float realDelta = (renderState->re_max - renderState->re_min) / backbuffer->w;
    float imagDelta = (renderState->img_max - renderState->img_min) / backbuffer->h;

    complex z;
    //complex z = {-0.75, 0.0};
    //z.length = complexLength(&z);
    for (i = 0, z.real = renderState->re_min ; i < backbuffer->w; i++,z.real+=realDelta)
    {
        for (j = 0, z.imag = renderState->img_min; j < backbuffer->h; j++,z.imag+=imagDelta)
        {
            complex _x = {0, 0, 0.0};
            pComplex x = &_x;
            for (count = 0; count < MAX_ITER; count++)
            {
                if(x->length > 2) break;
                complexMultiply(x, x, x);
                complexAdd(x, &z, x);
            }
            if(x->length <= 2.0)
			{
				setColor(backbuffer, i, j, ARGB(0xff,0,0,0));
			}
            else
            {
                int depth =(int)( 765.0 * count / MAX_ITER);
                int red = depth % 255;
                int green = depth % 510;
                int blue = depth % 755;
				setColor(backbuffer, i, j, ARGB(0xff,red,green,blue));
            }
        }
    }
	isDrawn =TRUE;
}

STATUS initGameSetting()
{
	pCursorProp cp = (pCursorProp)malloc(sizeof(cursorProp));
	cp->shape = SQUARE;
	//red cursor
	cp->ThemeColor = 0xffff0000;
	if(!settingManager_setBool("cursor", TRUE))
		return ERROR;
	if(!settingManager_setProperty("cursor", cp))
		return ERROR;
	return OK;
}

BOOL handleInput(inputDevice* VDInput)
{
	if(coreInput_mouseDown(VD_MOUSE0))
	{
		//zoom in

		pmRange newRange = (pmRange)malloc(sizeof(mRange));
		newRange->pre = renderState;
		renderState = newRange;
		if (coreInput_getMouse_X() < backbuffer->w / 2)
		{
			renderState->re_max = (renderState->pre->re_max + renderState->pre->re_min) / 2.0f;
			renderState->re_min = renderState->pre->re_min;
		}
		else
		{
			renderState->re_min = (renderState->pre->re_max + renderState->pre->re_min) / 2.0f;
			renderState->re_max = renderState->pre->re_max;
		}
		if (coreInput_getMouse_Y() < backbuffer->h / 2)
		{
			renderState->img_max = (renderState->pre->img_max + renderState->pre->img_min) / 2.0f;
			renderState->img_min = renderState->pre->img_min;
		}
		else
		{
			renderState->img_min = (renderState->pre->img_max + renderState->pre->img_min) / 2.0f;
			renderState->img_max = renderState->pre->img_max;
		}
		isDrawn = false;
	}
	if(coreInput_mouseDown(VD_MOUSE1))
	{
		//zoom out
		if(renderState->pre != NULL)
		{
			pmRange preRange = renderState->pre;
			free(renderState);
			renderState = preRange;
			isDrawn = false;
		}
	}
	if(coreInput_keyDown(VD_Q) && coreInput_keyDown(VD_LCONTROL))
		//quit
		return FALSE;
	return TRUE;
}

BOOL doGameLogic()
{
	// do nothing
	return TRUE;
}

BOOL renderScene()
{
	if(!isDrawn)
		drawMandelbrot();
	return TRUE;
}