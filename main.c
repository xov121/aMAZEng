#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define TEXTURE_SIZE 64

/* Define the world map with walls (1), open spaces (0), and a finish line (2)
 */
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
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

/**
 * struct SDL_Instance - Holds the SDL window and renderer
 * @window: Pointer to SDL_Window
 * @renderer: Pointer to SDL_Renderer
 */
struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
};

SDL_Texture *floorTexture = NULL;
SDL_Texture *skyTexture = NULL;
SDL_Texture *wallTexture = NULL; /* Add wall texture */
TTF_Font *font = NULL;

/**
 * init_instance - Initialize the SDL instance (window and renderer)
 * @instance: Pointer to SDL_Instance
 *
 * Return: 0 on success, 1 on failure
 */
int init_instance(struct SDL_Instance *instance)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return (1);
	}

	/* Create a window */
	instance->window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
	                                    SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/* Create a renderer */
	instance->renderer =
	    SDL_CreateRenderer(instance->window, -1,
	                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	return (0);
}

/**
 * load_floor_texture - Load the floor texture from the image file
 * @renderer: Pointer to SDL_Renderer
 *
 * Return: 0 on success, 1 on failure
 */
int load_floor_texture(SDL_Renderer *renderer)
{
	SDL_Surface *surface = IMG_Load("/home/xov121/maze/images/blackfloor.jpg");
	if (!surface)
	{
		fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
		return (1);
	}
	floorTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!floorTexture)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return (1);
	}

	printf("Floor texture loaded successfully.\n");
	return (0);
}

/**
 * load_sky_texture - Load the sky texture from the image file
 * @renderer: Pointer to SDL_Renderer
 *
 * Return: 0 on success, 1 on failure
 */
int load_sky_texture(SDL_Renderer *renderer)
{
	SDL_Surface *surface =
	    IMG_Load("/home/xov121/maze/images/sky.jpeg"); /* Change the path to your
	                                                      sky texture file */
	if (!surface)
	{
		fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
		return (1);
	}
	skyTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!skyTexture)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return (1);
	}

	printf("Sky texture loaded successfully.\n");
	return (0);
}

/**
 * load_wall_texture - Load the wall texture from the image file
 * @renderer: Pointer to SDL_Renderer
 *
 * Return: 0 on success, 1 on failure
 */
int load_wall_texture(SDL_Renderer *renderer)
{
	SDL_Surface *surface =
	    IMG_Load("/home/xov121/maze/images/brick.bmp"); /* Change the path to your
	                                                       wall texture file */
	if (!surface)
	{
		fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
		return (1);
	}
	wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!wallTexture)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return (1);
	}

	printf("Wall texture loaded successfully.\n");
	return (0);
}

/**
 * render_floor - Render the floor texture
 * @renderer: Pointer to SDL_Renderer
 * @posX: Player's X position
 * @posY: Player's Y position
 * @dirX: Direction vector X
 * @dirY: Direction vector Y
 * @planeX: Camera plane X
 * @planeY: Camera plane Y
 * @screen_width: Screen width
 * @screen_height: Screen height
 */
void render_floor(SDL_Renderer *renderer, double posX, double posY, double dirX,
                  double dirY, double planeX, double planeY, int screen_width,
                  int screen_height)
{
	int x, y, p, cellX, cellY, tx, ty;
	double rayDirX0, rayDirY0, rayDirX1, rayDirY1, posZ, rowDistance;
	double floorStepX, floorStepY, floorX, floorY;
	SDL_Rect srcRect, dstRect;

	for (y = screen_height / 2; y < screen_height; y++)
	{
		rayDirX0 = dirX - planeX;
		rayDirY0 = dirY - planeY;
		rayDirX1 = dirX + planeX;
		rayDirY1 = dirY + planeY;

		p = y - screen_height / 2;
		posZ = 0.5 * screen_height;

		rowDistance = posZ / p;

		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screen_width;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screen_width;

		floorX = posX + rowDistance * rayDirX0;
		floorY = posY + rowDistance * rayDirY0;

		for (x = 0; x < screen_width; x++)
		{
			cellX = (int)(floorX);
			cellY = (int)(floorY);

			tx = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
			ty = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			srcRect = (SDL_Rect){tx, ty, 1, 1};
			dstRect = (SDL_Rect){x, y, 1, 1};

			SDL_RenderCopy(renderer, floorTexture, &srcRect, &dstRect);
		}
	}
}

/**
 * render_sky - Render the sky texture
 * @renderer: Pointer to SDL_Renderer
 * @screen_width: Screen width
 * @screen_height: Screen height
 */
void render_sky(SDL_Renderer *renderer, int screen_width, int screen_height)
{
	SDL_Rect dstRect = {0, 0, screen_width, screen_height / 2};

	SDL_RenderCopy(renderer, skyTexture, NULL, &dstRect);
}

/**
 * render_walls - Render the walls based on player's position, direction, and
 * camera plane
 * @instance: Pointer to SDL_Instance
 * @posX: Player's X position
 * @posY: Player's Y position
 * @dirX: Direction vector X
 * @dirY: Direction vector Y
 * @planeX: Camera plane X
 * @planeY: Camera plane Y
 */
void render_walls(struct SDL_Instance *instance, double posX, double posY,
                  double dirX, double dirY, double planeX, double planeY)
{
	int w = 800, h = 600;
	int x, mapX, mapY, stepX, stepY, hit, side, lineHeight, drawStart, drawEnd;
	double cameraX, rayDirX, rayDirY, sideDistX, sideDistY, deltaDistX, deltaDistY,
	    perpWallDist;
	double oldDirX, oldPlaneX;
	SDL_Rect srcRect, dstRect;

	for (x = 0; x < w; x++)
	{
		cameraX = 2 * x / (double)w - 1;
		rayDirX = dirX + planeX * cameraX;
		rayDirY = dirY + planeY * cameraX;

		mapX = (int)posX;
		mapY = (int)posY;

		deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
		hit = 0;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - posY) * deltaDistY;
		}

		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (world_map[mapX][mapY] > 0)
				hit = 1;
		}

		if (side == 0)
			perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

		lineHeight = (int)(h / perpWallDist);

		drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)
			drawEnd = h - 1;

		if (world_map[mapX][mapY] == 1)
		{
			int texX;
			if (side == 0)
				texX = (int)((posY + perpWallDist * rayDirY) * TEXTURE_SIZE) % TEXTURE_SIZE;
			else
				texX = (int)((posX + perpWallDist * rayDirX) * TEXTURE_SIZE) % TEXTURE_SIZE;

			if (side == 0 && rayDirX > 0)
				texX = TEXTURE_SIZE - texX - 1;
			if (side == 1 && rayDirY < 0)
				texX = TEXTURE_SIZE - texX - 1;

			for (int y = drawStart; y < drawEnd; y++)
			{
				int d = y * 256 - h * 128 + lineHeight * 128;
				int texY = ((d * TEXTURE_SIZE) / lineHeight) / 256;

				srcRect = (SDL_Rect){texX, texY, 1, 1};
				dstRect = (SDL_Rect){x, y, 1, 1};
				SDL_RenderCopy(instance->renderer, wallTexture, &srcRect, &dstRect);
			}
		}
		else if (world_map[mapX][mapY] == 2)
		{
			SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);
		}
	}
}

/**
 * render_frame_counter - Render the frame counter and player coordinates
 * @renderer: Pointer to SDL_Renderer
 * @font: Pointer to TTF_Font
 * @frame_count: Frame count
 * @averageFPS: Average FPS
 * @posX: Player's X position
 * @posY: Player's Y position
 */
void render_frame_counter(SDL_Renderer *renderer, TTF_Font *font,
                          int frame_count, double averageFPS, double posX,
                          double posY)
{
	char text[128];
	SDL_Color color = {255, 255, 255, 255}; /* White color */
	SDL_Surface *surface;
	SDL_Texture *texture;
	int texW = 0, texH = 0;
	SDL_Rect dstrect;

	sprintf(text, "Frame: %d | Avg FPS: %.2f | X: %.2f | Y: %.2f", frame_count,
	        averageFPS, posX, posY);

	surface = TTF_RenderText_Solid(font, text, color);
	if (!surface)
	{
		fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return;
	}

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = (SDL_Rect){10, 10, texW, texH};

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
}

/**
 * reset_game - Reset game to initial position and direction
 * @posX: Pointer to player's X position
 * @posY: Pointer to player's Y position
 * @dirX: Pointer to direction vector X
 * @dirY: Pointer to direction vector Y
 * @planeX: Pointer to camera plane X
 * @planeY: Pointer to camera plane Y
 */
void reset_game(double *posX, double *posY, double *dirX, double *dirY,
                double *planeX, double *planeY)
{
	do
	{
		*posX = rand() % MAP_WIDTH;
		*posY = rand() % MAP_HEIGHT;
	} while (world_map[(int)*posX][(int)*posY] != 0);

	*dirX = -1;
	*dirY = 0;
	*planeX = 0;
	*planeY = 0.66;
}

/**
 * is_solvable - Check if there is a path from the starting position to the
 * finish line using BFS
 * @startX: Starting X position
 * @startY: Starting Y position
 *
 * Return: 1 if solvable, 0 if not
 */
int is_solvable(int startX, int startY)
{
	int visited[MAP_WIDTH][MAP_HEIGHT] = {0};
	int queue[MAP_WIDTH * MAP_HEIGHT][2];
	int front = 0, rear = 0;
	int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int x, y, nx, ny, i;

	queue[rear][0] = startX;
	queue[rear][1] = startY;
	rear++;
	visited[startX][startY] = 1;

	while (front < rear)
	{
		x = queue[front][0];
		y = queue[front][1];
		front++;

		for (i = 0; i < 4; i++)
		{
			nx = x + directions[i][0];
			ny = y + directions[i][1];

			if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT &&
			    !visited[nx][ny] && world_map[nx][ny] != 1)
			{
				if (world_map[nx][ny] == 2)
				{
					return (1); /* Finish line found */
				}

				visited[nx][ny] = 1;
				queue[rear][0] = nx;
				queue[rear][1] = ny;
				rear++;
			}
		}
	}

	return (0); /* No path to the finish line */
}

/**
 * main - Entry point of the program
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	struct SDL_Instance instance;
	SDL_Event event;
	int running = 1;
	int mouseX, mouseY;
	double posX = 12, posY = 12;      /* Player position */
	double dirX = -1, dirY = 0;       /* Initial direction vector */
	double planeX = 0, planeY = 0.66; /* 2D raycaster version of camera plane */
	double rotSpeed;                  /* Rotation speed */
	double moveSpeed;                 /* Movement speed */
	double sensitivity = 0.001;       /* Mouse sensitivity */
	double oldPlaneX, oldDirX;
	Uint32 lastTime = SDL_GetTicks();
	Uint32 currentTime;
	double deltaTime;
	Uint32 startTime = SDL_GetTicks(); /* Add this line for startTime */
	Uint32 elapsedTime;                /* Add this line to declare elapsedTime */
	int moveForward = 0;
	int moveBackward = 0;
	int strafeLeft = 0;
	int strafeRight = 0;
	int frame_count = 0;
	double averageFPS = 0.0;
	Uint32 fpsUpdateTime = SDL_GetTicks();

	if (init_instance(&instance) != 0)
	{
		return (1);
	}

	if (load_floor_texture(instance.renderer) != 0)
	{
		return (1);
	}

	if (load_sky_texture(instance.renderer) != 0)
	{
		return (1);
	}

	if (load_wall_texture(instance.renderer) != 0)
	{
		return (1);
	}

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
		return (1);
	}

	font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
	                    24); /* Change the path to your font file */
	if (!font)
	{
		fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
		return (1);
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);
	srand(time(NULL));

	while (running)
	{
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - lastTime) / 1000.0; /* Time in seconds */
		lastTime = currentTime;

		moveSpeed = 2.0 * deltaTime; /* Adjusted movement speed per second */
		rotSpeed = 2.0 * deltaTime;  /* Adjusted rotation speed per second */

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				/* Close the window with ESC key */
				{
					running = 0;
				}
				if (event.key.keysym.sym == SDLK_w)
				{
					moveForward = 1;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					moveBackward = 1;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					strafeLeft = 1;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					strafeRight = 1;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_w)
				{
					moveForward = 0;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					moveBackward = 0;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					strafeLeft = 0;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					strafeRight = 0;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				SDL_GetRelativeMouseState(&mouseX, &mouseY);
				oldDirX = dirX;
				dirX =
				    dirX * cos(-mouseX * sensitivity) - dirY * sin(-mouseX * sensitivity);
				dirY = oldDirX * sin(-mouseX * sensitivity) +
				       dirY * cos(-mouseX * sensitivity);
				oldPlaneX = planeX;
				planeX = planeX * cos(-mouseX * sensitivity) -
				         planeY * sin(-mouseX * sensitivity);
				planeY = oldPlaneX * sin(-mouseX * sensitivity) +
				         planeY * cos(-mouseX * sensitivity);
			}

			/* Check if player reached the finish line */
			if (world_map[(int)posX][(int)posY] == 2)
			{
				printf(
				    "Congratulations! You've reached the "
				    "finish line!\n");
				SDL_Delay(1000);
				do
				{
					reset_game(&posX, &posY, &dirX, &dirY, &planeX, &planeY);
				} while (!is_solvable((int)posX, (int)posY));
			}
		}

		/* Move forward */
		if (moveForward)
		{
			if (world_map[(int)(posX + dirX * moveSpeed)][(int)posY] == 0 ||
			    world_map[(int)(posX + dirX * moveSpeed)][(int)posY] == 2)
				posX += dirX * moveSpeed;
			if (world_map[(int)posX][(int)(posY + dirY * moveSpeed)] == 0 ||
			    world_map[(int)posX][(int)(posY + dirY * moveSpeed)] == 2)
				posY += dirY * moveSpeed;
		}
		/* Move backward */
		if (moveBackward)
		{
			if (world_map[(int)(posX - dirX * moveSpeed)][(int)posY] == 0 ||
			    world_map[(int)(posX - dirX * moveSpeed)][(int)posY] == 2)
				posX -= dirX * moveSpeed;
			if (world_map[(int)posX][(int)(posY - dirY * moveSpeed)] == 0 ||
			    world_map[(int)posX][(int)(posY - dirY * moveSpeed)] == 2)
				posY -= dirY * moveSpeed;
		}
		/* Strafe left */
		if (strafeLeft)
		{
			if (world_map[(int)(posX - planeX * moveSpeed)][(int)posY] == 0 ||
			    world_map[(int)(posX - planeX * moveSpeed)][(int)posY] == 2)
				posX -= planeX * moveSpeed;
			if (world_map[(int)posX][(int)(posY - planeY * moveSpeed)] == 0 ||
			    world_map[(int)posX][(int)(posY - planeY * moveSpeed)] == 2)
				posY -= planeY * moveSpeed;
		}
		/* Strafe right */
		if (strafeRight)
		{
			if (world_map[(int)(posX + planeX * moveSpeed)][(int)posY] == 0 ||
			    world_map[(int)(posX + planeX * moveSpeed)][(int)posY] == 2)
				posX += planeX * moveSpeed;
			if (world_map[(int)posX][(int)(posY + planeY * moveSpeed)] == 0 ||
			    world_map[(int)posX][(int)(posY + planeY * moveSpeed)] == 2)
				posY += planeY * moveSpeed;
		}

		/* Clear the renderer */
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
		SDL_RenderClear(instance.renderer);

		/* Render the sky */
		render_sky(instance.renderer, 800, 600);

		/* Render the floor */
		render_floor(instance.renderer, posX, posY, dirX, dirY, planeX, planeY, 800,
		             600);

		/* Render the walls */
		render_walls(&instance, posX, posY, dirX, dirY, planeX, planeY);

		/* Calculate and display average FPS */
		elapsedTime = SDL_GetTicks() - startTime;
		/* Update average FPS every second */
		if (currentTime - fpsUpdateTime >= 1000)
		{
			averageFPS = frame_count / ((currentTime - startTime) / 1000.0);
			fpsUpdateTime = currentTime;
		}

		/* Render the frame counter and player coordinates */
		render_frame_counter(instance.renderer, font, frame_count, averageFPS, posX,
		                     posY);

		SDL_RenderPresent(instance.renderer);
		SDL_Delay(1); /* Approx. 60 frames per second */
		frame_count++;
	}

	/* Clean up */
	SDL_DestroyTexture(floorTexture);
	SDL_DestroyTexture(skyTexture);
	SDL_DestroyTexture(wallTexture); /* Clean up wall texture */
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();

	return (0);
}
