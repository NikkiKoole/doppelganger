#ifndef GEOM_H
#define GEOM_H


typedef struct Vec2
{
    float x;
    float y;
} Vec2;

void vec2_to_buffer(Vec2 v, char *buffer);
int vec2_eql(Vec2 v1, Vec2 v2);

Vec2 vec2(float x, float y);

Vec2 vec2_zero(void);
Vec2 vec2_unit(void);
Vec2 vec2_heading(float a);

Vec2 vec2_add(Vec2 left, Vec2 right);
Vec2 vec2_sub(Vec2 left, Vec2 right);
Vec2 vec2_mul(Vec2 left, float mul);
Vec2 vec2_div(Vec2 left, float div);

float vec2_len_sqrd(Vec2 v);
float vec2_len(Vec2 v);
float vec2_dist(Vec2 v1, Vec2 v2);
float vec2_dist_sqrd(Vec2 v1, Vec2 v2);
float vec2_dist_manh(Vec2 v1, Vec2 v2);

Vec2 vec2_normalize(Vec2 v);
void vec2_swap(Vec2 *a, Vec2 *b);

typedef Vec2 Point;

typedef struct BBox
{
    Point tl;
    Point br;
} BBox;
#define BBOX_HEIGHT(bbox) (bbox.br.y - bbox.tl.y)
#define BBOX_WIDTH(bbox) (bbox.br.x - bbox.tl.x)
void printBBox(BBox b);

BBox bbox(float tlX, float tlY, float brX, float brY);
int bbox_eql(BBox b1, BBox b2);
int bbox_neighbour_vertically(BBox this, BBox other);
void bbox_to_buffer(BBox b, char *buffer);
int point_in_bbox(Point p, BBox b);
int bbox_in_bbox(BBox this, BBox other);
int bbox_intersect(BBox this, BBox other, BBox *result);
void bbox_grow(BBox *this, BBox other);
void bbox_grow_vertically(BBox *this, BBox other);
void bbox_shrink_vertically(BBox *this, BBox other);

#endif
