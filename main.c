#include "../raylib/src/raylib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_PARTICLES 100000
#define PARTICLE_SIZE 0.5f
#define MIN_VELOCITY -2.0f
#define MAX_VELOCITY 2.0f
#define MOUSE_INFLUENCE 200.0f
#define SQUARED_MOUSE_INFLUENCE (MOUSE_INFLUENCE * MOUSE_INFLUENCE)
#define NORMAL_SPEED 100.0f
#define VORTEX_SPEED 400.0f
#define VORTEX_FORCE 8.0f

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
    bool active;
} Particle;

float RandomFloat(float min, float max);
void initParticle(int index);
void UpdateAndDrawParticles(Vector2 mousePos, bool isVortexActive, float deltaTime);

Particle particles[MAX_PARTICLES];


int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    srand(time(NULL));
    
    InitWindow(screenWidth, screenHeight, "Flocking");
    SetTargetFPS(60);
    
    // Initialize all particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        initParticle(i);
    }
    
    while (!WindowShouldClose()) {
        UpdateAndDrawParticles(
            GetMousePosition(),
            IsMouseButtonDown(MOUSE_LEFT_BUTTON),
            GetFrameTime()
        );
    }
    
    CloseWindow();
    return 0;
}


float RandomFloat(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

void initParticle(int index) {
    particles[index].position = (Vector2) {
        RandomFloat(0, GetScreenWidth()),
        RandomFloat(0, GetScreenHeight())
    };

    particles[index].velocity = (Vector2) {
        RandomFloat(MIN_VELOCITY, MAX_VELOCITY),
        RandomFloat(MIN_VELOCITY, MAX_VELOCITY)
    };

    unsigned char grey = (unsigned char)(rand() % 40);
    particles[index].color = (Color){
        grey, grey, grey, 255
    };

    particles[index].life = RandomFloat(1.0f, 10.0f);
    particles[index].active = true;
}

void UpdateAndDrawParticles(Vector2 mousePos, bool isVortexActive, float deltaTime) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            
            float dx = mousePos.x - particles[i].position.x;
            float dy = mousePos.y - particles[i].position.y;
            float distSquared = dx * dx + dy * dy;
            
            if (distSquared > 0) {
                // pre calculate inverse distance for normalization
                float invDist = 1.0f / sqrt(distSquared);
                Vector2 dirToMouse = {dx * invDist, dy * invDist};
                
                if (isVortexActive && distSquared < SQUARED_MOUSE_INFLUENCE) {
                    particles[i].velocity.x += dirToMouse.x * 5.0f * deltaTime + 
                                            (-dirToMouse.y) * VORTEX_FORCE * deltaTime;
                    particles[i].velocity.y += dirToMouse.y * 5.0f * deltaTime + 
                                            dirToMouse.x * VORTEX_FORCE * deltaTime;
                } else if (distSquared < SQUARED_MOUSE_INFLUENCE) {
                    particles[i].velocity.x += dirToMouse.x * 0.5f * deltaTime;
                    particles[i].velocity.y += dirToMouse.y * 0.5f * deltaTime;
                }
            }
            
            float speed = isVortexActive ? VORTEX_SPEED : NORMAL_SPEED;
            particles[i].position.x += particles[i].velocity.x * deltaTime * speed;
            particles[i].position.y += particles[i].velocity.y * deltaTime * speed;
            
            particles[i].velocity.x *= 0.98f;
            particles[i].velocity.y *= 0.98f;
            
            particles[i].life -= deltaTime;
            
            // Check bounds and life in one if statement
            if (particles[i].life <= 0 ||
                particles[i].position.y > GetScreenHeight() ||
                particles[i].position.x < 0 ||
                particles[i].position.x > GetScreenWidth()) {
                initParticle(i);
                continue;
            }
            DrawPixel((int)particles[i].position.x, 
                     (int)particles[i].position.y, 
                     particles[i].color);
        }
    }
    
    DrawFPS(10, 10);
    EndDrawing();
}