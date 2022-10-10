#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	m_background = make_shared<Background>(FPOINT{ 0, 0 }, OBJECTSIZE{});
	if (m_background != nullptr)
	{
		m_background->Init();
	}

	m_player = make_shared<Player>(FPOINT{ SCREEN_WIDTH / 2, 500 }, OBJECTSIZE{}, 200.f);
	if (m_player != nullptr)
	{
		m_player->Init();
	}

	FPOINT pos{ rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT };
	m_bullet = make_shared<Bullet>(pos, OBJECTSIZE{}, _T("Data/Bullet04.bmp"));
	m_bullet->Init();
}

void GameScene::Update(float deltaTime)
{
	if (m_background != nullptr)
	{
		m_background->Update(deltaTime);
	}

	if (m_player != nullptr)
	{
		m_player->Update(deltaTime);
	}

	m_bullet->Update(deltaTime);

	// 총알 충돌검사 해줘야한다.
	if (m_bullet->IsCollision(m_player))
	{
		OutputDebugString(_T("총알이 영 좋지 않은 곳에....\n"));
	}
}

void GameScene::Render(HDC hdc, float deltaTime)
{
	if (m_background != nullptr)
	{
		m_background->Render(hdc, deltaTime);
	}

	if (m_player != nullptr)
	{
		m_player->Render(hdc, deltaTime);
	}

	m_bullet->Render(hdc, deltaTime);
}

void GameScene::Release()
{
	if (m_background != nullptr)
	{
		m_background->Release();
	}

	if (m_player != nullptr)
	{
		m_player->Release();
	}
}
