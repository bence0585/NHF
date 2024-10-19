// Currently making use of the SDL3.1.0 Preview release

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>

#define MAX_OBSTACLES 100

typedef struct
{
    int x, y;
    int size;
} Obstacle;

void draw_circle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderPoint(renderer, centerX + x, centerY + y);
        SDL_RenderPoint(renderer, centerX + y, centerY + x);
        SDL_RenderPoint(renderer, centerX - y, centerY + x);
        SDL_RenderPoint(renderer, centerX - x, centerY + y);
        SDL_RenderPoint(renderer, centerX - x, centerY - y);
        SDL_RenderPoint(renderer, centerX - y, centerY - x);
        SDL_RenderPoint(renderer, centerX + y, centerY - x);
        SDL_RenderPoint(renderer, centerX + x, centerY - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void draw_square(SDL_Renderer *renderer, int x, int y, int size)
{
    SDL_FRect rect = {x - size / 2, y - size / 2, size, size};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

int check_collision(int circleX, int circleY, int radius, Obstacle obstacle, float *adjustX, float *adjustY)
{
    int closestX = (circleX < obstacle.x - obstacle.size / 2) ? (obstacle.x - obstacle.size / 2) : (circleX > obstacle.x + obstacle.size / 2) ? (obstacle.x + obstacle.size / 2)
                                                                                                                                              : circleX;
    int closestY = (circleY < obstacle.y - obstacle.size / 2) ? (obstacle.y - obstacle.size / 2) : (circleY > obstacle.y + obstacle.size / 2) ? (obstacle.y + obstacle.size / 2)
                                                                                                                                              : circleY;

    int distanceX = circleX - closestX;
    int distanceY = circleY - closestY;

    int distanceSquared = distanceX * distanceX + distanceY * distanceY;
    int radiusSquared = radius * radius;

    if (distanceSquared < radiusSquared)
    {
        float distance = sqrtf((float)distanceSquared);
        float overlap = radius - distance;

        *adjustX = (distanceX / distance) * overlap;
        *adjustY = (distanceY / distance) * overlap;

        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL Circle", 800, 600, NULL);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return -2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, "opengl");
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return -3;
    }

    SDL_Log("SDL3 initialized");

    int quit = 0;
    SDL_Event event;

    int centerX = 400;
    int centerY = 300;
    int radius = 100;
    float gravity = 2.0f;
    int gravityEnabled = 1;
    float velocityX = 0;
    float velocityY = 0;
    float accelerationX = 0;
    float accelerationY = 0;
    float damping = 0.5f; // 50% of energy remains after bounce
    float force = 0.5f;   // Force applied by arrow keys

    Obstacle obstacles[MAX_OBSTACLES];
    int obstacleCount = 0;
    int obstacleSize = 50;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    SDL_Log("SDL3 ESC key quit");
                    quit = 1;
                }
                else if (event.key.key == SDLK_UP)
                {
                    gravityEnabled = 0;
                    accelerationY = -force;
                }
                else if (event.key.key == SDLK_DOWN)
                {
                    accelerationY = force;
                }
                else if (event.key.key == SDLK_LEFT)
                {
                    accelerationX = -force;
                }
                else if (event.key.key == SDLK_RIGHT)
                {
                    accelerationX = force;
                }
                break;
            case SDL_EVENT_KEY_UP:
                if (event.key.key == SDLK_UP)
                {
                    gravityEnabled = 1;
                    accelerationY = 0;
                }
                else if (event.key.key == SDLK_DOWN)
                {
                    accelerationY = 0;
                }
                else if (event.key.key == SDLK_LEFT)
                {
                    accelerationX = 0;
                }
                else if (event.key.key == SDLK_RIGHT)
                {
                    accelerationX = 0;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT && obstacleCount < MAX_OBSTACLES)
                {
                    obstacles[obstacleCount].x = event.motion.x;
                    obstacles[obstacleCount].y = event.motion.y;
                    obstacles[obstacleCount].size = obstacleSize;
                    obstacleCount++;
                    SDL_Log("Obstacle added at (%d, %d). Total obstacles: %d", event.motion.x, event.motion.y, obstacleCount);
                }
                break;
            }
        }

        if (gravityEnabled)
        {
            velocityY += gravity;
        }

        velocityX += accelerationX;
        velocityY += accelerationY;

        centerX += (int)velocityX;
        centerY += (int)velocityY;

        // Check for collisions with obstacles
        for (int i = 0; i < obstacleCount; i++)
        {
            float adjustX = 0, adjustY = 0;
            if (check_collision(centerX, centerY, radius, obstacles[i], &adjustX, &adjustY))
            {
                // Adjust position to prevent sticking
                centerX += (int)adjustX;
                centerY += (int)adjustY;

                // Handle collision (e.g., stop movement or bounce)
                velocityX = -velocityX * damping;
                velocityY = -velocityY * damping;
                break;
            }
        }

        if (centerY + radius > 600)
        {
            centerY = 600 - radius;
            velocityY = -velocityY * damping; // Invert velocity and apply damping
        }

        if (centerX - radius < 0)
        {
            centerX = radius;
            velocityX = -velocityX * damping; // Invert velocity and apply damping
        }
        else if (centerX + radius > 800)
        {
            centerX = 800 - radius;
            velocityX = -velocityX * damping; // Invert velocity and apply damping
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_circle(renderer, centerX, centerY, radius); // Draw circle at updated position

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (int i = 0; i < obstacleCount; i++)
        {
            draw_square(renderer, obstacles[i].x, obstacles[i].y, obstacles[i].size);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approximately 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}