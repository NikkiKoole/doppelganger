#include "geom.h"
#include <math.h>

int vec2_eql(Vec2 v1, Vec2 v2)
{
    return (v1.x == v2.x && v1.y == v2.y);
}

Vec2 vec2(float x, float y)
{
    Vec2 result;
    result.x = x;
    result.y = y;
    return result;
}

Vec2 vec2_zero(void)
{
    return vec2(0, 0);
}
Vec2 vec2_unit(void)
{
    return vec2(1, 1);
}

Vec2 vec2_heading(float a)
{
    return vec2(cos(a), sin(a));
}

Vec2 vec2_add(Vec2 left, Vec2 right)
{
    Vec2 result;
    result.x = left.x + right.x;
    result.y = left.y + right.y;
    return result;
}

Vec2 vec2_sub(Vec2 left, Vec2 right)
{
    Vec2 result;
    result.x = left.x - right.x;
    result.y = left.y - right.y;
    return result;
}

Vec2 vec2_mul(Vec2 left, float mul)
{
    left.x *= mul;
    left.y *= mul;
    return left;
}

Vec2 vec2_div(Vec2 left, float div)
{
    left.x /= div;
    left.y /= div;
    return left;
}

float vec2_len_sqrd(Vec2 v)
{
    return(v.x*v.x + v.y*v.y);
}

float vec2_len(Vec2 v)
{
    return sqrt(vec2_len_sqrd(v));
}

float vec2_dist(Vec2 v1, Vec2 v2)
{
    return sqrt(vec2_dist_sqrd(v1, v2));
}

float vec2_dist_sqrd(Vec2 v1, Vec2 v2)
{
    return (v1.x - v2.x) * (v1.x - v2.x) +
           (v1.y - v2.y) * (v1.y - v2.y);
}

float vec2_dist_manh(Vec2 v1, Vec2 v2)
{
    return fabs(v1.x - v2.x) + fabs(v1.y - v2.y);
}

Vec2 vec2_normalize(Vec2 v)
{
    float len = vec2_len(v);
    return vec2_div(v, len);
}

void vec2_swap(Vec2* a, Vec2* b)
{
    Vec2 tmp = *a;
    a->x = b->x;  a->y = b->y;
    b->x = tmp.x; b->y = tmp.y;
}
