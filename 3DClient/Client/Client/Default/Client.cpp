#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include "System.h"



#include "Timer_Mgr.h"
#include "Frame_Mgr.h"
#include "Input.h"
#include "Device.h"
#define MAX_LOADSTRING 100


HINSTANCE g_hInst;
WCHAR szTitle[MAX_LOADSTRING];                
WCHAR szWindowClass[MAX_LOADSTRING];           
HWND g_hWnd;
IDirect3DSurface9*  offscreenSurface = nullptr;
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
	// For.CRTDBG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;
	CMainApp*		pMainApp = nullptr;
	pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

	CSystem*			pSystem = CSystem::GetInstance();
	if (nullptr == pSystem)
		return FALSE;
	pSystem->AddRef();

	if (FAILED(pSystem->Add_Timer(L"Timer_Default")))
		return FALSE;

	// For.Timer_60
	if (FAILED(pSystem->Add_Timer(L"Timer_60")))
		return FALSE;

	// For.Frame_60
	if (FAILED(pSystem->Add_Frame(L"Frame_60", 144.0f)))
		return FALSE;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (nullptr == pMainApp)
			return FALSE;

		_float		fTimeDelta_Default = pSystem->Get_TimeDelta(L"Timer_Default");

		if (true == pSystem->Permit_Call(L"Frame_60", fTimeDelta_Default))
		{
			_float		fTimeDelta_60 = pSystem->Get_TimeDelta(L"Timer_60");

			pMainApp->Update_MainApp(fTimeDelta_60);
			pMainApp->Render_MainApp();
		}
	}
	_ulong		dwRefCnt = 0;
	Safe_Release(pSystem);

	if (dwRefCnt = Safe_Release(pMainApp))
		_MSG_BOX("CMainApp Release Failed");

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	RECT			rcClient = { 0, 0, g_iBackCX, g_iBackCY };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_hWnd = hWnd;

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        int x, y, width, height;
        RECT rtDesk, rtWindow;
        GetWindowRect(GetDesktopWindow(), &rtDesk);
        GetWindowRect(hWnd, &rtWindow);

        width = rtWindow.right - rtWindow.left;
        height = rtWindow.bottom - rtWindow.top;

        x = (rtDesk.right - width) / 2;
        y = (rtDesk.bottom - height) / 2;
        x += 600;

        MoveWindow(hWnd, x, y, width, height, TRUE);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
