#pragma once
#include "BBasicStd.h"

class BWindow
{
public:
	BWindow(); // 생성자
	~BWindow(); // 파괴자

public:
	int Run(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 이건 BWindow가 직접 해줘야할 일들
private:
	void RegisterWndClass();
	void WindowCreate();
	void WindowShow(int nCmdShow);
	int MessageLoop();

	// 이건 BWindow가 상속받은 녀석들이 구현해야하는 행동
public:
	virtual void StartUp() = 0;	// 추상 메서드 선언방법
	virtual void MainLoop() = 0;
	virtual void CleanUp() = 0;

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; // 멤버변수로 윈도우 핸들과 인스턴스 핸들을 가지고 있어야 한다.

public:
	HWND GetWindowHandle();	// 멤버변수 겟터
	HINSTANCE GetInstanceHandle();
};


