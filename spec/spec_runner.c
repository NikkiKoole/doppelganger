#include <stdio.h>
#include "c89spec.h"
#include <math.h>
#include <string.h>
#include "../source/geom.h"
#include "../source/defines.h"
#include "../source/blockmap.h"

#define STRINGS_EQL(a, b) (strcmp((a),(b)) == 0)
#define BBOX_HEIGHT(a) (a.br.y - a.tl.y)
#define BBOX_WIDTH(a) (a.br.x - a.tl.x)

describe(blockmap) {
    it (3d blocky world dynamically sizeable) {
        World w = {.width=32, .height=32, .depth=32};
        w.blocks = malloc((32*32*32));
        setBlockAt(&w, 10, 10, 10, 100);
        expect(getBlockAt(&w, 10,10,10) == 100);
    }
    it (can look at the world from all 6 sides) {
        World w = {.width=32, .height=32, .depth=32};
        w.blocks = malloc((32*32*32));
        createOrdersFromDimensions(32, 32, 32, orderArray);
        //printf((unsigned int) front);
        printf("\n%d\n", orderArray[1].first_start);
        //setBlockAt(&w, 10, 10, 10, 100);
        expect(getBlockWhileLookingFromSide(&w, 0, 10,10,10) == 100);
    }
}



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
        expect(BBOX_WIDTH(result) == 40);
    }

    it (can be stringified) {
        in = (BBox){50, 50, 100, 100};
        char buffer[64];
        bbox_to_buffer(in, buffer);
        expect(STRINGS_EQL("BBox(50.0, 50.0, 100.0, 100.0)", buffer));
    }

    it (doesnt intersect bounding boxes packed against each other) {
        in = (BBox){50,50,100,100};
        out = (BBox){50,100,100,150};
        expect(bbox_intersect(in, out, &result) == 0);
    }

    it (knows when bounding boxes just touch when packed against each other as above) {
        in = (BBox){50,50,100,100};
        out = (BBox){50,100,100,150};
        expect(bbox_neighbour_vertically(in, out) == 1);

    }

    it (can grow bounding boxes vertically) {
        BBox first = (BBox) {50,50,100,100};
        BBox second = (BBox) {50,100,100,200};
        bbox_grow_vertically(&first, second);
        expect(BBOX_HEIGHT(first) == 150);
    }

    it (can shrink bounding boxes vertically) {
        BBox first = (BBox) {50,50,100,200};
        BBox second = (BBox) {50,100,100,200};
        bbox_shrink_vertically(&first, second);
        second = (BBox) {50,50,100,100};
        bbox_shrink_vertically(&first, second);
        expect(BBOX_HEIGHT(first) == 0);
    }

    it (succeeds at a situation with differently sized bounding boxes) {

        //     +---+
        //     | A |
        // +---+---+----+
        // |   |   |    |
        // |   | B |    |
        // |   +---+    |
        // |     C      |
        // +------------+
        // in the above case I start with the rectangle A+B and another one C
        // the intersection of the two is the B shape
        // I can shrink the AB one with B to get at A

        BBox AB = (BBox) {100,100,200,400};
        BBox C = (BBox) {0,200,600,600};
        BBox intersect;
        bbox_intersect(AB, C, &intersect);
        expect(bbox_eql(intersect, (BBox){100,200,200,400}));
        bbox_shrink_vertically(&AB, intersect);
        expect(bbox_eql(AB, (BBox){100,100,200,200}));

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
    test(blockmap);
    /* test(rectangles); */
    /* test(defines); */
    /* test(geom); */
    return summary();
}
