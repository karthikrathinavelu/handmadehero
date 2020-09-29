#include <windows.h>

#define global static
#define local_persist static
#define local2file static

global BOOL Running;

LRESULT CALLBACK MainWindowCallback(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    LRESULT result = 0;

    switch (uMsg)
    {
        case WM_CREATE:
            OutputDebugString(TEXT("WM_CREATE\n"));
            break;

        case WM_SIZE:
            OutputDebugString(TEXT("WM_SIZE\n"));
            break;

        case WM_CLOSE:
            Running = FALSE;
            break;

        case WM_DESTROY:
            Running = FALSE;
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps = {};
            HDC deviceContext = BeginPaint(hWnd, &ps);
            int x = ps.rcPaint.left;
            int y = ps.rcPaint.top;
            int w = ps.rcPaint.right - ps.rcPaint.left;
            int h = ps.rcPaint.bottom - ps.rcPaint.top;
            local_persist DWORD rasterOperation = BLACKNESS;
            PatBlt(deviceContext, x, y, w, h, rasterOperation);
            if (rasterOperation == BLACKNESS)
            {
                rasterOperation = WHITENESS;
            }
            else
            {
                rasterOperation = BLACKNESS;
            }

            EndPaint(hWnd, &ps);
        }
            break;

        default:
            result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return result;
}

int WINAPI WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPSTR lpCmdLine,
    _In_     int nShowCmd
)
{
    LPCTSTR windowClassName = TEXT("HandmadeHeroWindowClass");

    WNDCLASSEX wndclass = {}; // Initializes to zero
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = MainWindowCallback;
    wndclass.lpszClassName = windowClassName;

    if (!RegisterClassEx(&wndclass))
    {
        // Log error
    }

    LPCTSTR windowName = TEXT("Handmade Hero");

    HWND window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                                windowClassName,
                                windowName,
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                NULL,
                                NULL,
                                hInstance,
                                NULL);

    if (!window)
    {
        // Log error
    }

    BOOL gmReturn;
    MSG msg;

    Running = TRUE;

    while (Running)
    {
        gmReturn = GetMessage(&msg, NULL, 0, 0);
        if (gmReturn == -1)
        {
            // handle the error and possibly exit
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	return 0;
}