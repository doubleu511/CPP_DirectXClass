#include "BInput.h"

BInput::BInput()
{
}

BInput::~BInput()
{
}

void BInput::Init()
{
}

void BInput::Update()
{
}

void BInput::Release()
{
}

void BInput::KeyCheck(DWORD dwCheckKey, KeyState& dwStateKey)
{
	if (GetAsyncKeyState(dwCheckKey) & 0x8000)
	{
		if (dwStateKey == KeyState::KEY_FREE)
		{
			dwStateKey = KeyState::KEY_DOWN; // 지금 막 키가 눌렸다.
		}
		else
		{
			dwStateKey = KeyState::KEY_PUSH; // 계속 눌리고있다.
		}
	}
	else
	{
		// 키가 떼졌다
		if (dwStateKey == KeyState::KEY_DOWN || dwStateKey == KeyState::KEY_PUSH)
		{
			dwStateKey = KeyState::KEY_UP;
		}
		else
		{
			dwStateKey = KeyState::KEY_FREE;
		}
	}
}

int BInput::ScanToASCII(DWORD scanCode, shared_ptr<WORD> result)
{
	static HKL layout = GetKeyboardLayout(0); // 현재 쓰레드의 키보드 레이아웃
	static BYTE state[256];

	// GetKeyboardState는 키보드가 뭐라도 눌렸으면 true를 리턴
	if(GetKeyboardState(state) == FALSE)
		return 0;

	/*
		scanCode : 스캔하고자 하는 키
		mapType,
		키보드 레이아웃

		MAPVK_VSC_TO_VK : 첫번쨰 파라미터를 scanCode로 취급한다.
	*/
	UINT vk = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, layout);

	return ToAsciiEx(vk, scanCode, state, result.get(), 0, layout);
	/*
		결과는 0, 1, 2 중 하나
		0 : 지정된 vk가 현재 키보드에 번역이 존재하지 않는다.
		1 : 번역이 1개, 2 : 번역이 2개
		// result 포인터가 가르치는 곳엥 결과번역된 캐릭터가 들어간다.
	*/
}
