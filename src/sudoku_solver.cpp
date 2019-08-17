// sudoku_solver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sudoku_solver.h"
#include "Table.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
 
enum Options {
    eCommand = 0,
    eLogTable,
    eEmpty,
    eValidChars,
    eBlockSize,
    eEntryTable,
    numOptions
};

void DisplayUsage(int argc, char** argv)
{
    std::cout << "Usage: " << argv[0] << " log.txt '*' 123456789 3 sudoku.txt" << endl;
}

char* ReadFile(char* filename, char* validChars, char* emptyChar)
{
    size_t numChar = strlen(validChars);
    size_t tableSize = numChar*numChar;
    char* values = new char[tableSize];
    size_t idx = 0;
    std::ifstream ifs (filename, std::ifstream::in);
    char c = ifs.get();
    std::cout << "Valid " << validChars << " empty " << emptyChar << " tableSize " << tableSize << endl;
    while (ifs.good()) 
    {
        if(c != ' ')
            std::cout << " " << c;
        for (size_t i = 0; i < strlen(validChars); i++)
        {
            if(c == validChars[i] || c == emptyChar[0])
            {
                values[idx] = c;
                if(idx + 1 >= tableSize)
                {
                    std::cout << endl;
                    ifs.close();
                    return values;
                }
                idx++;
                break;
            }
        }
        c = ifs.get();
    }
    std::cout << endl;
    ifs.close();
    if(idx != tableSize)
    {
        std::cout << "TableSize " << tableSize << " count " << idx << endl;
        delete[] values;
        values = NULL;
    }
    return values;
}

int main(int argc, char** argv) 
{
    // TestTable();
    if(argc < numOptions)
    {
        DisplayUsage(argc, argv);
        return 0;
    }
    char* logTable = argv[eLogTable];
    char* emptyChar = argv[eEmpty];
    char* validChars = argv[eValidChars];
    size_t blockSize = size_t(atoi(argv[eBlockSize]));
    char* entryTable = argv[eEntryTable];
    char* values = ReadFile(entryTable, validChars, emptyChar);
    if(!values)
    {
        std::cout << "Invalid entryTable" << endl;
        return 1;
    }
    Table tbl(logTable, emptyChar[0], validChars, blockSize);
    tbl.FillCells(values);
    tbl.Print();

    for (size_t t = 0; t < 100; t++)
    {
        if (tbl.ProcessTable())
        {
            std::cout << "Success!" << endl;
            tbl.Print(true);
            break;
        }
    }
    delete[] values;
    values = NULL;
    return 0;
}

#ifdef WINDOWS
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
        TestTable();
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
#endif // WINDOWS