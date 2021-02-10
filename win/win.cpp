#include "win.h"
//#include <rpe.hpp>

#define MAX_LOADSTRING 100

HINSTANCE g_hInst;                                  // current instance
WCHAR g_szTitle[ MAX_LOADSTRING ];                  // The title bar text
WCHAR g_szWindowClass[ MAX_LOADSTRING ];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass( HINSTANCE hInstance );
BOOL                InitInstance( HINSTANCE, int );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK    about( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK setFormula(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK setVariablesViaTruthTable(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK setVariablesRandomly(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK setVariablesInBinaryMode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK setVariablesInDecimalMode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain (
        _In_     HINSTANCE hInstance
    ,   _In_opt_ HINSTANCE hPrevInstance
    ,   _In_     LPWSTR lpCmdLine
    ,   _In_     int nCmdShow
)
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW( hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_WIN, g_szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Perform application initialization:
    if ( !InitInstance ( hInstance, nCmdShow ) )
    {
        return FALSE;
    }

    HACCEL hAccelTable =
        LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_WIN ) );

    MSG msg;

    // Main message loop:
    while ( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return static_cast< int >( msg.wParam );
}

//----------------------------------------------------------------------------//

// Registers the window class.
ATOM MyRegisterClass ( HINSTANCE hInstance )
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof( WNDCLASSEX );

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_WIN ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = reinterpret_cast< HBRUSH >( COLOR_WINDOW + 1 );
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_WIN );
    wcex.lpszClassName  = g_szWindowClass;
    wcex.hIconSm        =
        LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassExW( &wcex );
}

//----------------------------------------------------------------------------//

// Saves instance handle and creates main window
// Also:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
BOOL InitInstance ( HINSTANCE hInstance, int nCmdShow )
{
    g_hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(
            g_szWindowClass
        ,   g_szTitle
        ,   WS_OVERLAPPEDWINDOW
        ,   CW_USEDEFAULT
        ,   0
        ,   CW_USEDEFAULT
        ,   0
        ,   nullptr
        ,   nullptr
        ,   hInstance
        ,   nullptr
    );

    if ( !hWnd )
    {
        return FALSE;
    }

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return TRUE;
}

//----------------------------------------------------------------------------//

// Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD( wParam );
            // Parse the menu selections:
            switch ( wmId )
            {
            case IDM_ABOUT:
                DialogBox( g_hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, about );
                break;

            case IDM_EXIT:
                DestroyWindow( hWnd );
                break;

            case IDM_CALCULATOR_SETFORMULA:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, setFormula);
                break;
            
            case IDM_SETVARIABLES_VIATRUTHTABLE:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, setVariablesViaTruthTable);
                break;
            
            case IDM_SETVARIABLES_RANDOMLY:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, setVariablesRandomly);
                break;
            
            case IDM_SETVARIABLES_INBINARYMODE:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, setVariablesInBinaryMode);
                break;
            
            case IDM_SETVARIABLES_INDECIMALMODE:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, setVariablesInDecimalMode);
                break;
            
            case IDM_CALCULATOR_CALCULATE:
                /*if ( !calculate() )
                    MessageBox(
                            hWnd
                        ,   L"Can't calculate because nothing have been set"
                        ,   L"Calculation Error"
                        ,   MB_ICONERROR
                    );*/
                break;
            
            case IDM_CALCULATOR_GET:
                // MessageBox with big message string with results
                break;
            
            case IDM_CALCULATOR_GETCLASSIFICATION:
                /*if ( !classificable() )
                    MessageBox(
                        hWnd
                        ,   L"Can't get classification"
                        ,   L"Classification Error"
                        ,   MB_ICONERROR
                    );*/
                // else: MessageBox with big message string with classification
                break;

            default:
                return DefWindowProc( hWnd, message, wParam, lParam );
            }
        }
        break;
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );
            // TODO: Add any drawing code that uses hdc here...
            EndPaint( hWnd, &ps );
        }
        break;
    
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

//----------------------------------------------------------------------------//

// Message handler for about box.
INT_PTR CALLBACK about ( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );

    switch ( message )
    {
    case WM_INITDIALOG:
        return ( INT_PTR )TRUE;

    case WM_COMMAND:
        if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return ( INT_PTR )TRUE;
        }
        break;
    }

    return ( INT_PTR )FALSE;
}

//----------------------------------------------------------------------------//

INT_PTR CALLBACK setFormula(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        // Use child "EDIT" window to write the formula
        break;
    }

    return (INT_PTR)FALSE;
}

//----------------------------------------------------------------------------//

INT_PTR CALLBACK setVariablesViaTruthTable(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        // Use child "EDIT" window to fill variables with the truth table
        break;
    }

    return (INT_PTR)FALSE;
}

//----------------------------------------------------------------------------//

INT_PTR CALLBACK setVariablesRandomly(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        // Use child "EDIT" window to fill variables with random values
        break;
    }

    return (INT_PTR)FALSE;
}

//----------------------------------------------------------------------------//

INT_PTR CALLBACK setVariablesInBinaryMode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        // Use child "EDIT" window to fill variables with binary values
        break;
    }

    return (INT_PTR)FALSE;
}

//----------------------------------------------------------------------------//

INT_PTR CALLBACK setVariablesInDecimalMode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        // Use child "EDIT" window to fill variables with decimal values
        break;
    }

    return (INT_PTR)FALSE;
}
