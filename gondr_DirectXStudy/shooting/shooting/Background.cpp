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
	// ��� �Ʒ��� �������� �ȴ�. �׷��� ����ũ�⸸ŭ �Ѿ�� ����
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
