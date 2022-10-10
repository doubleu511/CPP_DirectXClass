#pragma once
#include "framework.h"
#include "resource.h"
#include "MoveInfo.h"

#define MAX_STAGE 3
#define BW 32
#define BH 32

#include <vector>
using namespace std;

class Game
{
public:
    char arStage[MAX_STAGE][18][21] = {
    {
    "####################",
    "####################",
    "####################",
    "#####   ############",
    "#####O  ############",
    "#####  O############",
    "###  O O ###########",
    "### # ## ###########",
    "#   # ## #####  ..##",
    "# O  O   @      ..##",
    "##### ### # ##  ..##",
    "#####     ##########",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "####..  #     ######",
    "####..  # O  O  ####",
    "####..  #O####  ####",
    "####..    @ ##  ####",
    "####..  # #  O #####",
    "######### ##O O ####",
    "###### O  O O O ####",
    "######    #     ####",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "##########     @####",
    "########## O#O #####",
    "########## O  O#####",
    "###########O O #####",
    "########## O # #####",
    "##....  ## O  O  ###",
    "###...    O  O   ###",
    "##....  ############",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    }
    };

    int playerSpriteIdx = 3;

    vector<MoveInfo>* undoList;
    vector<MoveInfo>* redoList;

    HWND hWndMain;
    Game(HWND main, HINSTANCE hInst);
    ~Game();
    
    void DrawScreen(HDC hdc, PAINTSTRUCT& ps);
    void DrawEnding(HDC hdc, PAINTSTRUCT& ps);
    BOOL TestEnd(); //������ Ŭ���� �Ǿ����� Ȯ���ϴ°�
    void Move(int dir);
    void InitStage(); //�������� �ʱ�ȭ�Լ�
    void DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet);
    void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBitSheet);

    BOOL GotoNextStage();
    BOOL GotoPrevStage();
    void ErasePack(int x, int y); // ���ڰ� �ִ� �ڸ��� ���� ���������ִ� �Լ�
    void TestClear();

    BOOL Undo(); // ������
    BOOL Redo();

    void PlayerSpriteChange(int dx, int dy);

    int GetCurrentStage(); // ���� �������� ���� �˾ƿ���

    BOOL isEnding = false;

private:
    char ns[18][21] = { 0, }; // ���� �÷��̾����� ��������
    int nStage = 0;
    int nx = 0, ny = 0;
    int nMove = 0;
    HBITMAP hBitSheet = nullptr; // ��Ʈ��
    HBITMAP hBitEnding = nullptr; // ��Ʈ��


};