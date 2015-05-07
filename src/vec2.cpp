#include "../headers/vec2.h"

Vec2 operator + (Vec2 u, Vec2 v)
{
    Vec2 result;
    result.x = u.x + v.x;
    result.y = u.y + v.y;

    return result;
}
Vec2 operator += (Vec2 &u, Vec2 v)
{
    u.x += v.x;
    u.y += v.y;

    return u;
}
Vec2 operator * (Vec2 u, int s)
{
    Vec2 result;
    result.x = u.x * s;
    result.y = u.y * s;

    return result;
}
Vec2 operator *= (Vec2 &u, int s)
{
    u.x *= s;
    u.y *= s;

    return u;
}