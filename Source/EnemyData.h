#pragma once

#include "EnemySlime.h"
#include "EnemyCrab.h"
#include "EnemyScallops.h"

#include <Windows.h>
#include <sstream>
#include <iostream>
#include <string>

#define SetUni(pos)  {pos,EnemyType::uni,false}
#define SetCrabLR(pos,move){pos,EnemyType::crabLR,move}
#define SetCrabUD(pos,move){pos,EnemyType::crabUD,move}
#define SetScallops(pos){pos,EnemyType::scallops,false}
#define SetEnd(){DirectX::XMFLOAT3(-1,-1,-1),-1,false}

struct EnemyScript
{
    DirectX::XMFLOAT3 pos;
    int type;
    bool move;  //最初に左右上下どちらに動くか    //true:：左 下 false：右 上
};


enum EnemyType
{
    uni,
    crabLR,
    crabUD,
    scallops
};

#define ofP  0.0f

EnemyScript enemyData0[] =
{
    /*
    SetUni(DirectX::XMFLOAT3(1,0,1)),
    SetUni(DirectX::XMFLOAT3(1,0,2)),
    SetUni(DirectX::XMFLOAT3(1,0,4)),
    SetScallops(DirectX::XMFLOAT3(1,0,6)),
    SetCrabLR(DirectX::XMFLOAT3(1,0,5),true),
    SetCrabUD(DirectX::XMFLOAT3(1,0,7),true),
    SetUni(DirectX::XMFLOAT3(1,0,16)),
    */
    //uni

     SetUni(DirectX::XMFLOAT3(4.7f,0.0f,24.0f)),
     SetUni(DirectX::XMFLOAT3(1.0f,0.0f,37.0f)),
     SetUni(DirectX::XMFLOAT3(4.5f,0.0f,37.0f)),
     SetUni(DirectX::XMFLOAT3(8.0f,0.0f,37.0f)),
     SetUni(DirectX::XMFLOAT3(3.5f,0.0f,39.0f)),

     SetUni(DirectX::XMFLOAT3(6.5f,0.0f,39.0f)),
     SetUni(DirectX::XMFLOAT3(4.0f,0.0f,58.0f)),
     SetUni(DirectX::XMFLOAT3(3.0f,0.0f,74.0f)),

     //crab
     SetCrabLR(DirectX::XMFLOAT3(2.0f,0.0f,54.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(7.0f,0.0f,60.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(2.0f,0.0f,66.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(9.0f,0.0f,71.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(0.8f,0.0f,92.0f),true),

     SetCrabUD(DirectX::XMFLOAT3(4.1f,0.0f,95.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(7.4f,0.0f,98.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(1.6f,0.0f,105.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(4.6f,0.0f,108.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(7.5f,0.0f,111.0f),true),

     //Scallops
     SetScallops(DirectX::XMFLOAT3(4.6f,0.0f,80.0f)),

    SetEnd()
};

EnemyScript enemyData1[] =
{
    //uni
     SetUni(DirectX::XMFLOAT3(2.0f,0.0f,17.4f)),
     SetUni(DirectX::XMFLOAT3(2.4f,0.0f,51.0f)),
     SetUni(DirectX::XMFLOAT3(7.0f,0.0f,51.0f)),
     SetUni(DirectX::XMFLOAT3(1.5f,0.0f,65.0f)),
     SetUni(DirectX::XMFLOAT3(5.5f,0.0f,68.0f)),

     SetUni(DirectX::XMFLOAT3(8.5f,0.0f,65.0f)),
     SetUni(DirectX::XMFLOAT3(6.1f,0.0f,96.0f )),
     SetUni(DirectX::XMFLOAT3(3.0f,0.0f,99.0f )),
     SetUni(DirectX::XMFLOAT3(5.0f,0.0f,120.0f)),

     //crab
     SetCrabUD(DirectX::XMFLOAT3(4.8f,0.0f,6.0f),false),
     SetCrabUD(DirectX::XMFLOAT3(4.8f,0.0f,10.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(4.8f,0.0f,14.0f),false),
     SetCrabLR(DirectX::XMFLOAT3(0.0f,0.0f,44.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(2.3f,0.0f,35.0f),true),

     SetCrabUD(DirectX::XMFLOAT3(6.4f,0.0f,38.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(4.2f,0.0f,44.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(9.0f,0.0f,47.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(4.4f,0.0f,56.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(8.0f,0.0f,61.0f),true),

     SetCrabLR(DirectX::XMFLOAT3(2.5f,0.0f,61.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(2.0f,0.0f,80.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(7.7f,0.0f,80.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(4.3f,0.0f,86.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(2.2f,0.0f,90.0f),true),

     SetCrabLR(DirectX::XMFLOAT3(6.8f,0.0f,90.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(1.1f,0.0f,104.5f),true),
     SetCrabUD(DirectX::XMFLOAT3(7.8f,0.0f,104.5f),true),
     SetCrabUD(DirectX::XMFLOAT3(5.0f,0.0f,108.0f),true),
     SetCrabLR(DirectX::XMFLOAT3(2.1f,0.0f,113.0f),true),

     SetCrabLR(DirectX::XMFLOAT3(7.3f,0.0f,113.0f),true),
     SetCrabUD(DirectX::XMFLOAT3(4.7f,0.0f,116.0f),true),


     //Scallops
     SetScallops(DirectX::XMFLOAT3(9.0f,0.0f,24.0f)),

    SetEnd()
};

EnemyScript enemyData2[] =
{
    //uni
    SetUni(DirectX::XMFLOAT3(4.3f,0.0f,45.0f)),
    SetUni(DirectX::XMFLOAT3(5.6f,0.0f,47.0f)),
    SetUni(DirectX::XMFLOAT3(6.8f,0.0f,45.0f)),
    SetUni(DirectX::XMFLOAT3(4.5f,0.0f,64.0f)),
    SetUni(DirectX::XMFLOAT3(1.3f,0.0f,68.0f)),

    SetUni(DirectX::XMFLOAT3(7.3f,0.0f,68.0f )),
    SetUni(DirectX::XMFLOAT3(3.6f,0.0f,129.0f)),
    SetUni(DirectX::XMFLOAT3(7.0f,0.0f,129.0f)),
    SetUni(DirectX::XMFLOAT3(6.5f,0.0f,137.0f)),
    SetUni(DirectX::XMFLOAT3(8.0f,0.0f,158.0f)),
    
    //crab
    SetCrabUD(DirectX::XMFLOAT3(9.0f,0.0f,4.8f ),true),
    SetCrabLR(DirectX::XMFLOAT3(2.8f,0.0f,8.7f +2.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(5.2f,0.0f,11.0f+4.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(7.3f,0.0f,13.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(5.0f,0.0f,18.0f+4.0f+2.0f),true),

    SetCrabLR(DirectX::XMFLOAT3(6.4f,0.0f,22.0f+4.0f+2.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(0.0f,0.0f,21.0f+4.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(0.0f,0.0f,16.5f+4.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(8.5f,0.0f,26.0f+4.0f+2.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(2.7f,0.0f,28.0f+4.0f),true),

    SetCrabUD(DirectX::XMFLOAT3(0.0f,0.0f,46.0f+6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(9.0f,0.0f,46.0f+6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(2.0f,0.0f,46.0f+6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(7.0f,0.0f,46.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(4.0f,0.0f,52.0f+6.0f),true),

    SetCrabUD(DirectX::XMFLOAT3(2.0f,0.0f,56.0f +6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(7.0f,0.0f,56.0f +6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(0.0f,0.0f,60.0f +6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(9.0f,0.0f,60.0f +6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(0.0f,0.0f,100.0f+6.0f),true),

    SetCrabUD(DirectX::XMFLOAT3(4.8f,0.0f,100.0f+6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(9.0f,0.0f,100.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(2.0f,0.0f,108.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(5.3f,0.0f,108.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(8.6f,0.0f,108.0f+6.0f),true),

    SetCrabLR(DirectX::XMFLOAT3(0.0f,0.0f,117.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(9.0f,0.0f,117.0f+6.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(3.2f,0.0f,122.0f+6.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(5.0f,0.0f,142.0f+3.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(1.5f,0.0f,130.0f+9.0f),true),
    SetCrabUD(DirectX::XMFLOAT3(7.6f,0.0f,133.0f+9.0f),true),

    SetCrabLR(DirectX::XMFLOAT3(4.7f,0.0f,137.0f+9.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(2.2f,0.0f,143.0f+9.0f),true),
    SetCrabLR(DirectX::XMFLOAT3(7.7f,0.0f,143.0f+9.0f),true),

    //Scallops
    SetScallops(DirectX::XMFLOAT3(5.4f,0.0f,58.0f)),
    SetScallops(DirectX::XMFLOAT3(4.5f,0.0f,65.0f+6.0f)),
    SetScallops(DirectX::XMFLOAT3(2.1f,0.0f,73.0f+6.0f)),
    SetScallops(DirectX::XMFLOAT3(9.0f,0.0f,80.0f+6.0f)),
    SetScallops(DirectX::XMFLOAT3(0.0f,0.0f,87.0f+6.0f)),

    SetScallops(DirectX::XMFLOAT3(5.2f,0.0f,96.0f+6.0f)),

    SetEnd()
};



EnemyScript* enemyDataAll[3] =
{
    enemyData0,
    enemyData1,
    enemyData2,
};
