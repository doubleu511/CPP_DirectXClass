#include "Game.h"

void Game::Init(HINSTANCE hInst, HWND hWnd)
{
	_player = make_unique<Player>(BW / 2, 3, shared_from_this() ); // �� �κ��� �����ؾ���

	_status = GameStatus::GAMEOVER;
	_hInst = hInst;
	_hWndMain = hWnd;

	RECT clientRect;
	SetRect(&clientRect, 0, 0, (BW + 12) * TS, (BH + 2) * TS);
	AdjustWindowRect(&clientRect, WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX, TRUE);
	SetWindowPos(hWnd, NULL, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SWP_NOMOVE | SWP_NOZORDER);

	srand(static_cast<unsigned int>(GetTickCount64()));
	_blockSheet = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOCK_SHEET)); // �� ��Ʈ �ε�
}

void Game::Update(ULONGLONG tick)
{
    if (_status == GameStatus::RUNNING)
        _currentTime += static_cast<int>(tick);

	if (_currentTime > _blockTime) {
		_currentTime = 0;
		if (_player->MoveDown() == TRUE) {
			MakeNewBrick();
		}
	}
}

void Game::Render(HDC hdc)
{
    // ȭ�� �ܰ��� �׸�
    for (int x = 0; x < BW + 1; x++)
    {
        PrintTile(hdc, x, 0, Type::WALL);
        PrintTile(hdc, x, BH + 1, Type::WALL);
    }

    for (int y = 0; y < BH + 2; y++)
    {
        PrintTile(hdc, 0, y, Type::WALL);
        PrintTile(hdc, BW + 1, y, Type::WALL);
    }

    // ���带 �׸�
    for (int y = 1; y < BH + 1; y++)
    {
        for (int x = 1; x < BW + 1; x++)
        {
            if (CheckPlayerRender(x, y))
            {
                PrintTile(hdc, x, y, _player->GetBrickType());
            }
            else
            {
                PrintTile(hdc, x, y, _board[y][x]);
            }
        }
    }

    // ������� �׸���
    for (int x = BW + 3; x <= BW + 11; x++)
    {
        for (int y = BH - 5; y <= BH + 1; y++)
        {
            if (x == BW + 3 || x == BW + 11 || y == BH - 5 || y == BH + 1)
            {
                PrintTile(hdc, x, y, Type::WALL);
            }
            else
            {
                PrintTile(hdc, x, y, Type::EMPTY);
            }
        }
    }

    if (_status != GameStatus::GAMEOVER)
    {
        for (int i = 0; i < 4; i++)
        {
            PrintTile(hdc, BW + 7 + Shape[_nextBrick][0][i].x, BH - 2 + Shape[_nextBrick][0][i].y, static_cast<Type>(_nextBrick + 1));
        }
    }

    TCHAR msgStr[256];
    lstrcpy(msgStr, _T("GGM Tetris1"));
    TextOut(hdc, (BW + 4) * TS, 30, msgStr, lstrlen(msgStr));

    wsprintf(msgStr, _T("���� ��"));
    TextOut(hdc, (BW + 6) * TS, (BH - 3.5) * TS, msgStr, lstrlen(msgStr));

    wsprintf(msgStr, _T("���� : %d"), _score);
    TextOut(hdc, (BW + 4) * TS, 60, msgStr, lstrlen(msgStr));

    wsprintf(msgStr, _T("���� : %d ��"), _brickCount);
    TextOut(hdc, (BW + 4) * TS, 90, msgStr, lstrlen(msgStr));
}

void Game::PrintTile(HDC hdc, int x, int y, Type t)
{
    DrawBitmap(hdc, x * TS, y * TS, static_cast<int>(t));
}

void Game::DrawBitmap(HDC hdc, int x, int y, int idx)
{
    int sx = idx % 4;
    int sy = idx / 4;

    HDC hMemDC;
    HBITMAP oldBitmap;
    hMemDC = CreateCompatibleDC(hdc);
    oldBitmap = static_cast<HBITMAP>(SelectObject(hMemDC, _blockSheet));

    StretchBlt(hdc, x, y, TS, TS, hMemDC, sx * TS, sy * TS, TS, TS, SRCCOPY);
    SelectObject(hMemDC, oldBitmap);
    DeleteDC(hMemDC);
}

void Game::GetInput(WPARAM keyCode)
{
    if (_status == GameStatus::RUNNING)
        _player->HandleInput(keyCode);
}

void Game::StartGame()
{
    
    if (_status != GameStatus::GAMEOVER) return;

    for (int y = 0; y < BH + 2; y++)
    {
        for (int x = 0; x < BW + 2; x++)
        {
            _board[y][x] = (x == 0 || x == BW + 1 || y == 0 || y == BH + 1) ? Type::WALL : Type::EMPTY;
        }
    }

    _score = 0;
    _brickCount = 0;
    _status = GameStatus::RUNNING;
    MakeNewBag();

    _nextBrick = brickBag.front();
    brickBag.pop();
    bagIndex++;


    MakeNewBrick(); // nextBrick => nowBrick���� �����ϰ� nextBrick�ϳ� ���� �̰�
    _blockTime = 1000; // 1�ʿ� �ѹ����� ���������� ���ش�.
}

void Game::PauseGame()
{
    if (_status == GameStatus::RUNNING)
    {
        _status = GameStatus::PAUSE;
    }
    else if (_status == GameStatus::PAUSE)
    {
        _status == GameStatus::RUNNING;
    }
}

void Game::MakeNewBag()
{
    vector<int> numberBox = { 0,1,2,3,4,5,6 };

    for (int i = 0; i < 7; i++)
    {
        int randomIndex = rand() % numberBox.size();

        int randomBrick = numberBox[randomIndex];
        numberBox.erase(numberBox.begin() + randomIndex);

        brickBag.push(randomBrick);
    }
}

void Game::MakeNewBrick()
{
    _brickCount++;
    _player->SetBrickNum(_nextBrick);
    _player->SetPos(BW / 2, 3);
    _player->SetRot(0);

    if (bagIndex >= 6)
    {
        MakeNewBag();
    }

    _nextBrick = brickBag.front();
    brickBag.pop();
    bagIndex++;


    InvalidateRect(_hWndMain, NULL, FALSE);
    Type t = GetAround(_player->GetPoint(), _player->GetBrickNum(), _player->GetRot());
    if (t != Type::EMPTY)
    {
        _status = GameStatus::GAMEOVER;
        MessageBox(_hWndMain, _T("Game over!, �ٽ� �Ϸ��� ���ӽ����� ��������"), _T("�˸�"), MB_OK);
    }
}

void Game::UpdateBoard()
{
    RECT rt;
    // ����ȭ�� �ٽñ׸���
    SetRect(&rt, TS, TS, (BW + 1) * TS, (BH + 1) * TS);
    InvalidateRect(_hWndMain, &rt, FALSE);

    // ������ ��� �ٽñ׸���
    SetRect(&rt, (BW + 4) * TS, (BH - 6) * TS, (BW + 16) * TS, (BH)*TS);
    InvalidateRect(_hWndMain, &rt, FALSE);
}

Type Game::GetAround(POINT p, int brick, int rot)
{
    for (int i = 0; i < 4; i++)
    {
        Type t = _board[p.y + Shape[brick][rot][i].y][p.x + Shape[brick][rot][i].x];
        if (t != Type::EMPTY)
            return t;
    }

    return Type::EMPTY;
}

void Game::TestFull()
{
    int count = 0;
    // ���� ������ �ִ� ���� board���ٰ� ������ְ�
    POINT p = _player->GetPoint();
    int nowBrick = _player->GetBrickNum();
    int rot = _player->GetRot();
    Type t = _player->GetBrickType();

    for (int i = 0; i < 4; i++)
    {
        _board[p.y + Shape[nowBrick][rot][i].y][p.x + Shape[nowBrick][rot][i].x] = t;
    }
    nowBrick = -1;


    // �� �ڿ� ���带 �˻��ؼ� ���� ���� ��������� ��.
    for (int y = BH; y >= 1; y--)
    {
        bool isFull = true;

        for (int x = BW; x >= 1; x--)
        {
            if (_board[y][x] == Type::EMPTY)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            for (int i = y; i > 1; i--)
            {
                for (int j = 1; j < BW + 1; j++)
                {
                    _board[i][j] = _board[i - 1][j];
                }
            }
            for (int j = 1; j < BW + 1; j++) {
                _board[1][j] = Type::EMPTY; // �� ������ ����������.
            }
            UpdateBoard();
            y++;
            count++;
        }
    }

    int combo;

    switch (count)
    {
    case 1:
        combo = 1;
        break;
    case 2:
        combo = 4;
        break;
    case 3:
        combo = 8;
        break;
    case 4:
        combo = 16;
    default:
        combo = 0;
        break;
    }

    // �� �ۼ�������
    _score += combo;
    if (_brickCount % 10 == 0 && _blockTime > 200)
    {
        _blockTime -= 50;
        //SetTimer(_hWndMain, 1, blockInterval, NULL);
    }
}

BOOL Game::CheckPlayerRender(int x, int y)
{
    if (_status != GameStatus::GAMEOVER && _player->GetBrickNum() != -1)
    {
        for (int idx = 0; idx < 4; idx++)
        {
            POINT p = _player->GetPoint();
            int rot = _player->GetRot();
            int nowBrick = _player->GetBrickNum();

            if (x == p.x + Shape[nowBrick][rot][idx].x && y == p.y + Shape[nowBrick][rot][idx].y)
            {
                return TRUE;
            }
        }
    }

	return FALSE;
}
