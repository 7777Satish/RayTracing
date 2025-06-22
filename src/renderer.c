#include "renderer.h"
#include "utils.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int innerWidth = 1000;
int innerHeight = 500;
float screenDistance = 0.5;
float FOV = 90;

Camera camera = {
    .pos = {0, 1, -2},
    .dir = {0, -0.2, 1}
};

Light light = {
    .pos = {0, 10, -10},
    .dir = {1, 0, 0}
};


void init(){
    window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, innerWidth, innerHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // createSphere((Vector3){0, -1002, 0}, 1000, (Vector3){129, 255, 127});
    createSphere((Vector3){0, -1002, 0}, 1000, (Vector3){140, 156, 182});

    createSphere((Vector3){-67.1, -1.8, 4.2}, 0.2, (Vector3){255, 228, 225});
    createSphere((Vector3){3.3, -0.7, 3.5}, 0.3, (Vector3){176, 224, 230});
    createSphere((Vector3){-34.9, -1.74, 6.6}, 0.26, (Vector3){152, 251, 152});
    createSphere((Vector3){21.4, -1.76, 4.3}, 0.24, (Vector3){250, 250, 210});
    createSphere((Vector3){-89.2, -1.75, 11.2}, 0.25, (Vector3){255, 182, 193});
    createSphere((Vector3){4.7, -1.7, 9.1}, 0.3, (Vector3){221, 160, 221});
    createSphere((Vector3){-8.6, -1.73, 7.9}, 0.27, (Vector3){144, 238, 144});
    createSphere((Vector3){3.5, -1.78, 4.8}, 0.22, (Vector3){238, 221, 130});
    createSphere((Vector3){-11.9, -1.8, 12.4}, 0.2, (Vector3){173, 216, 230});
    createSphere((Vector3){6.8, -1.76, 5.7}, 0.24, (Vector3){255, 222, 173});
    createSphere((Vector3){-4.3, -1.71, 13.3}, 0.29, (Vector3){255, 160, 122});
    createSphere((Vector3){2.4, -1.79, 6.2}, 0.21, (Vector3){255, 239, 213});
    createSphere((Vector3){9.9, -1.75, 4.6}, 0.25, (Vector3){240, 230, 140});
    createSphere((Vector3){-7.0, -1.74, 5.3}, 0.26, (Vector3){152, 251, 152});
    createSphere((Vector3){8.6, -1.72, 7.1}, 0.28, (Vector3){255, 218, 185});
    createSphere((Vector3){-2.5, -1.77, 3.9}, 0.23, (Vector3){240, 128, 128});
    createSphere((Vector3){5.3, -1.8, 11.9}, 0.2, (Vector3){175, 238, 238});
    createSphere((Vector3){-9.4, -1.73, 9.6}, 0.27, (Vector3){255, 250, 205});
    createSphere((Vector3){5.7, -1.74, 4.6}, 0.26, (Vector3){255, 228, 196});
    createSphere((Vector3){-4.2, -1.8, 13.7}, 0.2, (Vector3){216, 191, 216});
    createSphere((Vector3){8.9, -1.75, 10.2}, 0.25, (Vector3){255, 240, 245});
    createSphere((Vector3){-18.6, -1.7, 7.0}, 0.3, (Vector3){175, 238, 238});
    createSphere((Vector3){53.8, -1.72, 8.9}, 0.28, (Vector3){255, 239, 213});
    createSphere((Vector3){-64.7, -1.76, 12.8}, 0.24, (Vector3){152, 251, 152});
    createSphere((Vector3){29.4, -1.73, 5.4}, 0.27, (Vector3){255, 192, 203});
    createSphere((Vector3){-49.2, -1.78, 6.8}, 0.22, (Vector3){173, 255, 47});
    createSphere((Vector3){17.1, -1.8, 4.3}, 0.2, (Vector3){255, 228, 181});
    createSphere((Vector3){12.6, -1.74, 9.5}, 0.26, (Vector3){255, 250, 250});
    createSphere((Vector3){-3.1, -1.7, 10.7}, 0.3, (Vector3){176, 224, 230});
    createSphere((Vector3){4.0, -1.72, 13.1}, 0.28, (Vector3){230, 230, 250});

    createSphere((Vector3){0, -0.5, 10}, 1.5, (Vector3){55, 100, 200});
    createSphere((Vector3){-3, -0.5, 14}, 1.5, (Vector3){155, 100, 200});
    createSphere((Vector3){2, -0.5, 6}, 1.5, (Vector3){55, 10, 200});
    createTriangle((Vector3){-1, 0, 2}, (Vector3){-1, 1, 3}, (Vector3){1, 0, 2}, (Vector3){0, 100, 0});
    camera.dir = norm(camera.dir);
}

void renderBG(){
    for(int j=0; j<innerHeight; j++){
        int blue = (j+0.0)/innerHeight * 120 + 100;
        SDL_SetRenderDrawColor(renderer, blue-30, blue-30, 225, 255);
        SDL_Rect r = {0, j, innerWidth, 1};
        SDL_RenderFillRect(renderer, &r);
    }
}

void render(){
    renderBG();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    float aspect = (float)innerWidth / innerHeight;
    float scale = tanf((FOV * 0.5f) * 3.1415 / 180.0f);

    for (int i = 0; i < innerWidth; i++) {
        for (int j = 0; j < innerHeight; j++) {
            float u = (2 * ((i + 0.5f) / innerWidth) - 1) * scale;
            float v = (1 - 2 * ((j + 0.5f) / innerHeight)) * (scale / aspect);

            // Vector3 ray = norm((Vector3){u, v, 1});

            Vector3 forward = norm(camera.dir);
            Vector3 right   = norm(cross((Vector3){0, 1, 0}, forward));
            Vector3 up      = cross(forward, right);

            Vector3 ray = norm(add(add(
                multiply(right, u),
                multiply(up, v)
            ),
            forward));
            
            Vector3 color = trace(camera.pos, ray);
            if(color.x != -1 && !(color.x==0 && color.y==0 && color.z==0)){
                SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
    
                SDL_RenderDrawPoint(renderer, i, j);
            }
        
        }
    }
}

Vector3 trace(Vector3 pos, Vector3 dir){
    float a = dot(dir, dir);

    Shape* node = shapes;

    float intensity = 0;
    float minLambda = -1;
    Shape* renderedShape = NULL;
    while(node!=NULL){
        if(node->type == 0){
            Sphere s = node->sphere;
            float b = 2 * ( (pos.x - s.pos.x) * dir.x + (pos.y - s.pos.y) * dir.y + (pos.z - s.pos.z) * dir.z );
            float c = ( (pos.x - s.pos.x)*(pos.x - s.pos.x) + (pos.y - s.pos.y)*(pos.y - s.pos.y) + (pos.z - s.pos.z)*(pos.z - s.pos.z) ) - ( s.r * s.r );
            double D = b*b - 4*a*c;
            if(D<0){
                node = node->next;
                continue;
            };

            float lambda1 = (-b + sqrt(D))/(2*a);
            float lambda2 = (-b - sqrt(D))/(2*a);

            float lambda;
            if(lambda1>0 && lambda2>0) lambda = lambda1<lambda2?lambda1:lambda2;
            else if (lambda1>0) lambda = lambda1;
            else lambda = lambda2;

            if((minLambda!=-1 && lambda>=minLambda) || lambda<0){
                node = node->next;
                continue;
            }

            renderedShape = node;
            minLambda = lambda;


            Vector3 pointOnCircle = add(pos, multiply(dir, lambda));
            Vector3 pointFromCircle = subtract(pointOnCircle, s.pos);
            Vector3 normal = norm(pointFromCircle);

            // Vector3 lightDir = norm(subtract(light.pos, pointOnCircle));
            Vector3 lightDir = {10, 100, -10};
            lightDir = norm(lightDir);
            float d = dot(normal, lightDir);
            d = fmaxf(d, 0.0f);
            float ambient = 0.1f;
            intensity = ambient + d * 0.9f;
        }
        
        else if(node->type == 1){
            // Triangle t = node->triangle;
             
            // float lambda;
            
            // if((minLambda!=-1 && lambda>=minLambda) || lambda<0){
            //     node = node->next;
            //     continue;
            // }

            // renderedShape = node;
            // minLambda = lambda;


            // Vector3 pointOnCircle = add(pos, multiply(dir, lambda));
            // Vector3 pointFromCircle = subtract(pointOnCircle, s.pos);
            // Vector3 normal = norm(pointFromCircle);

            // Vector3 lightDir = norm(subtract(light.pos, pointOnCircle));
            // float d = dot(normal, lightDir);
            // d = fmaxf(d, 0.0f);
            // float ambient = 0.1f;
            // intensity = ambient + d * 0.9f;
        }

        node = node->next;
    }

    if(!renderedShape){
        return (Vector3){-1, -1, -1};
    }

    intensity = powf(intensity, 1.0f/2.2f);

    Vector3 color = multiply(renderedShape->color, fmin(fmax(intensity, 0.0), 1.0));

    return color;
}