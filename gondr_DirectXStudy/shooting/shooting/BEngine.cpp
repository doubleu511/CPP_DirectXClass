#include "BEngine.h"

shared_ptr<BEngine> g_sEngine = nullptr; // 전역변수로 언제든 참조할 수 있게 엔진 만들어주고

BEngine::BEngine() : m_llPrevCount{}, m_llCurCount{}, m_llFrequency{}, m_fDT(0.0f), m_fAcc(0.f), m_iCallCount(0), m_iFPS(0)
{
	// 씬 매니저 생성
	m_SceneManager = make_shared<SceneManager>();

	// 현재 CPU의 카운트 틱을 받아온다.
	QueryPerformanceCounter(&m_llPrevCount);

	// 현재 성능 카운터의 타이머 주파수변환 : 초당 카운트 수를 반환한다.
	QueryPerformanceFrequency(&m_llFrequency);
}

BEngine::~BEngine()
{

}

void BEngine::StartUp()
{
	g_sEngine = shared_from_this(); // 전역변수에 자기자신을 공유 할당해주고
	Init(); // 앱에서 초기화할 직업이 있다면 실행해서 초기화한다.

	if (m_SceneManager != nullptr)
	{
		m_SceneManager->Init();
	}
	// 이 함수는 딱 한번만 실행된다.

}

void BEngine::MainLoop()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	m_llPrevCount = m_llCurCount; // 카운트 갱신

	++m_iCallCount;
	m_fAcc += m_fDT; /// 누적시간 합산
	if (m_fAcc >= 1.0f)
	{
		m_iFPS = m_iCallCount;
		m_fAcc = 0;
		m_iCallCount = 0;

		RenderText(m_iFPS, m_fDT); // 윈도우 캡션창에 띄울꺼야
	}

	_Update(m_fDT);

	HDC hdc = GetDC(m_hWnd); // 윈도우 핸들로부터 DC를 얻어다가
	_Render(hdc, m_fDT); // 화면 그리기 들어가고

	ReleaseDC(m_hWnd, hdc); // 다 썼으면 반환
}

void BEngine::CleanUp()
{
	this->Release();

	// 입력, 사운드, 씬 등을 릴리즈 해줘야 한다.
}

void BEngine::_Update(float deltaTime)
{
	// 입력 업데이트
	// 씬 업데이트
	if (m_SceneManager != nullptr)
	{
		m_SceneManager->Update(deltaTime);
	}

	Update(deltaTime);
}

void BEngine::_Render(HDC hdc, float deltaTime)
{
	HDC hMemDC;
	RECT winRect;
	HBITMAP bitmap;

	GetClientRect(this->GetWindowHandle(), &winRect); // 현재 윈도우 핸들을 통해서 윈도우의 크기를 정확하게 알아내고

	hMemDC = CreateCompatibleDC(hdc); // 현재 HDC와 호환되는 메모리 DC를 하나 생성한다.

	bitmap = CreateCompatibleBitmap(hdc, winRect.right, winRect.bottom); // 호환 비트맵 생성후
	SelectObject(hMemDC, bitmap);
	FillRect(hMemDC, &winRect, (HBRUSH)COLOR_BACKGROUND); // 회색으로 사각형 그려주고

	if (m_SceneManager != nullptr)
	{
		m_SceneManager->Render(hMemDC, deltaTime);
	}

	Render(hMemDC, deltaTime); // 자식으로 이 메모리 DC를 넘겨준다. 자식은 실제론 이 메모리 DC에 그림을 덧대서 계속 그리게 된다.

	// 상속받은 녀석들이 bitmap 객체와 memDC에 그릴것들을 전부 그렸다면 BitBlt를 이용해서 실제 화면에 출력하도록 보내준다.
	BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(bitmap);
	DeleteDC(hMemDC);
}

void BEngine::RenderText(UINT fps, float deltaTime)
{
	static TCHAR szTemp[256];
	swprintf_s(szTemp, _T("Game FPS : %d, DT : %f"), fps, deltaTime);
	SetWindowText(m_hWnd, szTemp); // 타이틀에 이 내용이 출력되게 된다.
}

void BEngine::Init()
{
}

void BEngine::Update(float deltaTime)
{
}

void BEngine::Render(HDC hdc, float deltaTime)
{
}

void BEngine::Release()
{
}
