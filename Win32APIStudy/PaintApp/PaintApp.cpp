// PaintApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "windows.h"
#include <tchar.h>

#define MAX_LOADSTRING 100

// n : 정수
// cb : count of byte 바이트 수
// sz : string terminated zero 0으로 끝나는 문자열
// lpsz : long pointer string terminated zero
// h : handle


// TEXT, _T => 문자집합의 호환가능한 문자열로 변환해준다.
// LOWORD => 4바이트 정수의 아래쪽 2바이트를 뱉음
// HIWORD => 4바이트 정수의 위쪽 2바이트를 뱉음
// RGB => rgb값을 받아서 COLORREF 자료형으로 변환함

// UTF-8
// 영어는 1바이트(아스키), 유럽 특문(2바이트), 한글-한자-일어(3바이트)
// UTF-16 latin-sewendish, general-ci
// 모든 문자를 2바이트로


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = _T("Simple GGM Application");                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING] = _T("SimpleGGM");            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //MyRegisterClass(hInstance);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // 윈도우는 메시지 기반 프로그래밍
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // 애플리케이션 초기화를 수행합니다:
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //HACCEL hAccelTable = LoadAccelerators(hInstance, IDI_APPLICATION);

    MSG msg;

    // 게임 루프 UPDATE
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg); // 메시지 번역
        DispatchMessage(&msg);  // 메시지 처리
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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