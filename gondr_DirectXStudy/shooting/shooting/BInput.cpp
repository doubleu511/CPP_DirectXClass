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
			dwStateKey = KeyState::KEY_DOWN; // ���� �� Ű�� ���ȴ�.
		}
		else
		{
			dwStateKey = KeyState::KEY_PUSH; // ��� �������ִ�.
		}
	}
	else
	{
		// Ű�� ������
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
	static HKL layout = GetKeyboardLayout(0); // ���� �������� Ű���� ���̾ƿ�
	static BYTE state[256];

	// GetKeyboardState�� Ű���尡 ���� �������� true�� ����
	if(GetKeyboardState(state) == FALSE)
		return 0;

	/*
		scanCode : ��ĵ�ϰ��� �ϴ� Ű
		mapType,
		Ű���� ���̾ƿ�

		MAPVK_VSC_TO_VK : ù���� �Ķ���͸� scanCode�� ����Ѵ�.
	*/
	UINT vk = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, layout);

	return ToAsciiEx(vk, scanCode, state, result.get(), 0, layout);
	/*
		����� 0, 1, 2 �� �ϳ�
		0 : ������ vk�� ���� Ű���忡 ������ �������� �ʴ´�.
		1 : ������ 1��, 2 : ������ 2��
		// result �����Ͱ� ����ġ�� ���� ��������� ĳ���Ͱ� ����.
	*/
}
