#pragma once
#include "BBasicStd.h"

class Obj
{
public:
	Obj() = default;
	virtual ~Obj() = default;

	virtual void Init() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void Render(HDC hdc, float deltaTime) abstract;
	virtual void Release() abstract;

	virtual float GetAngle();
	virtual void SetAngle(float value);
	virtual FPOINT GetCenter();
protected:
	Obj(FPOINT pos, OBJECTSIZE size): m_Pos(pos), m_Size(size), m_fAngle(0) {}

protected:
	FPOINT m_Pos;
	OBJECTSIZE m_Size;
	float m_fAngle;
};

