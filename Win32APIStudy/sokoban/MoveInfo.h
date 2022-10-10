#pragma once
class MoveInfo
{
public:
	char dx : 3; // 비트 필드
	char dy : 3;
	char bWithPack : 2;
	MoveInfo(int dx, int dy, bool bWithPack)
	{
		this->dx = dx;
		this->dy = dy;
		this->bWithPack = bWithPack;
		// 비트 필드는 원하는 비트만 잘라서 가져가기 때문에 하단 비트만 잘려나간다.
	}
};

