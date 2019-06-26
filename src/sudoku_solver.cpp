// sudoku_solver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sudoku_solver.h"
#include "Table.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    if (GetCommandLine())
    {
        // do everything on the command line, and exit
        Table tbl("table1.txt", "123456789");
        tbl.SetChar(0, 1, '2');
        tbl.SetChar(0, 6, '5');
        tbl.SetChar(0, 7, '6');

        tbl.SetChar(1, 0, '1');
        tbl.SetChar(1, 1, '6');
        tbl.SetChar(1, 3, '5');
        tbl.SetChar(1, 4, '7');
        tbl.SetChar(1, 7, '8');

        tbl.SetChar(2, 0, '7');
        tbl.SetChar(2, 3, '2');
        tbl.SetChar(2, 5, '3');
        tbl.SetChar(2, 6, '1');
        tbl.SetChar(2, 7, '4');
        tbl.SetChar(2, 8, '9');

        tbl.SetChar(3, 1, '9');
        tbl.SetChar(3, 3, '1');
        tbl.SetChar(3, 8, '2');
        
        tbl.SetChar(4, 0, '3');
        tbl.SetChar(4, 1, '1');
        tbl.SetChar(4, 2, '2');
        tbl.SetChar(4, 4, '4');
        tbl.SetChar(4, 6, '9');

        tbl.SetChar(5, 0, '6');
        tbl.SetChar(5, 2, '7');
        tbl.SetChar(5, 4, '5');
        tbl.SetChar(5, 7, '3');
        
        tbl.SetChar(6, 1, '7');
        tbl.SetChar(6, 2, '1');
        tbl.SetChar(6, 4, '8');
        tbl.SetChar(6, 7, '9');
        tbl.SetChar(6, 8, '4');

        tbl.SetChar(7, 1, '8');
        tbl.SetChar(7, 2, '4');
        tbl.SetChar(7, 3, '6');
        tbl.SetChar(7, 6, '3');
        tbl.SetChar(7, 7, '2');
        tbl.SetChar(7, 8, '5');

        tbl.SetChar(8, 8, '8');
        tbl.Print();

        for (size_t t = 0; t < 10; t++)
            if (tbl.ProcessTable())
                break;
        tbl.Print();

        Table tbl2("solver.txt", "123456789");
        tbl2.SetChar(0, 1, '5');
        tbl2.SetChar(0, 4, '3');
        tbl2.SetChar(0, 6, '4');

        tbl2.SetChar(1, 1, '3');
        tbl2.SetChar(1, 2, '8');
        tbl2.SetChar(1, 3, '9');
        tbl2.SetChar(1, 4, '4');

        tbl2.SetChar(2, 0, '9');
        tbl2.SetChar(2, 4, '8');
        tbl2.SetChar(2, 5, '6');
        
        tbl2.SetChar(3, 1, '9');
        tbl2.SetChar(3, 2, '4');
        tbl2.SetChar(3, 3, '3');
        tbl2.SetChar(3, 8, '5');

        tbl2.SetChar(4, 2, '1');
        tbl2.SetChar(4, 6, '6');

        tbl2.SetChar(5, 4, '9');
        tbl2.SetChar(5, 7, '3');
        tbl2.SetChar(5, 8, '1');

        tbl2.SetChar(6, 7, '7');

        tbl2.SetChar(7, 1, '7');
        tbl2.SetChar(7, 2, '9');
        tbl2.SetChar(7, 7, '4');

        tbl2.SetChar(8, 2, '3');
        tbl2.SetChar(8, 4, '6');

        tbl2.Print();

        for (size_t t = 0; t < 3; t++)
            if (tbl2.ProcessTable())
                break;
        tbl2.Print();

        return TRUE;
    }
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SUDOKUSOLVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUDOKUSOLVER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUDOKUSOLVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SUDOKUSOLVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
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
