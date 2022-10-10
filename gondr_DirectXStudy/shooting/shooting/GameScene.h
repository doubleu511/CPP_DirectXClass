#pragma once

#include "Scene.h"
#include "BBasicSys.h"
// �÷��̾�, ���׸�, ���Ŵ���, ����Ʈ �Ŵ���
#include "Player.h"
#include "Background.h"
#include "Bullet.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc, float deltaTime);
	void Release();


private:
	shared_ptr<Background> m_background;
	shared_ptr<Player> m_player;
	shared_ptr<Bullet> m_bullet;
};