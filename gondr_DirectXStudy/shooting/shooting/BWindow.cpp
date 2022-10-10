#include "BWindow.h"

BWindow::BWindow() : m_hWnd(nullptr), m_hInstance(nullptr)
{
	// do nothing
}

BWindow::~BWindow()
{
	// do nothing
}

// 윈도우를 띄워주고, 셋팅한다음에 무한루프로 들어가는 역할을 해주는 함수
int BWindow::Run(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	m_hInstance = hInstance;
	this->RegisterWndClass();
	this->WindowCreate();
	this->WindowShow(nCmdShow);

	return this->MessageLoop();
}

LRESULT BWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

void BWindow::RegisterWndClass()
{
	WNDCLASSEX winClassEx;
	winClassEx.cbSize = sizeof(WNDCLASSEX);
	winClassEx.style = CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc = BWindow::WndProc; // 메시지 핸들 프로시저 등록
	winClassEx.cbClsExtra = 0;
	winClassEx.cbWndExtra = 0;
	winClassEx.hInstance = m_hInstance;
	winClassEx.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	winClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 아마 회색
	winClassEx.lpszMenuName = nullptr;
	winClassEx.lpszClassName = BASIC_WINDOW_NAME;
	winClassEx.hIconSm = LoadIcon(m_hInstance, IDI_APPLICATION);

	RegisterClassEx(&winClassEx);
}

void BWindow::WindowCreate()
{
	// 클래스 이름, 윈도우 이름, 스타일, x, y, w, h, 부모, 메뉴, 인스턴스, lparam
	m_hWnd = CreateWindow(
		BASIC_WINDOW_NAME, _T("Framework"), WS_OVERLAPPEDWINDOW,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, m_hInstance, nullptr);
}

void BWindow::WindowShow(int nCmdShow)
{
	ShowWindow(m_hWnd, nCmdShow);
}

int BWindow::MessageLoop()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg)); // 전부 0으로 클린하게 만들고

	this->StartUp(); // 추상메서드 : 자식이 구현할꺼임

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->MainLoop(); // 윈도우 메시지가 없을 경우 메인루프 돌기
		}
	}

	this->CleanUp(); // 추상메서드 : 자식이 구현해서 자기가 만든걸 지울꺼임
	return (int)msg.wParam; // 종료코드를 리턴해주면 된다.
}

HWND BWindow::GetWindowHandle()
{

	return m_hWnd;
}

HINSTANCE BWindow::GetInstanceHandle()
{
	return m_hInstance;
}
