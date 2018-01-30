// DAT154-Assignment2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DAT154-Assignment2.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

struct StateInfo {
	trasim::simulator sim;
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DAT154ASSIGNMENT2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAT154ASSIGNMENT2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   // Structure for managing persistent state
   StateInfo *si = new StateInfo();
   if (!si) return FALSE;

   auto h_pos = 500;
   auto v_pos = 300;
   si->sim = trasim::simulator{};
   si->sim.set_signal_positions(h_pos, v_pos);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, si);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

inline StateInfo *GetWindowState(HWND hWnd) {
	LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	StateInfo *stateInfo = reinterpret_cast<StateInfo*>(ptr);
	return stateInfo;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	StateInfo *si = GetWindowState(hWnd);
	CREATESTRUCT *pCreate;

	switch (message)
	{
	case WM_CREATE:
		// Remember application state
		pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		si = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)si);

		SetTimer(hWnd, 0, 250, nullptr);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		draw_traffic_light(&si->sim.horizontal_signal(), {450, 350}, 0.2f, hdc);
		draw_traffic_light(&si->sim.vertical_signal(), { 550, 200 }, 0.2f, hdc);
		draw_cars(si->sim.horizontal_cars(), hdc);
		draw_cars(si->sim.vertical_cars(), hdc);
		draw_signal_lines(si->sim.horizontal_signal_position(), si->sim.vertical_signal_position(), hdc);
		draw_status_text(&si->sim, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		// Delete application state
		delete si;
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			si->sim.dec_w_spawn();
			break;
		case VK_RIGHT:
			si->sim.inc_w_spawn();
			break;
		case VK_UP:
			si->sim.inc_n_spawn();
			break;
		case VK_DOWN:
			si->sim.dec_n_spawn();
			break;
		case VK_ESCAPE:
			delete si;
			PostQuitMessage(0);
		case VK_SPACE:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, (DLGPROC)SettingsDlgProc, (LPARAM)si);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		si->sim();
		InvalidateRect(hWnd, nullptr, TRUE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

