#include "../../core/includes/gamelib.h"

static Surface* buf;
static BOOL isDrawn = FALSE;

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
void drawMandelbrot(float re_min, float re_max, float img_min, float img_max,
                     int max_iter)
{
    int i,j,count;
    float realDelta = (re_max - re_min) / buf->w;
    float imagDelta = (img_max - img_min) / buf->h;

    complex z;
    //complex z = {-0.75, 0.0};
    //z.length = complexLength(&z);
    for (i = 0, z.real=re_min ; i < buf->w; i++,z.real+=realDelta)
    {
        for (j = 0, z.imag = img_min; j < buf->h; j++,z.imag+=imagDelta)
        {
            complex _x = {0, 0, 0.0};
            pComplex x = &_x;
            for (count = 0; count < max_iter; count++)
            {
                if(x->length > 2) break;
                complexMultiply(x, x, x);
                complexAdd(x, &z, x);
            }
            if(x->length <= 2.0)
			{
				setColor(buf, i, j, ARGB(0xff,0,0,0));
			}
            else
            {
                int depth =(int)( 765.0 * count / max_iter);
                int red = depth % 255;
                int green = depth % 510;
                int blue = depth % 755;
				setColor(buf, i, j, ARGB(0xff,red,green,blue));
            }
        }
    }
	isDrawn =TRUE;
}

void handleInput(virtualDevice* VD)
{
	// do nothing
}

void doGameLogic()
{
	// do nothing
}
void onDrawFrame(Surface* s)
{
	buf = s;
	if(!isDrawn)
		drawMandelbrot(-2.0, 1.0, -1.25, 1.25, 20);
}