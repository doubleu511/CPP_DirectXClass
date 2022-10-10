#pragma once
#include "CouplePair.h"

#define CS 64
#define random(n) (rand() % n)

class Game
{
public:
	Game(HINSTANCE hInst, HWND hWnd);
	void Update(ULONGLONG tick);
	void HandleInput(shared_ptr<POINT> mouseCoord);
	void InitGame();
	void DrawScreen(HDC hdc);
	shared_ptr<POINT> GetTempFlip(); //���� ���� ī�带 ��ȯ
	int GetRemainPair(); //���� ������ ������ ��ȯ
	void DrawBitmap(HDC hdc, int x, int y, int idx);
	~Game();
	int _time = 0;
private:
	HINSTANCE _hInst;
	HWND _hWnd;
	CardCell _cardCells[4][4];
	int _count; //�õ� Ƚ��
	HBITMAP _hBitSheet; //ī���Ʈ
	GameStatus _gStatus; //���� ���� ����
	float _viewTimer; //�ӽ÷� �����ִ� �ð� Ÿ�̸�
	float _hintTimer; //��Ʈ �����ִ� �ð� Ÿ�̸�
};