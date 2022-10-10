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

        wsprintf(userMsg, _T("Q : ����, R : �ٽý���"));
        TextOut(hdc, 700, 40, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("N : ����, P : ����"));
        TextOut(hdc, 700, 60, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("Z : �ǵ�����, X : �ٽ��ϱ�"));
        TextOut(hdc, 700, 80, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("�������� : %d"), nStage + 1);
        TextOut(hdc, 700, 100, userMsg, lstrlen(userMsg));

        wsprintf(userMsg, _T("�̵�Ƚ�� : %d ȸ"), nMove);
        TextOut(hdc, 700, 120, userMsg, lstrlen(userMsg));
    }
    else
    {
        DrawBitmap(hdc, 0, 0, hBitEnding);
    }
}

BOOL Game::TestEnd()
{
    //���� ���߿� �����ϰ�
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == 'O') {  //�÷��̾��� ������ġ
                if (arStage[nStage][y][x] != '.')
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

void Game::Move(int dir) //Ű�� �������� ��������
{
    int dx = 0, dy = 0; // ��Ÿx, ��Ÿy
    BOOL bWithPack = FALSE; // �Ѱ� �Բ� �������°�?

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
        if (ns[ny + dy][nx + dx] == 'O') // ���ڸ� �о���
        {
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.')
            {
                // ���� �и��� �ڽ��� ��ġ�� ���� ���������ٸ� �������������� �������� ��ȯ
                ErasePack(nx + dx, ny + dy);
                ns[ny + dy * 2][nx + dx * 2] = 'O'; // �ڽ��� �о�
                bWithPack = TRUE;
            }
            else { // ���� ������ �̵��Ұ����ϴٸ� �̵���ü�� ����ϱ� ���� return
                return;
            }
        }

        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo mInfo(dx, dy, bWithPack);
        // �̰ɷ� ����
        undoList->push_back(mInfo);
        redoList->clear();

        InvalidateRect(hWndMain, NULL, FALSE);
    }
}

void Game::InitStage()
{
    memcpy(ns, arStage[nStage], sizeof(ns)); //�޸� ī��

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == '@') {  //�÷��̾��� ������ġ
                nx = x;
                ny = y;
                ns[y][x] = ' ';  //�ش���ġ�� �÷��̾ ������ ����ó��
            }
        }
    }

    // ���͸� ���� �� ���� �����̳ʸ� ��ٸ�
    // ���⼭ �ʱ�ȭ �ϴ� �۾��� �ݵ�� ������Ѵ�.
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

    hMemDC = CreateCompatibleDC(hdc); //�޸� DC����
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

    hMemDC = CreateCompatibleDC(hdc); //�޸� DC����
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
        InitStage(); // �������� �ʱ�ȭ
        return TRUE;
    }

    return FALSE;
}

BOOL Game::GotoPrevStage()
{
    if (nStage > 0)
    {
        nStage--;
        InitStage(); // �������� �ʱ�ȭ
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
        wsprintf(userMsg, _T("�������� Ŭ����"));
        MessageBox(hWndMain, userMsg, _T("�˸�"), MB_OK);
    }

    if (GotoNextStage()) {
    }
    else {
        isEnding = true;
        InvalidateRect(hWndMain, NULL, TRUE);
    }
}