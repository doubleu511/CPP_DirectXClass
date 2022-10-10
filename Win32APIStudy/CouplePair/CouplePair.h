#pragma once
#include "framework.h"
#include "resource.h"
#include <memory>
#include <vector>

using namespace std;

enum class Status
{
	HIDDEN = 1, //ī�尡 ������������
	FLIP = 2, //ī�尡 ����������
	TEMPFLIP = 3, //ī�尡 �ӽ÷� ����������
};

enum class GameStatus
{
	PAUSED = 0,
	RUN = 1,
	HINT = 2,
	VIEW = 3
};

struct CardCell
{
	int _num; //�� ī�尡 ��� ī���ΰ�?
	Status _status; //�� ī���� ���� ���´� �����ΰ�? (��������? �����ֳ�? �ӽ÷� �����ų�?)
};