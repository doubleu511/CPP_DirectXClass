#include "SceneManager.h"

SceneManager::SceneManager() : m_CurrentScene(nullptr), m_ReservedScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	// ����� ��� ���� ���� �������ϰ� shared_ptr ����
	for (auto it = m_SceneContainer.begin(); it != m_SceneContainer.end(); ++it)
	{
		it->second->Release();
		it->second.reset();
	}

	m_SceneContainer.clear();
}

void SceneManager::RegisterScene(const string& sceneName, shared_ptr<Scene> scene)
{
	if (scene == nullptr || sceneName.compare("") == 0)
	{
		return; // �������� �ʴ´�.
	}

	scene->Init(); // �� �ʱ�ȭ
	m_SceneContainer[sceneName] = scene;
}

void SceneManager::ReserveChangeScene(const string& sceneName)
{
	auto it = m_SceneContainer.find(sceneName);

	if (it != m_SceneContainer.end())
	{
		m_ReservedScene = it->second;
	}
	else
	{
		m_ReservedScene = nullptr;
	}
}

void SceneManager::Init()
{
}

void SceneManager::Update(float deltaTime)
{
	if (m_CurrentScene)
		m_CurrentScene->Update(deltaTime);

	if (m_ReservedScene)
	{
		if (m_CurrentScene)
		{
			m_CurrentScene->Release();
		}

		m_CurrentScene = m_ReservedScene;
		m_CurrentScene->Init();
		m_ReservedScene = nullptr;
	}
}

void SceneManager::Render(HDC hdc, float deltaTime)
{
	if (m_CurrentScene)
		m_CurrentScene->Render(hdc, deltaTime);
}

void SceneManager::Release()
{

}
