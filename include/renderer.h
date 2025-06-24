#ifndef RENDERER_H
#define RENDERER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "vector.h"
#include "utils.h"
#include <pthread.h>

typedef struct Square{
    Vector3 pos;
    Vector3 dim;
    Vector3 dir;
} Square;

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
extern int NUMBER_OF_THREADS;

extern Camera camera;

extern Light light;

void init();
void renderBG();
void* renderMpixels(void* arg);
void render();
Vector3 trace(Vector3 pos, Vector3 dir, int iteration);
int rayIntersectsTriangle(Vector3 pos, Vector3 dir,
                           Vector3 v0, Vector3 v1, Vector3 v2,
                           float* lambda);

#endif