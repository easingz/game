#include <Windows.h>
#include <Windowsx.h>

#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO
//#define PI_DEBUG
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


// C runtime includes
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <iostream>

#include "../../PI/gameCorePI.h"
#include "windowsVD.h"

#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600



#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000)? 0 : 1)


HINSTANCE hInst;								
TCHAR* WindowTitle = gameName;
TCHAR* WindowClass = _T("windowsGameApp");
HWND mainWindow = NULL;
static HDC mainWindowDC = NULL;
static pVirtualDevice windowsVD = NULL;
MSG msg;

int ClientWidth = WINDOW_WIDTH;
int ClientHeight = WINDOW_HEIGHT;




//forward declaration
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	MyRegisterClass(hInstance);

	if (!(InitInstance (hInstance, nCmdShow) && InitWindowsVD(&windowsVD)))
		return FALSE;

	gameCore_main(windowsVD);

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
   ShowCursor(false);

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
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			windowsVD->paused = true;
	
		}
		else
		{
			windowsVD->paused = false;
		}
		break;
	case WM_SIZE:
		// Save the new client area dimensions.
		ClientWidth  = LOWORD(lParam);
		ClientHeight = HIWORD(lParam);
		if( windowsVD )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				windowsVD->paused = true;

			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				windowsVD->paused = false;
				//onResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				//if( mMinimized )
				//{
				//	mAppPaused = false;
				//	mMinimized = false;
				//	onResize();
				//}

				// Restoring from maximized state?
				/*else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					onResize();
				}
				else if( mResizing )
				{*/
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				//else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				//{
				//	onResize();
				//}
			}
			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}