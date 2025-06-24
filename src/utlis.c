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


void createTree(Vector3 pos, float r, float height, int n){
    int m = 6;
    float angle = 2 * 3.1415f / m;
    for(int i = 0; i< 6; i++){
            Vector3 side1 = add(pos, (Vector3){r * cosf(angle*i), 0, r * sinf(angle*i)});
            Vector3 side2 = add(pos, (Vector3){r * cosf(angle*(i+1)), 0, r * sinf(angle*(i+1))});

            createTriangle(side1, side2, add(side1, (Vector3){0, height, 0}), (Vector3){180, 20, 20});
            createTriangle(side2, add(side2, (Vector3){0, height, 0}), add(side1, (Vector3){0, height, 0}), (Vector3){180, 20, 20});
    }
    
    createSphere(add(pos, (Vector3){0, height, 0}), 1.2, (Vector3){50, 160, 50}, 0, 1);

    for(int i = 0; i < n; i++){
        float dx = rand()%100/50.0f;
        float dy = rand()%100/50.0f;
        float dz = rand()%100/50.0f;
        createSphere(add(pos, (Vector3){dx - 1, dy - 1 + height, dz - 1}), 1.2 - dx/4, (Vector3){50+dz*5, 160+dx*5, 50+dy*5}, 0, 1);
    }

    // createSphere((Vector3){0.8, 2.5, 0.2}, 0.9, (Vector3){60, 170, 60}, 0, 1);
    // createSphere((Vector3){-0.7, 2.6, -0.3}, 1.0, (Vector3){55, 165, 55}, 0, 1);
    // createSphere((Vector3){0.3, 2.9, -0.6}, 0.8, (Vector3){70, 180, 70}, 0, 1);
    // createSphere((Vector3){-0.4, 2.8, 0.7}, 0.85, (Vector3){65, 175, 65}, 0, 1);
    // createSphere((Vector3){0.0, 3.2, 0.0}, 0.6, (Vector3){80, 190, 80}, 0, 1);
}