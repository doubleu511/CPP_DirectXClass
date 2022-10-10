// GDIProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <vector>
using namespace std;

#include "framework.h"
#include "GDIProject.h"
#include "PointPair.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

vector<PointPair> ptList;

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
    LoadStringW(hInstance, IDC_GDIPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPROJECT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPROJECT));
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
   //WS_OVERLAPPEDWINDOW
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// WM_KEYDOWN, WM_KEYUP => alt, F10 하고 시스템키는 감지 안함
// 어떤 이벤트에다가 값을 전달하려고 만들어 놓은것
// WM_KEYDOWN Wparam에는 키코드, lParam = 반복횟수, 스캔코드
// WM_CHAR

// 창이 줄어들거나 아래로 내려갔다와도 기존의 그림이 남아있게 (최근 5개까지만)


#define M 5
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HPEN hBluePen;
    static BOOL bDraw = FALSE;
    static POINT ptStart, ptEnd;
    static int index = 0;

    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 'Z':
            if (ptList.size() > 0)
            {
                if (index > 0)
                {
                    index--;
                    bDraw = TRUE;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            break;
        case 'X':
            if (ptList.size() > 0)
            {
                if (index < ptList.size())
                {
                    index++;
                    bDraw = TRUE;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            break;
        }
    }
    case WM_CREATE:
    {
        hBluePen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
    }
    break;
    case WM_LBUTTONDOWN:
    {
        ptStart.x = LOWORD(lParam);
        ptStart.y = HIWORD(lParam);
        ptEnd = ptStart;
    }
    break;
    case WM_LBUTTONUP:
    {
        bDraw = TRUE;

        PointPair pt(ptStart, ptEnd);

        ptList.push_back(pt);
        index = ptList.size();

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (wParam & MK_LBUTTON)
        {
            HDC hdc = GetDC(hWnd);
            // #1 이전 라인을 삭제하는 모드
            int oldMode = SetROP2(hdc, R2_NOTXORPEN); // 화면색상과 XOR 지우기
            HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hBluePen));

            MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
            LineTo(hdc, ptEnd.x, ptEnd.y);

            // #2 새로운 라인을 그리는 모드
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);

            MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
            LineTo(hdc, ptEnd.x, ptEnd.y);

            SelectObject(hdc, hOldPen);
            SetROP2(hdc, oldMode);
            ReleaseDC(hWnd, hdc);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (bDraw)
        {
            for (vector<PointPair>::iterator it = ptList.begin(); it != ptList.begin() + index; ++it)
            {
                MoveToEx(hdc, it->start.x, it->start.y, NULL);
                LineTo(hdc, it->end.x, it->end.y);

                Rectangle(hdc, it->start.x - 3, it->start.y - 3, it->start.x + 3, it->start.y + 3);
                Rectangle(hdc, it->end.x - 3, it->end.y - 3, it->end.x + 3, it->end.y + 3);
            }
            bDraw = FALSE;
        }

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        DeleteObject(hBluePen);
        PostQuitMessage(0);
        break;
    case WM_SIZE:
    {
        bDraw = TRUE;
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}