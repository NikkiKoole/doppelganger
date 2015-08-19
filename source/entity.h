
#ifndef ENTITY_H
#define ENTITY_H

#include "geom.h"
#include "defines.h"
typedef struct Entity {
    float x;
    float y;
    float z;
    Vec2 velocity;
    u8 red;
    u8 green;
    u8 blue;
} Entity;


Entity randomEntity(void);

#endif
