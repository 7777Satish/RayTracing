#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "vector.h"


typedef struct Sphere{
    Vector3 pos;
    Vector3 color;
    float r;
    float reflectivity;
    float smoothness;
} Sphere;

typedef struct Triangle{
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    Vector3 color;
} Triangle;


typedef struct Shape{
    Sphere sphere;
    Triangle triangle;
    Vector3 color;
    int type;
    struct Shape* next;
} Shape;

typedef enum {
    SHAPE_SPHERE,
    // Other shape types...
} ShapeType;


extern Shape* shapes;

float randomFloat();
void createSphere(Vector3 pos, float r, Vector3 color, float reflectivity, float smoothness);
void createTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 color);
void createTree(Vector3 pos, float r, float height, int n);

#endif