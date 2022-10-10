#include "Background.h"

Background::Background() : Obj(FPOINT{}, OBJECTSIZE{}), m_fScrollY(0.f), m_fScrollSpeed(300.f) {}

Background::Background(FPOINT pos, OBJECTSIZE size) : Obj(pos, size), m_fScrollY(0.f), m_fScrollSpeed(300.f) {}

Background::~Background()
{
}

void Background::Init()
{
	m_ImageBG.Load(_T("Data/Background1.bmp"));
	m_Size.width = m_ImageBG.GetBitmapInfo().bmWidth;
	m_Size.height = m_ImageBG.GetBitmapInfo().bmHeight;
}

void Background::Update(float deltaTime)
{
	// 계속 아래로 내려가게 된다. 그러다 한장크기만큼 넘어가면 리셋
	m_fScrollY += m_fScrollSpeed * deltaTime;
	if (m_fScrollY >= m_Size.height)
	{
		m_fScrollY -= m_Size.height;
	}
}

void Background::Render(HDC hdc, float deltaTime)
{
	m_ImageBG.DrawBitmap(hdc, 0, static_cast<int>(m_fScrollY), 0, 0);
	m_ImageBG.DrawBitmap(hdc, 0, static_cast<int>(m_fScrollY - m_Size.height), 0, 0);
}

void Background::Release()
{
}
