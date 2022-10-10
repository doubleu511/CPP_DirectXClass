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
    BOOL TestEnd(); //게임이 클리어 되었는지 확인하는거
    void Move(int dir);
    void InitStage(); //스테이지 초기화함수
    void DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet);
    void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBitSheet);

    BOOL GotoNextStage();
    BOOL GotoPrevStage();
    void ErasePack(int x, int y); // 상자가 있던 자리에 맵을 복구시켜주는 함수
    void TestClear();

    BOOL Undo(); // 수행평가
    BOOL Redo();

    void PlayerSpriteChange(int dx, int dy);

    int GetCurrentStage(); // 현재 스테이지 정보 알아오기

    BOOL isEnding = false;

private:
    char ns[18][21] = { 0, }; // 현재 플레이어중인 스테이지
    int nStage = 0;
    int nx = 0, ny = 0;
    int nMove = 0;
    HBITMAP hBitSheet = nullptr; // 비트맵
    HBITMAP hBitEnding = nullptr; // 비트맵


};