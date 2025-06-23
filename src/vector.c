#include "vector.h"

Vector3 add(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 subtract(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vector3 multiply(Vector3 a, float b) {
    Vector3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

Vector3 multiplyVec(Vector3 a, Vector3 b){
    return (Vector3){a.x*b.x, a.y*b.y, a.z*b.z};
}

Vector3 divide(Vector3 a, float b) {
    if(b==0) return a;
    Vector3 result;
    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    return result;
}

double dot(Vector3 a, Vector3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3 cross(Vector3 a, Vector3 b){
    Vector3 c;
    c.x = a.y*b.z-b.y*a.z;
    c.y = a.z*b.x-b.z*a.x;
    c.z = a.x*b.y-b.x*a.y;
    return c;
}

double mag(Vector3 a){
    return (a.x*a.x + a.y*a.y + a.x*a.z);
}

Vector3 norm(Vector3 v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0) return (Vector3){0, 0, 0};
    return (Vector3){v.x / len, v.y / len, v.z / len};
}


Vector3 copy(Vector3 a){
    Vector3 b;
    b.x = a.x;
    b.y = a.y;
    b.z = a.z;
    return b;
}

double angle(Vector3 a, Vector3 b){
    double cos = dot(a,b)/(mag(a) * mag(b));
    return acos(cos);
}