#pragma once
#include "framework.h"
#include "resource.h"
#include <memory>
#include <vector>

using namespace std;

enum class Status
{
	HIDDEN = 1, //카드가 뒤집혀있을때
	FLIP = 2, //카드가 열려있을때
	TEMPFLIP = 3, //카드가 임시로 열려있을때
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
	int _num; //이 카드가 몇번 카드인가?
	Status _status; //이 카드의 현재 상태는 무엇인가? (뒤집혔냐? 열려있냐? 임시로 열린거냐?)
};