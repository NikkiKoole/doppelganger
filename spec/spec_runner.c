#include <stdio.h>
#include "c89spec.h"
#include "../source/geom.h"
#include "../source/defines.h"
#include <math.h>
#include <string.h>

#define STRINGS_EQL(a, b) (strcmp((a),(b)) == 0)

describe(rectangles) {
    Point p;
    BBox b;
    BBox in;
    BBox out;
    BBox result;

    it (point in bbox should succeed) {
        p = (Point){100,100};
        b = (BBox){50,50,150,150};
        expect(point_in_bbox(p, b) == 1);
    }

    it (point in bbox should fail) {
        p = (Point){40,100};
        expect(point_in_bbox(p, b) == 0);
    }

    it (bbox in bbox should succeed) {
        in = (BBox){50,50,150,150};
        out = (BBox){10,50,150,150};
        expect(bbox_in_bbox(in, out));
    }

    it (bbox in bbox should fail) {
        in = (BBox){(Point){50,50}, (Point){150,150}};
        out = (BBox){(Point){60,50}, (Point){100,150}};
        expect(bbox_in_bbox(in, out) == 0);
    }

    it (clearly intersect bounding boxes) {
        in = (BBox){50,50,100,100};
        out = (BBox){60,50,100,100};
        expect(bbox_intersect(in, out, &result));
        expect(result.br.x - 40 == result.tl.x);
    }

    it (can be stringified) {
        in = (BBox){50, 50, 100, 100};
        char buffer[64];
        bbox_to_buffer(in, buffer);
        expect(STRINGS_EQL("BBox(50.0, 50.0, 100.0, 100.0)", buffer));
    }

    it (doesnt intersect bounding boxes packed against each other) {
        in = (BBox){50,50,100,100};
        out = (BBox){100,50,150,100};
        expect(bbox_intersect(in, out, &result) == 0);
    }

    it (can subtract two bounding boxes) {
        // in this situation we have to assert that the bounding boxes have the same width,
        // if they wouldn't then the result would not be a rectangle.

        BBox first = (BBox) {50,50,100,100};
        BBox second = (BBox) {50,50,100,100};
        //bbox_shrink_vertically();
        //bbox_grow_vertically();
        //bbox_subtract(second, first, &result);
        //expect(result.tl.x == result.br.x && result.tl.y == result.br.y);
    }

    it (can merge two bounding boxes) {
        // Same as the subtract I can only work with boxes that have the same width
        // bbox_merge_equal_width(this, other, result)
        // maybe a better name will be bbox_merge_with_similar_width(this, other, result)
        // the other one will be called bbox_subtract_from_similar_width(this, other, result)
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

    it (can be stringified) {
        result = vec2(1,2);
        char buffer[64];
        vec2_to_buffer(result, buffer);
        expect(STRINGS_EQL("Vec2(1.0, 2.0)", buffer));
    }

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
