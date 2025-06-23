#include "utils.h"

Shape* shapes = NULL;

#include <stdlib.h>

float randomFloat() {
    return (float)rand() / (float)RAND_MAX;
}

void createSphere(Vector3 pos, float r, Vector3 color, float reflectivity, float smoothness) {
    Sphere s = {
        .pos = pos,
        .color = color,
        .r = r,
        .reflectivity = reflectivity,
        .smoothness = smoothness
    };

    Shape* node = (Shape*)malloc(sizeof(Shape));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for new shape.\n");
        exit(EXIT_FAILURE);  // or handle more gracefully
    }

    node->color = color;
    node->type = 0;
    node->sphere = s;
    node->next = shapes;  // Always prepend
    shapes = node;
}

void createTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 color){

    Triangle t = {
        .v1 = v1,
        .v2 = v2,
        .v3 = v3,
        .color = color
    };

    Shape* node = (Shape*)malloc(sizeof(Shape));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for new shape.\n");
        exit(EXIT_FAILURE);
    }

    node->color = color;
    node->type = 1;
    node->triangle = t;
    node->next = shapes;
    shapes = node;

}