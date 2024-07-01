#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

/* Define the world map with walls (1), open spaces (0), and a finish line (2) */
int world_map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDL_Instance;

/* Initialize the SDL instance (window and renderer) */
int init_instance(SDL_Instance *instance) {
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    /* Create a window */
    instance->window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (instance->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Create a renderer */
    instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (instance->renderer == NULL) {
        SDL_DestroyWindow(instance->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

/* Render the walls based on player's position, direction, and camera plane */
void render_walls(SDL_Instance *instance, double posX, double posY, double dirX, double dirY, double planeX, double planeY) {
    int w = 800;
    int h = 600;
    int x;
    int lineHeight, drawEnd, drawStart;

    for (x = 0; x < w; x++) {
        /* calculate ray position and direction */
        double cameraX = 2 * x / (double)w - 1;  /* x-coordinate in camera space */
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        /* which box of the map we're in */
        int mapX = (int)posX;
        int mapY = (int)posY;

        /* length of ray from current position to next x or y-side */
        double sideDistX;
        double sideDistY;

        /* length of ray from one x or y-side to next x or y-side */
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double perpWallDist;

        /* what direction to step in x or y-direction (either +1 or -1) */
        int stepX;
        int stepY;

        int hit = 0;  /* was there a wall hit? */
        int side;  /* was a NS or a EW wall hit? */

        /* calculate step and initial sideDist */
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        /* perform DDA */
        while (hit == 0) {
            /* jump to next map square, OR in x-direction, OR in y-direction */
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            /* Check if ray has hit a wall or finish line */
            if (world_map[mapX][mapY] > 0) hit = 1;
        }

        /* Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!) */
        if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        /* Calculate height of line to draw on screen */
        lineHeight = (int)(h / perpWallDist);

        /* calculate lowest and highest pixel to fill in current stripe */
        drawStart = -lineHeight / 2 + h / 2;
        if (drawStart < 0) drawStart = 0;
        drawEnd = lineHeight / 2 + h / 2;
        if (drawEnd >= h) drawEnd = h - 1;

        /* draw the sky */
        SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 255);  /* sky blue */
        SDL_RenderDrawLine(instance->renderer, x, 0, x, drawStart);

        /* draw the ground */
        SDL_SetRenderDrawColor(instance->renderer, 139, 69, 19, 255);  /* brown */
        SDL_RenderDrawLine(instance->renderer, x, drawEnd, x, h);

        /* choose wall or finish line color */
        if (world_map[mapX][mapY] == 1) {
            SDL_SetRenderDrawColor(instance->renderer, 169, 169, 169, 255);  /* gray */
            if (side == 1) {
                SDL_SetRenderDrawColor(instance->renderer, 105, 105, 105, 255);  /* dark gray */
            }
        } else if (world_map[mapX][mapY] == 2) {
            SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255);  /* red for finish line */
        }

        /* draw the pixels of the stripe as a vertical line */
        SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);
    }
}

/* Reset game to initial position and direction */
void reset_game(double *posX, double *posY, double *dirX, double *dirY, double *planeX, double *planeY) {
    *posX = 12;
    *posY = 12;
    *dirX = -1;
    *dirY = 0;
    *planeX = 0;
    *planeY = 0.66;
}

int main() {
    SDL_Instance instance;
    SDL_Event event;
    int running = 1;
    int mouseX, mouseY;

    double posX = 12, posY = 12;  /* player position */
    double dirX = -1, dirY = 0;   /* initial direction vector */
    double planeX = 0, planeY = 0.66;  /* 2D raycaster version of camera plane */
    double rotSpeed = 0.05; /* rotation speed */
    double moveSpeed = 0.1; /* movement speed */
    double sensitivity = 0.001; /* mouse sensitivity */
    double oldPlaneX, oldDirX;

    if (init_instance(&instance) != 0) {
        return 1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    /* Main game loop */
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    if(world_map[(int)(posX + dirX * moveSpeed)][(int)posY] == 0 || world_map[(int)(posX + dirX * moveSpeed)][(int)posY] == 2) posX += dirX * moveSpeed;
                    if(world_map[(int)posX][(int)(posY + dirY * moveSpeed)] == 0 || world_map[(int)posX][(int)(posY + dirY * moveSpeed)] == 2) posY += dirY * moveSpeed;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    if(world_map[(int)(posX - dirX * moveSpeed)][(int)posY] == 0 || world_map[(int)(posX - dirX * moveSpeed)][(int)posY] == 2) posX -= dirX * moveSpeed;
                    if(world_map[(int)posX][(int)(posY - dirY * moveSpeed)] == 0 || world_map[(int)posX][(int)(posY - dirY * moveSpeed)] == 2) posY -= dirY * moveSpeed;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                    oldPlaneX = planeX;
                    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    oldDirX = dirX;
                    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                    oldPlaneX = planeX;
                    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                SDL_GetRelativeMouseState(&mouseX, &mouseY);
                oldDirX = dirX;
                dirX = dirX * cos(-mouseX * sensitivity) - dirY * sin(-mouseX * sensitivity);
                dirY = oldDirX * sin(-mouseX * sensitivity) + dirY * cos(-mouseX * sensitivity);
                oldPlaneX = planeX;
                planeX = planeX * cos(-mouseX * sensitivity) - planeY * sin(-mouseX * sensitivity);
                planeY = oldPlaneX * sin(-mouseX * sensitivity) + planeY * cos(-mouseX * sensitivity);
            }

            /* Check if player reached the finish line */
            if (world_map[(int)posX][(int)posY] == 2) {
                printf("Congratulations! You've reached the finish line!\n");
                SDL_Delay(1000);
                reset_game(&posX, &posY, &dirX, &dirY, &planeX, &planeY);
            }
        }

        /* Clear the renderer */
        SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
        SDL_RenderClear(instance.renderer);

        /* Render the walls */
        render_walls(&instance, posX, posY, dirX, dirY, planeX, planeY);
        SDL_RenderPresent(instance.renderer);
        SDL_Delay(16); /* Approx. 60 frames per second */
    }

    /* Clean up */
    SDL_DestroyRenderer(instance.renderer);
    SDL_DestroyWindow(instance.window);
    SDL_Quit();

    return 0;
}
