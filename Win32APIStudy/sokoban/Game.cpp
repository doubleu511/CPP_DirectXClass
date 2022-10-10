#include "Game.h"

Game::Game(HWND main, HINSTANCE hInst)
{
	hWndMain = main;
	hBitSheet = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHEET));
	hBitEnding = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDING));
	nStage = 0;
	undoList = new vector<MoveInfo>();
	redoList = new vector<MoveInfo>();
}

Game::~Game()
{
	DeleteObject(hBitSheet);
	delete undoList;
}

void Game::DrawScreen(HDC hdc, PAINTSTRUCT& ps)
{
    int iBit = 3;
    TCHAR userMsg[256];
    //EMPTY, MAN, PACK, TARGET, WALL

    if (!isEnding)
    {
        for (int y = 0; y < 18; y++)
        {
            for (int x = 0; x < 20; x++) {
                switch (ns[y][x])
                {
                case '#':
                    iBit = 0;
                    break;
                case '.':
                    iBit = 2;
                    break;
                case 'O':
                    iBit = 1;
                    break;
                case ' ':
                    iBit = 7;
                    break;
                }

                DrawBitmap(hdc, iBit, x * BW, y * BH, hBitSheet);
            }
        }
        DrawBitmap(hdc, playerSpriteIdx, nx * BW, ny * BH, hBitSheet);

        wsprintf(userMsg, _T("Sokoban!"));
        TextOut(hdc, 700, 20, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("Q : 종료, R : 다시시작"));
        TextOut(hdc, 700, 40, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("N : 다음, P : 이전"));
        TextOut(hdc, 700, 60, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("Z : 되돌리기, X : 다시하기"));
        TextOut(hdc, 700, 80, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("스테이지 : %d"), nStage + 1);
        TextOut(hdc, 700, 100, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("이동횟수 : %d 회"), nMove);
        TextOut(hdc, 700, 120, userMsg, lstrlen(userMsg));
    }
    else
    {
        DrawBitmap(hdc, 0, 0, hBitEnding);
    }
}

BOOL Game::TestEnd()
{
    //조건 나중에 생각하고
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == 'O') {  //플레이어의 시작위치
                if (arStage[nStage][y][x] != '.')
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

void Game::Move(int dir) //키가 눌린것을 받을꺼다
{
    int dx = 0, dy = 0; // 델타x, 델타y
    BOOL bWithPack = FALSE; // 팩과 함께 움직였는가?

    switch (dir)
    {
    case VK_LEFT:
        dx = -1;
        playerSpriteIdx = 5;
        break;
    case VK_RIGHT:
        dx = 1;
        playerSpriteIdx = 6;
        break;
    case VK_UP:
        dy = -1;
        playerSpriteIdx = 3;
        break;
    case VK_DOWN:
        dy = 1;
        playerSpriteIdx = 4;
        break;
    default:
        break;
    }

    if (ns[ny + dy][nx + dx] != '#')
    {
        if (ns[ny + dy][nx + dx] == 'O') // 상자를 밀었다
        {
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.')
            {
                // 지금 밀리는 박스의 위치가 원래 목적지였다면 스테이지정보도 목적지로 변환
                ErasePack(nx + dx, ny + dy);
                ns[ny + dy * 2][nx + dx * 2] = 'O'; // 박스를 밀어
                bWithPack = TRUE;
            }
            else { // 상자 뒤쪽이 이동불가능하다면 이동자체를 취소하기 위해 return
                return;
            }
        }

        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo mInfo(dx, dy, bWithPack);
        // 이걸로 수행
        undoList->push_back(mInfo);
        redoList->clear();

        InvalidateRect(hWndMain, NULL, FALSE);
    }
}

void Game::InitStage()
{
    memcpy(ns, arStage[nStage], sizeof(ns)); //메모리 카피

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == '@') {  //플레이어의 시작위치
                nx = x;
                ny = y;
                ns[y][x] = ' ';  //해당위치에 플레이어가 있으니 공백처리
            }
        }
    }

    // 벡터를 쓰든 뭘 쓰든 컨테이너를 썼다면
    // 여기서 초기화 하는 작업을 반드시 해줘야한다.
    undoList->clear();
    redoList->clear();

    nMove = 0;
    InvalidateRect(hWndMain, NULL, TRUE);
}

void Game::DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet)
{
    HDC hMemDC;
    HBITMAP hOldBitMap;
    int bx, by;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc); //메모리 DC생성
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hBitSheet));

    GetObject(hBitSheet, sizeof(BITMAP), &bit);

    bx = bit.bmWidth;
    by = bit.bmHeight;

    StretchBlt(hdc, x, y, 32, 32, hMemDC, idx * 32, 0, 32, 32, SRCCOPY);

    //BitBlt(hdc, x, y, bx, by, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

void Game::DrawBitmap(HDC hdc, int x, int y, HBITMAP hBitmap)
{
    HDC hMemDC;
    HBITMAP hOldBitMap;
    int bx, by;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc); //메모리 DC생성
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hBitmap));

    GetObject(hBitmap, sizeof(BITMAP), &bit);

    bx = bit.bmWidth;
    by = bit.bmHeight;

    BitBlt(hdc, x, y, bx, by, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

BOOL Game::GotoNextStage()
{
    if (nStage < MAX_STAGE - 1)
    {
        nStage++;
        InitStage(); // 스테이지 초기화
        return TRUE;
    }

    return FALSE;
}

BOOL Game::GotoPrevStage()
{
    if (nStage > 0)
    {
        nStage--;
        InitStage(); // 스테이지 초기화
        return TRUE;
    }   

    return FALSE;
}

void Game::ErasePack(int x, int y)
{
    ns[y][x] = arStage[nStage][y][x] == '.' ? '.' : ' ';
}

int Game::GetCurrentStage()
{
    return nStage;
}

BOOL Game::Undo()
{
    if (undoList->size() > 0)
    {
        MoveInfo info = undoList->back();
        undoList->pop_back();

        if (info.bWithPack)
        {
            ErasePack(nx + info.dx, ny + info.dy);
            ns[ny][nx] = 'O';
        }

        nx = nx - info.dx;
        ny = ny - info.dy;

        PlayerSpriteChange(info.dx, info.dy);

        MoveInfo mInfo(info.dx, info.dy, info.bWithPack);
        redoList->push_back(mInfo);

        InvalidateRect(hWndMain, NULL, FALSE);

        return TRUE;
    }

    return FALSE;
}

BOOL Game::Redo()
{
    if (redoList->size() > 0)
    {
        MoveInfo info = redoList->back();
        redoList->pop_back();

        if (info.bWithPack)
        {
            ErasePack(nx + info.dx, ny + info.dy);
            ns[ny + info.dy * 2][nx + info.dx * 2] = 'O';
        }

        nx = nx + info.dx;
        ny = ny + info.dy;

        PlayerSpriteChange(info.dx, info.dy);

        MoveInfo mInfo(info.dx, info.dy, info.bWithPack);
        undoList->push_back(mInfo);

        InvalidateRect(hWndMain, NULL, FALSE);
        return TRUE;
    }

    return FALSE;
}

void Game::PlayerSpriteChange(int dx, int dy)
{
    if (dx > 0)
    {
        playerSpriteIdx = 6;
    }
    else if (dx < 0)
    {
        playerSpriteIdx = 5;
    }
    else if (dy > 0)
    {
        playerSpriteIdx = 4;
    }
    else if (dy < 0)
    {
        playerSpriteIdx = 3;
    }
}

void Game::TestClear()
{
    TCHAR userMsg[256];

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == 'O')
            {
                ErasePack(x, y);
            }

            if (arStage[nStage][y][x] == '.')
            {
                ns[y][x] = 'O';
            }
        }
    }

    InvalidateRect(hWndMain, NULL, FALSE);

    if (nStage <= MAX_STAGE - 1)
    {
        wsprintf(userMsg, _T("스테이지 클리어"));
        MessageBox(hWndMain, userMsg, _T("알림"), MB_OK);
    }

    if (GotoNextStage()) {
    }
    else {
        isEnding = true;
        InvalidateRect(hWndMain, NULL, TRUE);
    }
}