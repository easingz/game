#ifndef _WINDOWS_VD_H_
#define _WINDOWS_VD_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
// C runtime includes
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#define TWEAK_MOUSE_BUFFER_MODE 0
//gameCore PI included
#include "../../PI/gameCorePI.h"

extern int ClientWidth;
extern int ClientHeight;
extern int keyMap[256];
extern MSG msg;
extern HWND mainWindow;

BOOL                InitWindowsVD(pVirtualDevice*);
void                releaseWindowsVD(void);

#endif