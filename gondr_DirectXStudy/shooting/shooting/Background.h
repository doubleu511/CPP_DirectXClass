#pragma once
#include "Obj.h"
#include "BImage.h"

class Background : public Obj
{
public:
	Background();
	Background(FPOINT pos, OBJECTSIZE size);
	~Background();

public:
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc, float deltaTime) override;
	void Release() override;

private:
	BImage m_ImageBG;
	float m_fScrollY;
	float m_fScrollSpeed;
};

