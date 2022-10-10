#include "Game.h"

Game::Game(HINSTANCE hInst, HWND hWnd)
{
	RECT crt;
	SetRect(&crt, 0, 0, CS * 4 + 250, CS * 4); //���� ���� ũ�� ���ؼ� ������ũ�� ����

	AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
	SetWindowPos(hWnd, nullptr, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);

	_hWnd = hWnd;
	_hInst = hInst;
	_hBitSheet = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_BITSHEET2));
	srand(GetTickCount64());
	_viewTimer = 0;
	_hintTimer = 0;
	_gStatus = GameStatus::PAUSED;
}

Game::~Game()
{
	DeleteObject(_hBitSheet);
}

void Game::InitGame()
{
	_count = 0;
	memset(_cardCells, 0, sizeof(_cardCells));
	int x = 0;
	int y = 0;

	vector<int> cardPools = { 1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8 };

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int index = random(cardPools.size());
			int cardNum = cardPools[index];

			cardPools.erase(cardPools.begin() + index);

			_cardCells[y][x]._num = cardNum;
			_cardCells[y][x]._status = Status::HIDDEN;
		}
	}

	_gStatus = GameStatus::HINT;
	InvalidateRect(_hWnd, nullptr, TRUE);
	_hintTimer = 2000; //2�ʵ��� ����(���ӽ����ϰ�)
}

void Game::Update(ULONGLONG tick)
{
	if (_gStatus == GameStatus::PAUSED)
		return;

	if (_gStatus == GameStatus::VIEW)
	{
		_viewTimer -= tick;
		if (_viewTimer <= 0)
		{
			_viewTimer = 0;
			_gStatus = GameStatus::RUN;

			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					if (_cardCells[y][x]._status == Status::TEMPFLIP)
						_cardCells[y][x]._status = Status::HIDDEN;
				}
			}
			InvalidateRect(_hWnd, nullptr, FALSE);
		}
	}
	else if (_gStatus == GameStatus::HINT)
	{
		_hintTimer -= tick;
		if (_hintTimer <= 0)
		{
			_gStatus = GameStatus::RUN;
			InvalidateRect(_hWnd, nullptr, FALSE);
		}
	}
}

void Game::HandleInput(shared_ptr<POINT> mouseCoord)
{
	int nx = mouseCoord->x;
	int ny = mouseCoord->y;

	if (_gStatus != GameStatus::RUN || nx > 3 || ny > 3 || _cardCells[ny][nx]._status != Status::HIDDEN)
	{
		return;
	}

	shared_ptr<POINT> p = GetTempFlip(); //���� �ӽ÷� ���� �ִ� ī�尡 �ִ��� 
	if (p == nullptr) //�ƹ��͵� �ȿ��� �ִ� ����
	{
		_cardCells[ny][nx]._status = Status::TEMPFLIP;
		InvalidateRect(_hWnd, nullptr, FALSE);
	}
	else { //�ι�°�� �� ����
		_count++; //�ѹ� �õ������� �õ�Ƚ�� ����
		if (_cardCells[p->y][p->x]._num == _cardCells[ny][nx]._num) //���� ī�忴�ٸ�
		{
			MessageBeep(0); //��
			_cardCells[p->y][p->x]._status = Status::FLIP;
			_cardCells[ny][nx]._status = Status::FLIP;
			InvalidateRect(_hWnd, nullptr, FALSE);
			if (GetRemainPair() == 0)
			{
				TCHAR msg[128];
				wsprintf(msg, _T("�����մϴ�. Ŭ���� �ϼ̽��ϴ�.\n�帥 �ð�: %d��"), _time);
				MessageBox(_hWnd, msg, _T("�˸�"), MB_OK);

				_gStatus = GameStatus::PAUSED;
			}
		}
		else { //�ٸ� ī�带 �������ٸ�
			_cardCells[ny][nx]._status = Status::TEMPFLIP;
			InvalidateRect(_hWnd, nullptr, FALSE);
			_gStatus = GameStatus::VIEW;
			_viewTimer = 1000;
		}
	}

}


shared_ptr<POINT> Game::GetTempFlip()
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (_cardCells[y][x]._status == Status::TEMPFLIP)
			{
				shared_ptr<POINT> p = make_shared<POINT>();
				p->x = x;
				p->y = y;
				return p;
			}
		}
	}
	return nullptr;
}

int Game::GetRemainPair()
{
	int remain = 16;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (_cardCells[y][x]._status == Status::FLIP)
			{
				remain--;
			}
		}
	}
	return remain / 2;
}

void Game::DrawScreen(HDC hdc)
{
	int imgIdx;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (_gStatus == GameStatus::HINT || _cardCells[y][x]._status != Status::HIDDEN)
			{
				imgIdx = _cardCells[y][x]._num - 1;
			}
			else {
				imgIdx = 8;
			}
			DrawBitmap(hdc, x * CS, y * CS, imgIdx);
		}
	}

	TCHAR msg[128];
	lstrcpy(msg, _T("¦ ���߱� ���� Ver 0.01"));
	TextOut(hdc, 300, 10, msg, lstrlen(msg));

	wsprintf(msg, _T("�� �õ� Ƚ�� : %d"), _count);
	TextOut(hdc, 300, 50, msg, lstrlen(msg));

	wsprintf(msg, _T("���� �� ã�� �� : %d"), GetRemainPair());
	TextOut(hdc, 300, 70, msg, lstrlen(msg));

	wsprintf(msg, _T("�帥�ð� : %d ��"), _time);
	TextOut(hdc, 300, 100, msg, lstrlen(msg));
}

void Game::DrawBitmap(HDC hdc, int x, int y, int idx)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc); //�־��� HDC�� ȣȯ�Ǵ� MemDC�� ����� 
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, _hBitSheet);

	StretchBlt(hdc, x, y, CS, CS, hMemDC, idx * CS, 0, CS, CS, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}