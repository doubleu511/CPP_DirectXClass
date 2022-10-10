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
	POINT m_StartBtnPos; // ���� ��ư�� ��ġ
	RECT m_StartBtnRect; // ���� ��ư�� �簢�� => �̰� ���߿� (C)Button���� Ŭ������ ���ָ� �� ����
	BImage m_ImageBackground;
	BImage m_ImageStartBtn;

	// ���� �Է°��� ���µ� �־�� ��
	TInputState m_InputState;
};

