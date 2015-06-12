#include <stdio.h>
#include "c89spec.h"
#include "../source/geom.h"
#include "../source/defines.h"
#include <math.h>


describe(defines) {
    it (min; positive ints) {
        assert(MIN(0, 23) == 0);
    }
    it (min; negative ints) {
        assert(MIN(-120, 23) == -120);
    }
    it (min; negative floats) {
        assert(MIN(-120.0f, 23) == -120.0);
    }
    it (max; negative ints) {
        assert(MAX(-9213123, -12312) == -12312);
    }
    it (max; positive floats) {
        assert(MAX(12312.123f, 888123.000023f) == 888123.000023f);
    }
    it (clamps; from negative to positive values) {
        assert(CLAMP(90, -123456, 654321) == 90);
    }
}


describe(geom) {
    Vec2 result;

    it (can add two vectors){
        result = vec2_add(vec2(1,9), vec2(3,7));
        assert(vec2_eql(result, vec2(4,16)));
    }

    it (can subtract two vectors) {
        result = vec2_sub(vec2(12, 34), vec2(8, 14));
        assert(vec2_eql(result, vec2(4, 20)));
    }
}

int main() {
    test(defines);
    test(geom);
    return summary();
}
