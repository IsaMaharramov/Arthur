#pragma once

enum EnemyState
{
    STATE_IDLE,
    STATE_FLYING,
    STATE_ATTACK,
    STATE_DYING,
    STATE_DEAD
};

struct Enemy
{
    float x;
    float y;
    EnemyState state;
    int currentFrame;
    float timeInFrame;

    float aiTimer;
};