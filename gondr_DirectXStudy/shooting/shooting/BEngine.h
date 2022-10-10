#pragma once
#include "BWindow.h"
#include "BImage.h"
#include "SceneManager.h"
#include "BInput.h"

// ���⿡ �Է�, �̹���, ���� �Ŵ��� ���� ����� �߰������� ����.

class BEngine : public BWindow, public enable_shared_from_this<BEngine>
{
public:
	BEngine();
	~BEngine();

public:
	virtual void StartUp() override;
	virtual void MainLoop() override;
	virtual void CleanUp() override;

	virtual void _Update(float deltaTime);
	virtual void _Render(HDC hdc, float deltaTime);

private:
	void RenderText(UINT fps, float deltaTime); // ������ fps ǥ��

public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc, float deltaTime);
	virtual void Release();

private:
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llFrequency; // �̰� ��ŸŸ���� ���ϱ� ���� ������

	float m_fDT; // �����Ӱ��� �ð��� �����Ѵ�(��ŸŸ��)
	float m_fAcc; // 1�� üũ�� ���� �����ð�
	UINT m_iCallCount; // �Լ� ȣ�� Ƚ��
	UINT m_iFPS; // �ʴ� ȣ��Ƚ�� ����

public:
	shared_ptr<SceneManager> m_SceneManager;
	BInput m_InputObj;
};

