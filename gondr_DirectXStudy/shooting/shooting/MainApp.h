#pragma once

#include "BBasicSys.h"

class MainApp : public BEngine
{
public:
	MainApp();
	~MainApp();
public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc, float deltaTime) override;
	virtual void Release() override;
};

