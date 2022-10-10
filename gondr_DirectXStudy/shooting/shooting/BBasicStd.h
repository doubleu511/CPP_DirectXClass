#ifndef BBASIC_STD
#define BBASIC_STD

#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <string>
#include <ctime>
#include <map>    //씬 관리하는데 사용
#include <list> //총알, 적 등 매니징하는데 사용
#include <stack>
#include <vector>
#include <queue> //애니메이션 큐 하는데 사용
#include <memory>

#include "Vector2.h"

#define BASIC_WINDOW_NAME _T("FRAMEWORK ENGINE")
#define PI 3.141592654

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

//명시적으로 winmm.lib 파일을 추가한다.
#pragma comment(lib, "winmm.lib")

using namespace std;

typedef struct tagFPOINT
{
    float x;
    float y;
} FPOINT;

typedef struct tagOBJECTSIZE
{
    int width;
    int height;
} OBJECTSIZE;

typedef struct SeqPoint
{
    float time;
    float angle;
} SeqPoint;

enum class ENEMY_TYPE
{
    ENEMY_01 = 0,
    ENEMY_02 = 1,
    ENEMY_03 = 2,
    ENEMY_04 = 3,
    ENEMY_BOSS = 4
};

#endif // !BBASIC_STD