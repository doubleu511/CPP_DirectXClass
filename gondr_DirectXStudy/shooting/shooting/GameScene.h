#pragma once

#include "Scene.h"
#include "BBasicSys.h"
// 플레이어, 배경그림, 적매니저, 이펙트 매니저
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