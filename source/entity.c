#include "entity.h"

Entity randomEntity(void){
    Entity result;
    result.x = (0)*16;
    result.y = (0)*16;
    result.z = 0;

    result.velocity = (Vec2){-150 +randFloat()*300, -150 + randFloat()*300};
    result.red = randFloat()*0xFF;
    result.green = randFloat()*0xFF;
    result.blue = randFloat()*0xFF;
    return (result);
}
