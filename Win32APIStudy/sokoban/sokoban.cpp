﻿// sokoban.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "sokoban.h"

#define MAX_LOADSTRING 100

Game* game = nullptr;

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
    LoadStringW(hInstance, IDC_SOKOBAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rtClient;
    TCHAR userMsg[256]; // 사용자에게 띄울 메시지를 저장해둘 변수
    switch (message)
    {
    case WM_CREATE:
    {
        SetRect(&rtClient, 0, 0, 900, BH * 18);

        AdjustWindowRect(&rtClient, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

        SetWindowPos(hWnd, NULL, 0, 0,
            rtClient.right - rtClient.left,
            rtClient.bottom - rtClient.top, SWP_NOMOVE | SWP_NOZORDER);

        game = new Game(hWnd, hInst);
        game->InitStage();
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        game->DrawScreen(hdc, ps); //전체적인 그리는건 여기서 끝낸다.

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        case VK_RIGHT:
        case VK_UP:
        case VK_DOWN:
            game->Move(wParam);
            if (game->TestEnd()) { //게임 오버 조건을 검사한결과 클리어다
                if (game->GotoNextStage()) {
                    wsprintf(userMsg,
                        _T("%d 스테이지를 풀었습니다.")_T("다음 스테이지로 이동합니다"),
                        game->GetCurrentStage() + 1);
                }
                else {
                    game->isEnding = true;
                    wsprintf(userMsg, _T("모든 스테이지를 클리어했습니다."));
                }
                MessageBox(hWnd, userMsg, _T("알림"), MB_OK);
            }
            break;
        case 'Q':
            DestroyWindow(hWnd);
            break;
        case 'R':
            game->InitStage();
            break;
        case 'N':
            game->GotoNextStage();
            break;
        case 'P':
            game->GotoPrevStage();
            break;
        case 'Z':
            if (game->Undo())
            {

            }
            // Undo => 이거에 맞춰서 안내문자도 출력해야한다.
            break;
        case 'X':
            if (game->Redo())
            {

            }
            // Redo
            break;
        case 'T':
            game->TestClear();
            break;
        default:
            break;
        }
    }
        break;
    case WM_DESTROY:
        delete game;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}