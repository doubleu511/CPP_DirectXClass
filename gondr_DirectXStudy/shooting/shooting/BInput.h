#pragma once
#include "BBasicStd.h"

enum class KeyState
{
	KEY_FREE = 0,
	KEY_PUSH = 1,
	KEY_UP = 2,
	KEY_DOWN = 3,
	KEY_HOLD = 4
};

struct TInputState
{
	KeyState dwKeyUP;
	KeyState dwKeyDOWN;
	KeyState dwKeyLEFT;
	KeyState dwKeyRIGHT;
	KeyState dwKeySPACE;
	KeyState dwKeyW;
	KeyState dwKeyA;
	KeyState dwKeyS;
	KeyState dwKeyD;
	KeyState dwMouseLB;
	KeyState dwMouseRB;
	KeyState dwKeyM;
};

class BInput
{
public:
	BInput();
	~BInput();

public:
	void Init();
	void Update();
	void Release();

	// 이녀석은 체크를 원하는 키의 코드를 보내면 해당키의 상태를 KeyState에 기록해서 준다.
	void KeyCheck(DWORD dwCheckKey, KeyState& dwStateKey);
	int ScanToASCII(DWORD scanCode, shared_ptr<WORD> result);
};

