#pragma once

enum EnemyType
{
    TYPE_WIZARD,
    TYPE_GOBLIN,
    TYPE_SKELETON,
    TYPE_MUSHROOM,
    TYPE_EYE
};

enum EnemyState
{
    STATE_IDLE,
    STATE_FLYING,
    STATE_ATTACK,
    STATE_TAKE_HIT,
    STATE_DYING,
    STATE_DEAD
};

struct Enemy
{
    float x;
    float y;
    EnemyType type;
    EnemyState state;
    int currentFrame;
    float timeInFrame;
    float aiTimer;
    bool isWandering;
    float wanderAngle;
    int health;
};