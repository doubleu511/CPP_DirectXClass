// Timer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Timer.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIMER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMER));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SendMessage(hWnd, WM_TIMER, 3, 0);

   return TRUE;
}

// SetTimer, KillTimer => 게임루프가 없는 게임
#define STEP 10
#define R 20

POINT ptCircle;
RECT rtClient; // 화면 영역을 저장할 RECT
int direction = VK_RIGHT;

void __stdcall TimerProc(HWND hWnd, UINT uMsg, UINT_PTR nID, DWORD dwTime)
{
    switch (direction)
    {
    case VK_LEFT:
        if (ptCircle.x <= R)
        {
            direction = VK_RIGHT;
        }
        else
        {
            ptCircle.x -= STEP;
        }
        break;
    case VK_RIGHT:
        if (ptCircle.x >= rtClient.right - R)
        {
            direction = VK_LEFT;
        }
        else
        {
            ptCircle.x += STEP;
        }
        break;
    case VK_UP:
        if (ptCircle.y <= R)
        {
            direction = VK_DOWN;
        }
        else
        {
            ptCircle.y -= STEP;
        }
        break;
    case VK_DOWN:
        if (ptCircle.y >= rtClient.bottom - R)
        {
            direction = VK_UP;
        }
        else
        {
            ptCircle.y += STEP;
        }
    }

    InvalidateRect(hWnd, &rtClient, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 50, static_cast<TIMERPROC>(TimerProc)); // 마지막 타이머 프로시져가 NULL이면 주어진 시간마다 Wnd로 WM_TIMER라는 메시지를 발행
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            direction = VK_LEFT;
            break;
        case VK_RIGHT:
            direction = VK_RIGHT;
            break;
        case VK_UP:
            direction = VK_UP;
            break;
        case VK_DOWN:
            direction = VK_DOWN;
            break;
        }
    }
        break;
    case WM_SIZE:
    {
        GetClientRect(hWnd, &rtClient);
        ptCircle.x = rtClient.right / 2;
        ptCircle.y = rtClient.bottom / 2;
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            Ellipse(hdc, ptCircle.x - R, ptCircle.y - R, ptCircle.x + R, ptCircle.y + R);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}