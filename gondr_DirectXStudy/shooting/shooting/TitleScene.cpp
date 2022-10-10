#include "TitleScene.h"

TitleScene::TitleScene() : m_StartBtnPos{}, m_StartBtnRect{}
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	m_StartBtnPos = { 150, 400 }; // 이건 그냥 위치 찾아서 하드코딩 한거
	m_StartBtnRect = {};

	// 여기에 이미지 로딩하는게 들어와야 함
	m_ImageBackground.Load(_T("Data/BG02.bmp"));
	m_ImageStartBtn.Load(_T("Data/Start.bmp"));

	// 시작버튼은 크기 조정해야한다.
	m_StartBtnRect.left = m_StartBtnPos.x;
	m_StartBtnRect.right = m_StartBtnRect.left + m_ImageStartBtn.GetBitmapInfo().bmWidth;
	m_StartBtnRect.top = m_StartBtnPos.y;
	m_StartBtnRect.bottom = m_StartBtnRect.top + m_ImageStartBtn.GetBitmapInfo().bmHeight;
}

void TitleScene::Update(float deltaTime)
{
	// 입력감지해서 버튼 눌리면 씬체인지
	if (g_sEngine == nullptr)
		return;

	g_sEngine->m_InputObj.KeyCheck(VK_LBUTTON, m_InputState.dwMouseLB);

	if (m_InputState.dwMouseLB == KeyState::KEY_UP)
	{
		POINT point;
		GetCursorPos(&point);

		ScreenToClient(g_sEngine->GetWindowHandle(), &point);

		wstring msg = L"클릭 : x(" + to_wstring(point.x) + L"), y(" + to_wstring(point.y) + L") \n";

		// 마우스 왼쪽 버튼이 클릭되었다.
		OutputDebugString(msg.c_str());

		if (PtInRect(&m_StartBtnRect, point))
		{
			if (g_sEngine != nullptr && g_sEngine->m_SceneManager != nullptr)
			{
				g_sEngine->m_SceneManager->ReserveChangeScene("game");
			}
		}
	}
}

void TitleScene::Render(HDC hdc, float deltaTime)
{
	m_ImageBackground.DrawBitmap(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_ImageStartBtn.DrawBitmap(hdc, m_StartBtnPos.x, m_StartBtnPos.y, 0, 0);
}

void TitleScene::Release()
{
	m_ImageBackground.Release();
	m_ImageStartBtn.Release();
}
