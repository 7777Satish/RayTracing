#include "renderer.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int innerWidth = 1000;
int innerHeight = 500;
float screenDistance = 0.5;
float FOV = 90;

Camera camera = {
    .pos = {0, 3, -2},
    .dir = {0, -0.5, 1}
};

Light light = {
    .pos = {-0.5, 2, 6.5},
    .dir = {1, 0, 0}
};


#define MAX_VERTICES 1000
#define MAX_LINE_LEN 256

void init(){
    srand(time(NULL));
    window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, innerWidth, innerHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Ground
    createSphere((Vector3){0, -1000, 0}, 1000, (Vector3){129, 255, 127}, 0.8f, 0);

    // Large central spheres
    createSphere((Vector3){-4, 1, 7}, 1.0f, (Vector3){102, 51, 126}, 0.0f, 0.1);
    createSphere((Vector3){0, 1, 5}, 1.0f, (Vector3){255, 255, 255}, 1.0f, 1.0);
    createSphere((Vector3){4, 1, 3}, 1.0f, (Vector3){255, 255, 255}, 0.0f, 0);

    // Random spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = randomFloat(); // 0 to 1
            Vector3 center = {
                a + 0.9f * randomFloat(),
                0.2f,
                2+b + 0.9f * randomFloat()
            };

            if (mag(subtract(center, (Vector3){4, 0.2f, 0})) > 0.9f) {
                Vector3 color;
                float reflectivity = 0.0f;

                if (chooseMat < 0.8f) {
                    // Diffuse (random pastel)
                    color = (Vector3){
                        rand() % 256,
                        rand() % 256,
                        rand() % 256
                    };
                } else if (chooseMat < 0.95f) {
                    // Metallic
                    int r = 128 + rand() % 128;
                    int g = 128 + rand() % 128;
                    int b = 128 + rand() % 128;
                    color = (Vector3){r, g, b};
                    reflectivity = 0.1f;
                } else {
                    // Glass-like white
                    color = (Vector3){255, 255, 255};
                    reflectivity = 0.3f;
                }

                createSphere(center, 0.2f, color, reflectivity, rand()%100/100.0f);
            }
        }
    }

    createSphere((Vector3){-3, 1.5, 14}, 1.5, (Vector3){155, 100, 200}, 0.5, 1);
    createSphere((Vector3){0, 1.5, 10}, 1.5, (Vector3){55, 100, 200}, 1, 0.5);
    createSphere((Vector3){2.8, 1.5, 6}, 1.5, (Vector3){55, 10, 200}, 0.5, 0.9);

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
            
            Vector3 color = trace(camera.pos, ray, 1);
            if(color.x != -1 && !(color.x==0 && color.y==0 && color.z==0)){
                SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
    
                SDL_RenderDrawPoint(renderer, i, j);
            }
        
        }
    }
}

Vector3 trace(Vector3 pos, Vector3 dir, int iteration){
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
            Vector3 lightDir = {10, 100, 100};
            lightDir = norm(lightDir);
            float d = dot(normal, lightDir);

            if(iteration==1){
                Vector3 n_pos = add(pos, multiply(dir, minLambda));
                n_pos = add(n_pos, multiply(normal, 0.001f)); // prevent self-hit
                Vector3 color2 = trace(n_pos, lightDir, 100);
    
                if(color2.x != -1) d = 0;
            }

            d = fmaxf(d, 0.0f);
            float ambient = 0.15f;
            intensity = ambient + d * 0.9f;
        }
        
        else if(node->type == 1){
            // Triangle t = node->triangle;
             
            // float lambda;

            // rayIntersectsTriangle(pos, dir, t.v1, t.v2, t.v3, &lambda);
            
            // if((minLambda!=-1 && lambda>=minLambda) || lambda<0){
            //     node = node->next;
            //     continue;
            // }

            // renderedShape = node;
            // minLambda = lambda;


            // // Vector3 pointOnCircle = add(pos, multiply(dir, lambda));
            // // Vector3 pointFromCircle = subtract(pointOnCircle, s.pos);
            // // Vector3 normal = norm(pointFromCircle);

            // // Vector3 lightDir = norm(subtract(light.pos, pointOnCircle));

            // Vector3 normal = norm(cross(subtract(t.v3, t.v1), subtract(t.v2, t.v1)));

            // Vector3 lightDir = {10, 100, -10};
            // lightDir = norm(lightDir);

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

    if (iteration <= 3 && renderedShape->sphere.reflectivity != 0) {

        Vector3 n_pos = add(pos, multiply(dir, minLambda));
        Vector3 normal = norm(subtract(n_pos, renderedShape->sphere.pos));

        Vector3 noice = {
            0.3 * (rand()%100 + 0.0f)/100.0f,
            0.3 * (rand()%100 + 0.0f)/100.0f,
            0.3 * (rand()%100 + 0.0f)/100.0f
        };

        noice = multiply(noice, 1-renderedShape->sphere.smoothness);

        Vector3 n_dir = subtract(dir, multiply(normal, 2.0f * dot(normal, dir)));
        n_dir = add(n_dir, noice);
        n_pos = add(n_pos, multiply(normal, 0.001f));
        Vector3 color2 = trace(n_pos, norm(n_dir), iteration+1);
        
        if(color2.x==-1) color2 = (Vector3){80, 90, 255};

        if(color2.x != -1){
            color = add(
                multiply(color2, renderedShape->sphere.reflectivity),
                multiply(color, 1.0f - renderedShape->sphere.reflectivity)
            );
        }
    }

    return color;
}

// int rayIntersectsTriangle(Vector3 pos, Vector3 dir, Vector3 v1, Vector3 v2, Vector3 v3, float* lambda){

//     Vector3 edge1 = subtract(v2, v1);
//     Vector3 edge2 = subtract(v3, v1);

//     Vector3 normal = cross(edge1, edge2);
//     float dotNormalDir = dot(normal, dir);

//     if(dotNormalDir < 0.00001f){
//         return 0;
//     }

//     // (1-u-v)A + uB + vC = pos + (lambda)dir
    
// }