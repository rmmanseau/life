#ifndef VEC2_H
#define VEC2_H

struct Vec2
{
    int x;
    int y;

    Vec2() : x(0) , y(0)
    {}
    Vec2(int _x, int _y) : x(_x) , y(_y)
    {}
};

inline Vec2 operator + (Vec2 u, Vec2 v)
{
    Vec2 result;
    result.x = u.x + v.x;
    result.y = u.y + v.y;

    return result;
}
inline Vec2 operator += (Vec2 &u, Vec2 v)
{
    u.x += v.x;
    u.y += v.y;

    return u;
}
inline Vec2 operator * (Vec2 u, int s)
{
    Vec2 result;
    result.x = u.x * s;
    result.y = u.y * s;

    return result;
}
inline Vec2 operator *= (Vec2 &u, int s)
{
    u.x *= s;
    u.y *= s;

    return u;
}
inline bool operator < (const Vec2 &u, const Vec2 &v)
{
    if (u.x < v.x)
        return true;
    else if (u.x > v.x)
        return false;
    else
    {
        if (u.y < v.y)
            return true;
        else
            return false;
    }
}
inline bool operator == (const Vec2 &u, const Vec2 &v)
{
    return (u.x == v.x && u.y == v.y);
}

#endif // VEC2_H
