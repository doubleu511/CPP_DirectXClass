#pragma once
class MoveInfo
{
public:
	char dx : 3; // ��Ʈ �ʵ�
	char dy : 3;
	char bWithPack : 2;
	MoveInfo(int dx, int dy, bool bWithPack)
	{
		this->dx = dx;
		this->dy = dy;
		this->bWithPack = bWithPack;
		// ��Ʈ �ʵ�� ���ϴ� ��Ʈ�� �߶� �������� ������ �ϴ� ��Ʈ�� �߷�������.
	}
};

