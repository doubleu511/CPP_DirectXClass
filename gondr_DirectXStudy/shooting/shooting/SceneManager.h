#pragma once

#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	void RegisterScene(const string& sceneName, shared_ptr<Scene> scene);
	void ReserveChangeScene(const string& sceneName);

public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc, float deltaTime);
	void Release();

private:
	map<string, shared_ptr<Scene>> m_SceneContainer; // 이름과 포인터로 씬관리

	shared_ptr<Scene> m_ReservedScene;
	shared_ptr<Scene> m_CurrentScene;
};