#include <stdio.h>
#include "geom.h"
#include "defines.h"
#include <math.h>

void vec2_to_buffer(Vec2 v, char *buffer)
{
    sprintf(buffer, "Vec2(%.1f, %.1f)", v.x, v.y);
}
void bbox_to_buffer(BBox b, char *buffer)
{
    sprintf(buffer, "BBox(%.1f, %.1f, %.1f, %.1f)", b.tl.x, b.tl.y, b.br.x, b.br.y);
}


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

void vec2_swap(Vec2 *a, Vec2 *b)
{
    Vec2 tmp = *a;
    a->x = b->x;  a->y = b->y;
    b->x = tmp.x; b->y = tmp.y;
}

int point_in_bbox(Point p, BBox b)
{
    return ( ( p.x >= b.tl.x && p.x <= b.br.x ) &&
             ( p.y >= b.tl.y && p.y <= b.br.y) );
}

int bbox_eql(BBox b1, BBox b2)
{
    return (b1.tl.x == b2.tl.x &&
            b1.tl.y == b2.tl.y &&
            b1.br.x == b2.br.x &&
            b1.br.y == b2.br.y);
}

int bbox_in_bbox(BBox this, BBox other)
{
     return point_in_bbox( this.tl, other ) &&
         point_in_bbox( this.br, other );
}

int bbox_neighbour_vertically(BBox this, BBox other)
{
    return(this.br.y == other.tl.y || this.tl.y == other.br.y);
}

int bbox_intersect(BBox this, BBox other, BBox *result)
{
    result->tl.x = MAX(this.tl.x, other.tl.x);
    result->tl.y = MAX(this.tl.y, other.tl.y);
    result->br.x = MIN(this.br.x, other.br.x);
    result->br.y = MIN(this.br.y, other.br.y);

    return (result->br.x > result->tl.x &&
            result->br.y > result->tl.y);
}

void bbox_grow_vertically(BBox *this, BBox other)
{
    ASSERT(other.br.y == this->tl.y || other.tl.y == this->br.y);
    ASSERT(other.br.y >= other.tl.y);

    if (other.br.y == this->tl.y) {
        this->tl.y = other.tl.y;  // growing at the top
    } else if (other.tl.y == this->br.y) {
        this->br.y = other.br.y; // growing at the bottom
    }
}

void bbox_shrink_vertically(BBox *this, BBox other)
{
    ASSERT(other.tl.y == this->tl.y || other.br.y == this->br.y);
    ASSERT(other.br.y >= other.tl.y);

    if (other.tl.y == this->tl.y) {
        ASSERT(other.br.y <= this->br.y);
        this->tl.y = other.br.y;
    } else if ( other.br.y == this->br.y) {
        ASSERT(other.tl.y >= this->tl.y);
        this->br.y = other.tl.y;
    }
}
