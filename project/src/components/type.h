#pragma once

enum class ObjectType {
    UNDEFINED,
    RECTANGLE,
    TRIANGLE
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