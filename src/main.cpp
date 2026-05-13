// main.cpp
// scriptusMedia v0.0.1
// Single-file Win32 dark mode media player UI prototype

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "Comctl32.lib")

// ----------------------------------------------------
// Globals
// ----------------------------------------------------

const wchar_t CLASS_NAME[] = L"scriptusMediaWindow";

HWND playButton;
HWND timelineBar;
HWND volumeSlider;

bool isPlaying = false;

// ----------------------------------------------------
// Dark Mode Colors
// ----------------------------------------------------

COLORREF BG_COLOR        = RGB(18, 18, 18);
COLORREF PANEL_COLOR     = RGB(28, 28, 28);
COLORREF TEXT_COLOR      = RGB(230, 230, 230);
COLORREF ACCENT_COLOR    = RGB(80, 80, 80);

HBRUSH bgBrush = CreateSolidBrush(BG_COLOR);
HBRUSH panelBrush = CreateSolidBrush(PANEL_COLOR);

// ----------------------------------------------------
// Toggle Play/Pause
// ----------------------------------------------------

void TogglePlayback()
{
    isPlaying = !isPlaying;

    SetWindowTextW(
        playButton,
        isPlaying ? L"Pause" : L"Play"
    );
}

// ----------------------------------------------------
// Window Procedure
// ----------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Play Button
            playButton = CreateWindowW(
                L"BUTTON",
                L"Play",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                20, 610, 100, 40,
                hwnd,
                (HMENU)1,
                NULL,
                NULL
            );

            // Timeline Slider
            timelineBar = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                L"",
                WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,
                140, 615, 900, 30,
                hwnd,
                (HMENU)2,
                NULL,
                NULL
            );

            SendMessage(timelineBar, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));

            // Volume Slider
            volumeSlider = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                L"",
                WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,
                1060, 615, 180, 30,
                hwnd,
                (HMENU)3,
                NULL,
                NULL
            );

            SendMessage(volumeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
            SendMessage(volumeSlider, TBM_SETPOS, TRUE, 80);

            return 0;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case 1:
                    TogglePlayback();
                    break;
            }

            return 0;
        }

        case WM_CTLCOLORBTN:
        {
            HDC hdc = (HDC)wParam;

            SetTextColor(hdc, TEXT_COLOR);
            SetBkColor(hdc, PANEL_COLOR);

            return (INT_PTR)panelBrush;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);

            // Background
            FillRect(hdc, &rect, bgBrush);

            // Video Area
            RECT videoRect = { 0, 0, rect.right, 590 };

            HBRUSH videoBrush = CreateSolidBrush(RGB(8, 8, 8));
            FillRect(hdc, &videoRect, videoBrush);
            DeleteObject(videoBrush);

            // Bottom Panel
            RECT panelRect = { 0, 590, rect.right, rect.bottom };
            FillRect(hdc, &panelRect, panelBrush);

            // Title
            SetTextColor(hdc, TEXT_COLOR);
            SetBkMode(hdc, TRANSPARENT);

            HFONT font = CreateFontW(
                24, 0, 0, 0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                VARIABLE_PITCH,
                L"Segoe UI"
            );

            SelectObject(hdc, font);

            TextOutW(hdc, 20, 20, L"scriptusMedia", 14);

            DeleteObject(font);

            EndPaint(hwnd, &ps);

            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ----------------------------------------------------
// WinMain
// ----------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    INITCOMMONCONTROLSEX icex;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;

    InitCommonControlsEx(&icex);

    WNDCLASSW wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = bgBrush;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"scriptusMedia 0.0.1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // ------------------------------------------------
    // Message Loop
    // ------------------------------------------------

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}