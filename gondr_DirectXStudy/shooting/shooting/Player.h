#pragma once
#include "Obj.h"
#include "BBasicSys.h"

class Player : public Obj, public enable_shared_from_this<Player>
{
public:
	Player();
	Player(FPOINT pos, OBJECTSIZE size, float speed);
	~Player();

public:
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc, float deltaTime) override;
	void Release() override;

public:
	void CreateBullet(float angle);
	shared_ptr<RECT> GetRect();
	FPOINT GetPos() { return m_Pos; }
	void SetHP(float hp) { m_hp = hp; }
	float GetHP() { return m_hp; }
	void AddHP(float hp) { m_hp += hp; }

private:
	float m_fSpeed;
	float m_fBulletSpeed;

	shared_ptr<RECT> m_rect;
	float m_hp;
	float m_maxHP;

	RECT m_sceneRect;

	TInputState m_InputState;
	BImage m_ImagePlayer;

	//bool m_hadChild;
	float m_fRotateSpeed;
};

