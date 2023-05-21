#pragma once

#include <DirectXMath.h>
#include "Stage.h"

//TODO::ñYÇÍÇ∏Ç…
#include "Collision.h"


#define SetStage(pos){pos,StageObjType::stage,0}
#define SetSignboard(pos,num){pos,StageObjType::signboard,num}
#define SetGoal(pos){pos,StageObjType::flagGoal,0}
#define SetSpeedUp(pos){pos,StageObjType::flagSpeedUp,0}
#define SetEnemyWall(pos){pos,StageObjType::enemyWall,0}
#define SetHalfEnemyWall(pos){{pos.x,-4.0f,pos.z},StageObjType::enemyWall,0}
#define SetEnd(){DirectX::XMFLOAT3(-1,-1,-1),-1,0}

struct StageScript
{
    DirectX::XMFLOAT3 pos;
    int type;
    int num;
};

enum StageObjType
{
    stage,
    signboard,
    flagGoal,
    flagSpeedUp,
    enemyWall,
};

#define offPs  15.0f

static StageScript stageData0[] =
{
    //{DirectX::XMFLOAT3(0,0,0),0},
    {DirectX::XMFLOAT3(4,0,7.0f*-1*2) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*0*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*1*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*2*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*3*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*4*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*5*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*6*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*7*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*8*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*9*2 ) ,0},
    {DirectX::XMFLOAT3(4,0,7.0f*10*2 ),0},
    //Signboard
    SetSignboard(DirectX::XMFLOAT3(-0.5,6,8 ) ,0),
    SetSignboard(DirectX::XMFLOAT3(-0.5,6,24 ),1),
    SetSignboard(DirectX::XMFLOAT3(-0.5,6,37 ),2),
    SetSignboard(DirectX::XMFLOAT3(-0.5,6,77 ),3),
    SetSignboard(DirectX::XMFLOAT3(-0.5,6,120),4),  //èoå˚

    //enemyWall
    SetEnemyWall(DirectX::XMFLOAT3(6,0,45)),
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,82)),

#if SPEEDUP

    //speedup
    SetSpeedUp(DirectX::XMFLOAT3(0,0,79)),

#endif
    //goal
    SetGoal(DirectX::XMFLOAT3(6.5,0,130)),

    SetEnd()
};

static StageScript stageData1[] =
{
    {DirectX::XMFLOAT3(4,0,7.0f * -1 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 0 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 1 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 2 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 3 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 4 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 5 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 6 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 7 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 8 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 9 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 10 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 11 * 2),0},

    SetSignboard(DirectX::XMFLOAT3(-0.5,6,120),4),  //èoå˚

    //enemyWall
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,26)),
    SetEnemyWall(DirectX::XMFLOAT3(6,0,73)),

#if SPEEDUP
    //speedup
    SetSpeedUp(DirectX::XMFLOAT3(0,0,32)),
    SetSpeedUp(DirectX::XMFLOAT3(0,0,94)),

#endif
    //goal
    SetGoal(DirectX::XMFLOAT3(6.5,0,130)),

    SetEnd()
};

static StageScript stageData2[] =
{
    {DirectX::XMFLOAT3(4,0,7.0f * -1 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 0 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 1 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 2 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 3 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 4 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 5 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 6 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 7 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 8 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 9 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 10 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 11 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 12 * 2),0},
    {DirectX::XMFLOAT3(4,0,7.0f * 13 * 2),0},

    SetSignboard(DirectX::XMFLOAT3(-0.5,6,161),4),  //èoå˚

    //enemyWall
    SetEnemyWall(DirectX::XMFLOAT3(6,0,51)),
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,68+offPs)),
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,75+offPs)),
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,82+offPs)),
    SetHalfEnemyWall(DirectX::XMFLOAT3(6,0,89+offPs)),
    SetEnemyWall(DirectX::XMFLOAT3(6,0,124 + offPs)),

#if SPEEDUP
    //speedup
    SetSpeedUp(DirectX::XMFLOAT3(0,0,51 + offPs)),
#endif
    //goal
    SetGoal(DirectX::XMFLOAT3(6.5,0,171)),

    SetEnd()
};



static StageScript* stageDataAll[3] =
{
    stageData0,
    stageData1,
    stageData2,
};

static float stageGoalPos[3] =
{
    130,
    130,
    171,
};

