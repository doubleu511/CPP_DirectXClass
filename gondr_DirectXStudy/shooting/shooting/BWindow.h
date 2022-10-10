#pragma once
#include "BBasicStd.h"

class BWindow
{
public:
	BWindow(); // ������
	~BWindow(); // �ı���

public:
	int Run(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �̰� BWindow�� ���� ������� �ϵ�
private:
	void RegisterWndClass();
	void WindowCreate();
	void WindowShow(int nCmdShow);
	int MessageLoop();

	// �̰� BWindow�� ��ӹ��� �༮���� �����ؾ��ϴ� �ൿ
public:
	virtual void StartUp() = 0;	// �߻� �޼��� ������
	virtual void MainLoop() = 0;
	virtual void CleanUp() = 0;

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; // ��������� ������ �ڵ�� �ν��Ͻ� �ڵ��� ������ �־�� �Ѵ�.

public:
	HWND GetWindowHandle();	// ������� ����
	HINSTANCE GetInstanceHandle();
};


