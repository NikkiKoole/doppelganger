#include "entity.h"

Entity randomEntity(void){
    Entity result;
    result.position = (Vec2){randFloat()*500,randFloat()*500};
    result.velocity = (Vec2){-1.5 +randFloat()*3, -1.5 + randFloat()*3};
    result.red = randFloat()*0xFF;
    result.green = randFloat()*0xFF;
    result.blue = randFloat()*0xFF;
    return (result);
}
