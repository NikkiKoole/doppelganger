#include <stdio.h>
#include "c89spec.h"
#include "../source/geom.h"
#include <math.h>

describe(geom) {
    Vec2 result;

    it (can add two vectors){
        result = vec2_add(vec2(1,9), vec2(3,7));
        assert(vec2_eql(result, vec2(4,16)));
    }

    it (can subtract two vevtors) {
        result = vec2_sub(vec2(12, 34), vec2(8, 14));
        assert(vec2_eql(result, vec2(4, 20)));
    }
}

int main() {
   test(geom);
   return summary();
}
