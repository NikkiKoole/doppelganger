#include <stdio.h>
#include "c89spec.h"
#include <math.h>
#include <string.h>
#include "../source/geom.h"
#include "../source/defines.h"
#include "../source/blockmap.h"
#include "../source/linkedlist.h"

#define STRINGS_EQL(a, b) (strcmp((a),(b)) == 0)

describe(double_linked_list) {
    it (can add_last) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 1;
        int mock2 = 12;
        list_add_last(list, make_node_calloc(&mock1));
        list_add_last(list, make_node_calloc(&mock2));

        expect(list->length == 2);
        int *value = list->last->value;
        expect(*value == 12);
        int *value2 = list->first->value;
        expect(*value2 == 1);
    }

    it (has a handy foreach macro) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 1;
        int mock2 = 2;
        int mock3 = 3;
        int mock4 = 5;
        int mock5 = 7;
        int mock6 = 11;
        list_add_last(list, make_node_calloc(&mock1));
        list_add_last(list, make_node_calloc(&mock2));
        list_add_last(list, make_node_calloc(&mock3));
        list_add_last(list, make_node_calloc(&mock4));
        list_add_last(list, make_node_calloc(&mock5));
        list_add_last(list, make_node_calloc(&mock6));
        int running_total = 0;

        LIST_FOREACH(list, first, next, cur) {
            int *v = cur->value;
            running_total += *v;

        }
        expect(list->length == 6);
        expect(running_total == 1+2+3+5+7+11);

    }
    it (can pop_last) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 134;
        int mock2 = 12;
        list_add_last(list, make_node_calloc(&mock1));
        list_add_last(list, make_node_calloc(&mock2));
        expect(list->length == 2);
        int *value = list_pop(list);
        expect(list->length == 1);
        expect(*value == 12);
    }
    it (can add_first) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 1;
        int mock2 = 23;
        list_add_first(list, make_node_calloc( &mock1));
        list_add_first(list, make_node_calloc(&mock2));
        expect(list->length == 2);
        int *value = list->first->value;
        expect(*value == 23);
        int *value2 = list->last->value;
        expect(*value2 == 1);
    }
    it (can shift the first) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 1;
        int mock2 = 23;
        list_add_first(list, make_node_calloc(&mock1));
        list_add_first(list, make_node_calloc(&mock2));
        expect(list->length == 2);
        int *value = list_shift(list);
        expect(list->length == 1);
        expect(*value == 23);
    }

    it (can remove from the middle) {
        List *list = calloc(1, sizeof(List));
        int mock1 = 1;
        int mock2 = 23;
        int mock3 = 69;
        list_add_first(list, make_node_calloc(&mock1));
        list_add_first(list, make_node_calloc(&mock2));
        list_add_first(list, make_node_calloc(&mock3));
        int *value = list_remove(list, list->first->next);
        expect(list->length == 2);
        expect(*value == 23);
    }
}

describe(blockmap) {
    it (3d blocky world dynamically sizeable) {
        World w = {.width=11, .height=11, .depth=11};
        w.blocks = malloc((w.width*w.height*w.depth));
        setBlockAt(&w, 10, 10, 10, 100);
        expect(getBlockAt(&w, 10,10,10) == 100);
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
    test(double_linked_list);
    /* test(blockmap); */
    test(rectangles);
    /* test(defines); */
    test(geom);
    return summary();
}
