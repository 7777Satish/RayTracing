#include "renderer.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int innerWidth = 1000;
int innerHeight = 500;
float screenDistance = 0.5;
float FOV = 120;
int NUMBER_OF_THREADS = 15;

Camera camera = {
    .pos = {0, 13, -1},
    .dir = {0, -1.2, 1}
};

Light light = {
    .pos = {0, 2, 6.5},
    .dir = {1, 0, 0}
};


#define MAX_VERTICES 1000
#define MAX_LINE_LEN 256

Vector3 pixelBuffer[500][1000];

void init(){
    srand(time(NULL));
    window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, innerWidth, innerHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Ground
    createSphere((Vector3){0, -1000, 0}, 1000, (Vector3){129, 255, 127}, 0.0f, 0);

    // for(int i = 0; i < 5; i++){
    //     float z = (rand() % 1000) / 100.0f + 4.1;
    //     float x = (rand() % 1000) / 100 - 5;
    //     createTree((Vector3){x, 0, z}, 0.5, 3, 5);
    // }

    createTree((Vector3){10, 0, 12}, 0.5, 3, 5);
    createTree((Vector3){-12, 0, 17}, 0.5, 3, 5);
    createTree((Vector3){-9, 0, 6}, 0.5, 3, 5);
    createTree((Vector3){0, 0, 10}, 0.5, 3, 5);
    createTree((Vector3){9, 0, 14}, 0.5, 3, 5);
    createTree((Vector3){2.2, 0, 0.9}, 0.5, 3, 5);

    // Large central spheres
    createSphere((Vector3){-4, 1, 7}, 1.0f, (Vector3){212, 151, 126}, 0.0f, 0.1);
    createSphere((Vector3){5, 1, 5.3}, 1.0f, (Vector3){255, 255, 255}, 1.0f, 1.0);
    // createSphere((Vector3){4, 1, 3}, 1.0f, (Vector3){255, 255, 255}, 0.0f, 0);

    // createTriangle((Vector3){-1, -1, 1}, (Vector3){0, 1, 2}, (Vector3){1, -1, 2}, (Vector3){100, 100, 100});

    // Random spheres
    for (int a = -11; a < 11; a+=1) {
        for (int b = -11; b < 11; b+=1) {
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

    createSphere((Vector3){3, 1.5, 10}, 1.5, (Vector3){155, 100, 200}, 0.5, 1);
    createSphere((Vector3){-2, 1.5, 1}, 1.5, (Vector3){55, 100, 200}, 1, 1);
    // createSphere((Vector3){1.8, 1.5, 4.5}, 1.5, (Vector3){55, 10, 200}, 0.5, 0.9);

    camera.dir = norm(camera.dir);
}

void renderBG(){
    for(int j=0; j<innerHeight; j++){
        int blue = (j+0.0)/innerHeight * 120 + 100;
        // SDL_SetRenderDrawColor(renderer, blue-30, blue-30, 225, 255);
        // SDL_Rect r = {0, j, innerWidth, 1};
        // SDL_RenderFillRect(renderer, &r);

        for(int i=0; i<innerWidth; i++){
            pixelBuffer[j][i] = (Vector3){blue-30, blue-30, 255};
        }
    }
}

void* renderMpixels(void* arg){
    int n = *((int*)arg);

    int area = innerWidth * innerHeight;
    int noOfPixelsPerThread = area/NUMBER_OF_THREADS;

    float aspect = (float)innerWidth / innerHeight;
    float scale = tanf((FOV * 0.5f) * 3.1415 / 180.0f);

    int start = n * noOfPixelsPerThread;
    int end = (n == NUMBER_OF_THREADS - 1) ? area : (n + 1) * noOfPixelsPerThread;

    float pixelWidth = 1.0 / innerWidth;
    float pixelHeight = 1.0 / innerHeight;

    for(int i = start; i < end; i++){
        int x = i%innerWidth;
        int y = i/innerWidth;

        float u = (2 * ((x + 0.5f) / innerWidth) - 1) * scale;
        float v = (1 - 2 * ((y + 0.5f) / innerHeight)) * (scale / aspect);

        Vector3 forward = norm(camera.dir);
        Vector3 right   = norm(cross((Vector3){0, 1, 0}, forward));
        Vector3 up      = cross(forward, right);

        float offsets[4][2] = {
            { -0.25f, -0.25f },
            {  0.25f, -0.25f },
            { -0.25f,  0.25f },
            {  0.25f,  0.25f },
        };

        Vector3 color = {0, 0, 0};

        for (int k = 0; k < 4; k++) {
            float du = u + offsets[k][0] * pixelWidth;
            float dv = v + offsets[k][1] * pixelHeight;

            Vector3 ray = norm(add(add(
                multiply(right, du),
                multiply(up, dv)
            ), forward));

            color = add(color, trace(camera.pos, ray, 1));
        }

        color = divide(color, 4); // average of 4 samples

        if(color.x != -1 && !(color.x==0 && color.y==0 && color.z==0)){
            // SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);

            pixelBuffer[y][x] = color;
            // SDL_RenderDrawPoint(renderer, x, y);
        }
        
    }

    pthread_exit(NULL);
}

void render(){
    renderBG();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    pthread_t threads[NUMBER_OF_THREADS];
    int thread_ids[NUMBER_OF_THREADS];

    for(int i = 0; i<NUMBER_OF_THREADS; i++){
        thread_ids[i] = i;
        int result = pthread_create(&threads[i], NULL, renderMpixels, (void*)(&thread_ids[i]));
        if (result != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return;
        }
    }

    for(int i = 0; i<NUMBER_OF_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    SDL_Delay(1000);
    for(int j = 0; j<innerHeight; j++){
        for(int i = 0; i<innerWidth; i++){
            Vector3 color = pixelBuffer[j][i];
            SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
            SDL_RenderDrawPoint(renderer, i, j);
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

            Vector3 lightDir = norm(subtract(light.pos, pointOnCircle));
            // Vector3 lightDir = {10, 100, 100};
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
            Triangle t = node->triangle;
             
            float lambda;

            if(!rayIntersectsTriangle(pos, dir, t.v1, t.v2, t.v3, &lambda)){
                node = node->next;
                continue;
            }

            if((minLambda!=-1 && lambda>=minLambda) || lambda<0){
                node = node->next;
                continue;
            }

            renderedShape = node;
            minLambda = lambda;


            Vector3 pointOnPlane = add(pos, multiply(dir, lambda));
            // Vector3 pointOnPlane = subtract(pointOnPlane, s.pos);
            // Vector3 normal = norm(pointOnPlane);

            Vector3 lightDir = norm(subtract(light.pos, pointOnPlane));

            Vector3 normal = norm(cross(subtract(t.v3, t.v1), subtract(t.v2, t.v1)));

            // Vector3 lightDir = {10, 100, -10};
            lightDir = norm(lightDir);

            float d = dot(normal, lightDir);

            if(iteration==1){
                Vector3 n_pos = add(pos, multiply(dir, minLambda));
                n_pos = add(n_pos, multiply(normal, 0.001f)); // prevent self-hit
                Vector3 color2 = trace(n_pos, lightDir, 100);
    
                if(color2.x != -1) d = 0;
            }

            d = fmaxf(d, 0.0f);
            float ambient = 0.1f;
            intensity = ambient + d * 0.9f;
        }

        node = node->next;
    }

    if(!renderedShape){
        return (Vector3){-1, -1, -1};
    }

    intensity = powf(intensity, 1.0f/2.2f);

    Vector3 color = multiply(renderedShape->color, fmin(fmax(intensity, 0.0), 1.0));

    if (iteration <= 3 && renderedShape->type == 0 && renderedShape->sphere.reflectivity != 0) {

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

int rayIntersectsTriangle(Vector3 orig, Vector3 dir, Vector3 v0, Vector3 v1, Vector3 v2, float* lambda) {
    const float EPSILON = 0.0000001;

    Vector3 edge1 = subtract(v1, v0);
    Vector3 edge2 = subtract(v2, v0);

    Vector3 h = cross(dir, edge2);
    float a = dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return 0; // Ray is parallel to triangle

    float f = 1.0 / a;
    Vector3 s = subtract(orig, v0);
    float u = f * dot(s, h);

    if (u < 0.0 || u > 1.0)
        return 0;

    Vector3 q = cross(s, edge1);
    float v = f * dot(dir, q);

    if (v < 0.0 || u + v > 1.0)
        return 0;

    // At this point, we know the ray intersects the triangle
    float t = f * dot(edge2, q);
    if (t > EPSILON) { // ray intersection
        *lambda = t;
        return 1;
    } else {
        return 0; // Line intersection but not a ray intersection
    }
}


// int rayIntersectsTriangle(Vector3 pos, Vector3 dir, Vector3 v1, Vector3 v2, Vector3 v3, float* lambda){

//     Vector3 edge1 = subtract(v2, v1);
//     Vector3 edge2 = subtract(v3, v1);

//     Vector3 normal = cross(edge1, edge2);
//     float dotNormalDir = dot(normal, dir);

//     if(dotNormalDir < 0.00001f){
//         return 0; // Ray is parellel to the surface
//     }



//     // (1-u-v)A + uB + vC = pos + (lambda)dir
    
// }