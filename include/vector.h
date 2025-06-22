#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 add(Vector3 a, Vector3 b);
Vector3 subtract(Vector3 a, Vector3 b);
Vector3 multiply(Vector3 a, float b);
Vector3 divide(Vector3 a, float b);
double dot(Vector3 a, Vector3 b);
Vector3 cross(Vector3 a, Vector3 b);
Vector3 norm(Vector3 a);
Vector3 copy(Vector3 a);
double angle(Vector3 a, Vector3 b);


#endif