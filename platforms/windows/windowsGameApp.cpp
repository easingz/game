#include <Windows.h>
#include <Windowsx.h>

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


// C runtime includes
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

//gameCore PI included
#include "../../PI/gameCorePI.h"

#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000)? 0 : 1)


HINSTANCE hInst;								
TCHAR* WindowTitle = _T("My Game");					
TCHAR* WindowClass = _T("windowsGameApp");
static HWND mainWindow = NULL;
static HDC mainWindowDC = NULL;
static pVirtualDevice windowsVD = NULL;
MSG msg;
extern int keyMap[256];

// Directx related
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3dDevice = NULL;
LPDIRECT3DSURFACE9 backBuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;
LPDIRECTINPUT8 DIInput;
LPDIRECTINPUTDEVICE8 DIMouse;
LPDIRECTINPUTDEVICE8 DIKeyboard;
DIMOUSESTATE mouseState;
// MUST be 8bit for keyboard state retrieve
int8_t keyState[256];

//forward declaration
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	            WindowsMessageHandle(void);
BOOL                InitWindowsVD(void);
BOOL                InitD3D(void);
BOOL                d3dBeginScene(void);
void                d3dEndScene(void);
void                d3dLockSurface(void);
void                d3dUnlockSurface(void);
void                d3dPresent(void);
void                WindowsKeyHandle(void);
int64_t             WindowsGetTime(void);
void                releaseD3D(void);
void                releaseWindowsVD(void);
void                DIGetInputState(void);
int32_t             DIKeyDown(VD_KEY);
int32_t             DIButtonDown(VD_MOUSE);
int32_t             DIMouse_X(void);
int32_t             DIMouse_Y(void);
BOOL                InitDIInput(void);
void                ReleaseDIInput(void);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	MyRegisterClass(hInstance);

	if (!(InitInstance (hInstance, nCmdShow) && InitD3D() && InitWindowsVD()))
		return FALSE;

	gameCore_main(windowsVD);

	releaseD3D();
	releaseWindowsVD();
	return (int) msg.wParam;
}

// 注册窗口类
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    //wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//   创建窗口并显示	 
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

   hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
       0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   mainWindow = hWnd;
   mainWindowDC = GetDC(mainWindow);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL InitD3D()
{
	if (!(d3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(mainWindow, _T("Error create Direct3D interface"), _T("Error"), MB_OK);
		return FALSE;
	}

	// set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dParam;
	ZeroMemory(&d3dParam, sizeof(d3dParam));
	d3dParam.Windowed = TRUE;
	d3dParam.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dParam.BackBufferCount = 1;
	d3dParam.BackBufferWidth = WINDOW_WIDTH;
	d3dParam.BackBufferHeight = WINDOW_HEIGHT;
	d3dParam.hDeviceWindow = mainWindow;
	// create Direct3D device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mainWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParam, &d3dDevice);
	if(!d3dDevice)
	{
		MessageBox(mainWindow, _T("Error create Direct3D device"), _T("Error"), MB_OK);
		return FALSE;
	}

	//clear backbuffer to black
	//d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	//get backbuffer
	d3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	//get surface
	HRESULT ret = d3dDevice->CreateOffscreenPlainSurface(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&surface,
		NULL);
	if(!SUCCEEDED(ret))
		return FALSE;
	if (!InitDIInput())
		return FALSE;
	return TRUE;
}

void releaseD3D(void)
{
	if (surface)
		surface->Release();
	if (d3dDevice)
		d3dDevice->Release();
	if (backBuffer)
		backBuffer->Release();
	ReleaseDIInput();
}

BOOL InitDIInput(void)
{
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DIInput,
		NULL);
	if (!SUCCEEDED(ret))
	{
		MessageBox(mainWindow, _T("Error create DirectInput8 interface"), _T("Error"), MB_OK);
		return FALSE;
	}

	// keyboard
	if(FAILED(DIInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL)))
		MessageBox(mainWindow, _T("Error create DirectInput8 keyboard device"), _T("Error"), MB_OK);
	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	// need DISCL_EXCLUSIVE for fullscreen to inprove performance
	DIKeyboard->SetCooperativeLevel(mainWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	DIKeyboard->Acquire();
	
	// mouse
	if(FAILED(DIInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL)))
		MessageBox(mainWindow, _T("Error create DirectInput8 mouse device"), _T("Error"), MB_OK);;
	DIMouse->SetDataFormat(&c_dfDIMouse);
	// need DISCL_EXCLUSIVE for fullscreen to inprove performance
	DIMouse->SetCooperativeLevel(mainWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	DIMouse->Acquire();
	// show cursor?
	d3dDevice->ShowCursor(FALSE);

	return TRUE;
}

void ReleaseDIInput(void)
{
	if(DIKeyboard)
	{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
		DIKeyboard = NULL;
	}
	if(DIMouse)
	{
		DIMouse->Unacquire();
		DIMouse->Release();
		DIMouse = NULL;
	}
}

int32_t DIKeyDown(VD_KEY vd_key_code)
{
	return keyState[keyMap[vd_key_code]] & 0x80;
}

int32_t DIButtonDown(VD_MOUSE vd_mouse_code)
{
	return mouseState.rgbButtons[vd_mouse_code] & 0x80;
}

void DIGetInputState()
{
	//for debug, see error type
	HRESULT ret;
	//update mouse state
	while(FAILED(ret = DIMouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState)))
		DIMouse->Acquire();

	//update keyboard state
	while(FAILED(ret = DIKeyboard->GetDeviceState(sizeof(keyState),(LPVOID)keyState)))
		DIKeyboard->Acquire();
}

int32_t DIMouse_X()
{
	return mouseState.lX;
}

int32_t DIMouse_Y()
{
	return mouseState.lY;
}

BOOL InitWindowsVD()
{
	windowsVD = (virtualDevice*)malloc(sizeof(virtualDevice));
	
	windowsVD->surface = (Surface*)malloc(sizeof(Surface));
	windowsVD->surface->lockSurface = d3dLockSurface;
	windowsVD->surface->unlockSurface = d3dUnlockSurface;
	windowsVD->surface->beginFrame = d3dBeginScene;
	windowsVD->surface->endFrame = d3dEndScene;
	windowsVD->surface->present = d3dPresent;
	windowsVD->timer_tik = WindowsGetTime;
	windowsVD->platformMessageHandling = WindowsMessageHandle;
	windowsVD->platformKeyHandling = WindowsKeyHandle;
	windowsVD->input = (inputDevice*)malloc(sizeof(inputDevice));
	windowsVD->input->getInputState = DIGetInputState;
	windowsVD->input->keyDown = DIKeyDown;
	windowsVD->input->buttonDown = DIButtonDown;
	windowsVD->input->mouse_X = DIMouse_X;
	windowsVD->input->mouse_Y = DIMouse_Y;

	return TRUE;
}

void releaseWindowsVD()
{
	free(windowsVD->surface);
	free(windowsVD->input);
	free(windowsVD);
}

int64_t WindowsGetTime()
{
	//in windows API defination, it suggest using GetTickCount64 for longer time using, or will get infinate loop
	//when running more than 49 days
	return GetTickCount64();
}

BOOL d3dBeginScene()
{
	return d3dDevice->BeginScene();
}

void d3dEndScene()
{
	RECT rect;
	rect.left = 0;
	rect.right = WINDOW_WIDTH;
	rect.top = 0;
	rect.bottom = WINDOW_HEIGHT;
	d3dDevice->StretchRect(surface, NULL, backBuffer, &rect, D3DTEXF_NONE);
	d3dDevice->EndScene();
}

void d3dPresent(void)
{
	d3dDevice->Present(0, 0, 0, 0);
}

void d3dLockSurface(void)
{
	unsigned char* buf;
	D3DLOCKED_RECT rectLock = {0};

	//Locking the drawing surface
	surface->LockRect(&rectLock,NULL,D3DLOCK_DONOTWAIT | D3DLOCK_NOSYSLOCK );

	//get the memeory mapping address
	buf = (BYTE*)rectLock.pBits;
	windowsVD->surface->buf = (void*)buf;
	windowsVD->surface->w = WINDOW_WIDTH;
	windowsVD->surface->h = WINDOW_HEIGHT;
	windowsVD->surface->pitch = rectLock.Pitch;
	windowsVD->surface->depth = 4;
}

void d3dUnlockSurface(void)
{
	windowsVD->surface->buf = NULL;
	surface->UnlockRect();
}

void WindowsKeyHandle(void)
{
	//if (KEYDOWN(VK_ESCAPE))
	//	PostMessage(mainWindow, WM_DESTROY, 0, 0);
}

BOOL WindowsMessageHandle(void)
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
        if (msg.message == WM_QUIT)
			return FALSE;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//  实际的消息处理函数

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}