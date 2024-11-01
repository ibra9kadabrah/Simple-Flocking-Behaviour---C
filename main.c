#include "../raylib/src/raylib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_PARTICLES 10000
#define PARTICLE_SIZE 1.5f
#define MIN_VELOCITY -2.0f
#define MAX_VELOCITY 2.0f
#define MOUSE_INFLUENCE 200.0f
#define NORMAL_SPEED 100.0f
#define VORTEX_SPEED 400.0f
#define VORTEX_FORCE 8.0f

#define SPAWN_WIDTH 400
#define SPAWN_HEIGHT 100

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
    bool active;
} Particle;

float RandomFloat(float min, float max);
Vector2 GetDirection(Vector2 p1, Vector2 p2);
void initParticle(int index);

Particle particles[MAX_PARTICLES];

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    srand(time(NULL));

    InitWindow(screenWidth, screenHeight, "Flocking Simulation");
    SetTargetFPS(60);

    // init particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        initParticle(i);
    }

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        bool isVortexActive = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        // update each particle
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].active) {

                Vector2 dirToMouse = GetDirection(particles[i].position, mousePos);
                float distToMouse = sqrt(pow(mousePos.x - particles[i].position.x, 2) +
                                    pow(mousePos.y - particles[i].position.y, 2));

                // if mouse is clicked and particle is within influence field
                if (isVortexActive && distToMouse < MOUSE_INFLUENCE) {
                    // create vortex/spiral like shape
                    particles[i].velocity.x += dirToMouse.x * 5.0f * deltaTime + (-dirToMouse.y) * VORTEX_FORCE * deltaTime;
                    particles[i].velocity.y += dirToMouse.y * 5.0f * deltaTime + dirToMouse.x * VORTEX_FORCE * deltaTime;

                } else {
                    // normal movement
                    if (distToMouse < MOUSE_INFLUENCE) {
                        particles[i].velocity.x += dirToMouse.x * 0.5f * deltaTime;
                        particles[i].velocity.y += dirToMouse.y * 0.5f * deltaTime;
                    }
                }

                float speed = isVortexActive ? VORTEX_SPEED : NORMAL_SPEED;
                particles[i].position.x += particles[i].velocity.x * deltaTime * speed;
                particles[i].position.y += particles[i].velocity.y * deltaTime * speed;

                // slight drag effect
                particles[i].velocity.x *= 0.95f;
                particles[i].velocity.y *= 0.95f;

                particles[i].life -= deltaTime;

                // reset particle if dead or out of bound
                if (particles[i].life <= 0 ||
                    particles[i].position.y > screenHeight ||
                    particles[i].position.x < 0 ||
                    particles[i].position.x > screenWidth) {
                        initParticle(i);
                    }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].active) {
                DrawCircleV(particles[i].position, PARTICLE_SIZE, particles[i].color);
            }
        } 
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


// Function implementations

float RandomFloat(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

Vector2 GetDirection(Vector2 p1, Vector2 p2) {
    Vector2 dir = {p2.x - p1.x, p2.y - p1.y};
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0) {
        dir.x /= length;
        dir.y /= length;
    }
    return dir;
}

void initParticle(int index) {

    particles[index].position = (Vector2) {
        RandomFloat(0, GetScreenWidth()),
        RandomFloat(0, GetScreenHeight())
    };

    particles[index].velocity = (Vector2) {
        RandomFloat(MIN_VELOCITY, MAX_VELOCITY),
        RandomFloat(MIN_VELOCITY/2, MAX_VELOCITY/2) 
    };

    unsigned char grey = (unsigned char)(rand() % 40); 
    particles[index].color = (Color){
        grey, grey, grey, 255
    };

    particles[index].life = RandomFloat(1.0f, 10.0f);
    particles[index].active = true;
}