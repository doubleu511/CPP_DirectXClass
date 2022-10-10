#include "Player.h"

Player::Player() : Obj(FPOINT{}, OBJECTSIZE{}), m_fSpeed(100.f), m_fBulletSpeed(600.f), m_maxHP(100.f)
{
	m_hp = m_maxHP;
	m_fAngle = 0.f;
	memset(&m_InputState, 0, sizeof(TInputState)); // �Է� ����ü �ʱ�ȭ
}

Player::Player(FPOINT pos, OBJECTSIZE size, float speed) : Obj(pos, size), m_fSpeed(speed), m_fBulletSpeed(600.f), m_maxHP(100.f)
{
	m_hp = m_maxHP;
	m_fAngle = 0.f;
	memset(&m_InputState, 0, sizeof(TInputState)); // �Է� ����ü �ʱ�ȭ
}

Player::~Player()
{
}

void Player::Init()
{
	HWND hwnd = g_sEngine->GetWindowHandle();
	GetClientRect(hwnd, &m_sceneRect);

	m_ImagePlayer.Load(_T("Data/airplane.bmp"));
	m_Size.width = m_ImagePlayer.GetBitmapInfo().bmWidth;
	m_Size.height = m_ImagePlayer.GetBitmapInfo().bmHeight;

	m_rect = make_shared<RECT>();
	m_rect->left = static_cast<int>(m_Pos.x);
	m_rect->right = m_rect->left + static_cast<int>(m_Size.width);
	m_rect->top = static_cast<int>(m_Pos.y);
	m_rect->bottom = m_rect->top + static_cast<int>(m_Size.height);
	m_fRotateSpeed = 90;

	// ���� ���� �Ѿ˰� �ڽĺ������ο� ���� ��������� ���úκ� ����.
}

void Player::Update(float deltaTime)
{
	if (g_sEngine == nullptr)
		return;

	g_sEngine->m_InputObj.KeyCheck(VK_LEFT, m_InputState.dwKeyLEFT);
	g_sEngine->m_InputObj.KeyCheck(VK_RIGHT, m_InputState.dwKeyRIGHT);
	g_sEngine->m_InputObj.KeyCheck(VK_UP, m_InputState.dwKeyUP);
	g_sEngine->m_InputObj.KeyCheck(VK_DOWN, m_InputState.dwKeyDOWN);
	g_sEngine->m_InputObj.KeyCheck(VK_SPACE, m_InputState.dwKeySPACE);

	g_sEngine->m_InputObj.KeyCheck(0x41, m_InputState.dwKeyA);
	g_sEngine->m_InputObj.KeyCheck(0x44, m_InputState.dwKeyD);

	if (m_InputState.dwKeyLEFT == KeyState::KEY_PUSH)
	{
		m_Pos.x -= m_fSpeed * deltaTime;
	}

	if (m_InputState.dwKeyRIGHT == KeyState::KEY_PUSH)
	{
		m_Pos.x += m_fSpeed * deltaTime;
	}

	if (m_InputState.dwKeyUP == KeyState::KEY_PUSH)
	{
		m_Pos.y -= m_fSpeed * deltaTime;
	}

	if (m_InputState.dwKeyDOWN == KeyState::KEY_PUSH)
	{
		m_Pos.y += m_fSpeed * deltaTime;
	}

	if (m_InputState.dwKeySPACE == KeyState::KEY_DOWN)
	{
		OutputDebugString(_T("����"));
	}

	if (GetCenter().x < 0)
	{
		m_Pos.x = 0 - m_Size.width / 2;
	}

	if (GetCenter().x > m_sceneRect.right)
	{
		m_Pos.x = m_sceneRect.right - m_Size.width / 2;
	}

	if (GetCenter().y < 0)
	{
		m_Pos.y = 0 - m_Size.height / 2;
	}

	if (GetCenter().y > m_sceneRect.bottom)
	{
		m_Pos.y = m_sceneRect.bottom - m_Size.height / 2;
	}

	if (m_InputState.dwKeyA == KeyState::KEY_PUSH)
	{
		// Ű�� ������ ������ ������Ʈ ���ǵ�� ��ŸŸ�ӿ� ���缭 ȸ���� ��Ų ����
		// m_fAngle�� �־���� �Ѵ�.
		m_fAngle -= m_fRotateSpeed * deltaTime;
	}
	if (m_InputState.dwKeyD == KeyState::KEY_PUSH)
	{
		m_fAngle += m_fRotateSpeed * deltaTime;
	}
	m_ImagePlayer.SetRotate(m_fAngle);

	m_rect->left = static_cast<int>(m_Pos.x);
	m_rect->right = m_rect->left + static_cast<int>(m_Size.width);
	m_rect->top = static_cast<int>(m_Pos.y);
	m_rect->bottom = m_rect->top + static_cast<int>(m_Size.height);
}

void Player::Render(HDC hdc, float deltaTime)
{
	m_ImagePlayer.DrawBitmap(hdc,
		static_cast<int>(m_Pos.x),
		static_cast<int>(m_Pos.y),
		static_cast<int>(m_Size.width),
		static_cast<int>(m_Size.height));
}

void Player::Release()
{
	m_ImagePlayer.Release();
}

void Player::CreateBullet(float angle)
{

}

shared_ptr<RECT> Player::GetRect()
{
	return m_rect;
}
