#pragma once

enum class ObjectType {
    UNDEFINED,
    WALL,
    FIREBALL,
    ENEMY,
    PLAYER,
};


struct FireballEntity
{
    int x;

};

struct WallEntity
{
    int x;

};

struct EnemyEntity
{
    int x;

};

struct PlayerEntity
{
    int x;
};

struct DeleteMe
{
    bool canDelete;
};