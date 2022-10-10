#pragma once
#include "Scene.h"
#include "BBasicSys.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

public:
	void Init() override;
	void Update(float deltaTime);
	void Render(HDC hdc, float deltaTime) override;
	void Release() override;

private:
	POINT m_StartBtnPos; // 시작 버튼의 위치
	RECT m_StartBtnRect; // 시작 버튼의 사각형 => 이건 나중에 (C)Button등의 클래스로 빼주면 더 좋다
	BImage m_ImageBackground;
	BImage m_ImageStartBtn;

	// 여기 입력감지 상태도 있어야 함
	TInputState m_InputState;
};

