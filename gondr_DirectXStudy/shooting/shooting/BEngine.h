#pragma once
#include "BWindow.h"
#include "BImage.h"
#include "SceneManager.h"
#include "BInput.h"

// 여기에 입력, 이미지, 사운드 매니저 등의 기능이 추가적으로 들어간다.

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
	void RenderText(UINT fps, float deltaTime); // 디버깅용 fps 표기

public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc, float deltaTime);
	virtual void Release();

private:
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llFrequency; // 이게 델타타임을 구하기 위한 변수들

	float m_fDT; // 프레임간의 시간을 저장한다(델타타임)
	float m_fAcc; // 1초 체크를 위한 누적시간
	UINT m_iCallCount; // 함수 호출 횟수
	UINT m_iFPS; // 초당 호출횟수 저장

public:
	shared_ptr<SceneManager> m_SceneManager;
	BInput m_InputObj;
};

