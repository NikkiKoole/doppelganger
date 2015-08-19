#include "entity.h"

Entity randomEntity(void){
    Entity result;
    result.x = (10+randFloat()*20)*16;
    result.y = (10+randFloat()*20)*16;
    result.z = 0;
    result.velocity = (Vec2){-1.5 +randFloat()*3, -1.5 + randFloat()*3};
    result.red = randFloat()*0xFF;
    result.green = randFloat()*0xFF;
    result.blue = randFloat()*0xFF;
    return (result);
}
