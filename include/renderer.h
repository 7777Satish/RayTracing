#ifndef RENDERER_H
#define RENDERER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "vector.h"

typedef struct Square{
    Vector3 pos;
    Vector3 dim;
    Vector3 dir;
} Square;

typedef struct Sphere{
    Vector3 pos;
    Vector3 color;
    float r;
} Sphere;

typedef struct Triangle{
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    Vector3 color;
} Triangle;

typedef struct Camera{
    Vector3 pos;
    Vector3 dir;
} Camera;

typedef struct Light{
    Vector3 pos;
    Vector3 dir;
} Light;



extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern float screenDistance;
extern int innerWidth;
extern int innerHeight;
extern float FOV;

extern Camera camera;

extern Light light;

void init();
void renderBG();
void render();
Vector3 trace(Vector3 pos, Vector3 dir);

#endif