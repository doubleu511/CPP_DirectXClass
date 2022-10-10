#pragma once
#include "Obj.h"
#include "BBasicSys.h"
#include "Player.h"

class Bullet: public Obj, public enable_shared_from_this<Bullet>
{
public:
	Bullet(FPOINT pos, OBJECTSIZE size, const TCHAR* filename);
	~Bullet();

public:
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc, float deltaTime) override;
	void Release() override;

public:
	bool IsCollision(shared_ptr<Player> player);
	RECT* GetRect();

private:
	RECT m_Rect;
	BImage m_ImageBullet;
};

