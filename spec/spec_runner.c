#include <stdio.h>
#include "c89spec.h"
#include "../source/geom.h"
#include "../source/defines.h"
#include <math.h>


describe(rectangles) {
    Point p;
    BBox b;
    BBox in;
    BBox out;
    BBox result;

    it (point in bbox should succeed) {
        p = (Point){100,100};
        b = (BBox){(Point){50,50}, (Point){150,150}};
        expect(point_in_bbox(p, b) == 1);
    }
    it (point in bbox should fail) {
        p = (Point){40,100};
        expect(point_in_bbox(p, b) == 0);
    }

    it (bbox in bbox should succeed) {
        in = (BBox){(Point){50,50}, (Point){150,150}};
        out = (BBox){(Point){10,50}, (Point){150,150}};
        expect(bbox_in_bbox(in, out));
    }
    it (bbox in bbox should fail) {
        in = (BBox){(Point){50,50}, (Point){150,150}};
        out = (BBox){(Point){60,50}, (Point){100,150}};
        expect(bbox_in_bbox(in, out) == 0);
    }
    it (clearly intersect bounding boxes) {
        in = (BBox){(Point){50,50}, (Point){100,100}};
        out = (BBox){(Point){60,50}, (Point){100,100}};
        expect(bbox_intersect(in, out, &result));
        expect(result.br.x - 40 == result.tl.x);
    }
    it (doesnt intersect bounding boxes packed against each other) {
        in = (BBox){(Point){50,50}, (Point){100,100}};
        out = (BBox){(Point){100,50}, (Point){150,100}};
        expect(bbox_intersect(in, out, &result) == 0);
        printf("\n%s.\n",vec2_to_string((Vec2){1.0f/3,1}));
    }


}

describe(defines) {
    it (min; positive ints) {
        expect(MIN(0, 23) == 0);
    }
    it (min; negative ints) {
        expect(MIN(-120, 23) == -120);
    }
    it (min; negative floats) {
        expect(MIN(-120.0f, 23) == -120.0);
    }
    it (max; negative ints) {
        expect(MAX(-9213123, -12312) == -12312);
    }
    it (max; positive floats) {
        expect(MAX(12312.123f, 888123.000023f) == 888123.000023f);
    }
    it (clamps; from negative to positive values) {
        expect(CLAMP(90, -123456, 654321) == 90);
    }
}


describe(geom) {
    Vec2 result;

    it (can add two vectors){
        result = vec2_add(vec2(1,9), vec2(3,7));
        expect(vec2_eql(result, vec2(4,16)));
    }

    it (can subtract two vectors) {
        result = vec2_sub(vec2(12, 34), vec2(8, 14));
        expect(vec2_eql(result, vec2(4, 20)));
    }
}

int main() {
    test(rectangles);
    test(defines);
    test(geom);
    return summary();
}
