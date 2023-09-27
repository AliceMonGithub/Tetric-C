#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

const float FALL_TIME = 1.3f;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

const int START_X_POSITION = 5;
const int START_Y_POSITION = 20;

const int GRID_SIZE_COLUMN = 10;
const int GRID_SIZE_ROW = 20;

const int GRID_OFFSET_X = 200;
const int GRID_OFFSET_Y = 700;

const int CELL_SIZE = 30;

typedef struct Vector2Int
{
	int x;
	int y;
} Vector2Int;

struct Vector2Int L_BRICK[4] = 
{
	{ 0, 0 },
	{ 1, 0 },
	{ 2, 0 },
	{ 2, 1 }
};

typedef struct Brick
{
	Vector2Int cells[4];
	Vector2Int position;
} Brick;

typedef struct BricksList
{
	Brick* value;
	int lenght;
} BricksList;

typedef struct Game
{
	BricksList bricks;
	Brick* controllableBrick;

	Brick* grid[20][10];

	float fallTimer;
} Game;

int UpdateGrid(Brick* brick, Game* game)
{
	for (int i = 0; i < sizeof(brick->cells) / sizeof(brick->cells[0]); i++)
	{
		int row = GRID_SIZE_ROW - brick->position.y - brick->cells[i].y - 1;
		int column = GRID_SIZE_COLUMN - brick->position.x + brick->cells[i].x - 1;

		game->grid[row][column] = brick;
	}
}

int TrySpawnNext(Game* game)
{
	if (game->controllableBrick->position.y == 0)
	{
		Brick instance;

		instance.position.x = START_X_POSITION;
		instance.position.y = START_Y_POSITION;

		memcpy(instance.cells, L_BRICK, sizeof(L_BRICK));

		UpdateGrid(&game->bricks.value[game->bricks.lenght - 1], game);

		Brick* ptr = (Brick*)realloc(game->bricks.value, sizeof(Brick) * (game->bricks.lenght + 1));

		if (ptr == NULL)
		{
			return;
		}

		game->bricks.value = ptr;
		game->bricks.lenght++;

		game->bricks.value[game->bricks.lenght - 1] = instance;
		game->controllableBrick = &game->bricks.value[game->bricks.lenght - 1];
	}
}

int Update(Game* game)
{
	game->fallTimer += GetFrameTime();

	if (IsKeyPressed(KEY_D))
	{
		game->controllableBrick->position.x++;
	}
	else if (IsKeyPressed(KEY_A))
	{
		game->controllableBrick->position.x--;
	}
	else if (IsKeyPressed(KEY_S))
	{
		game->controllableBrick->position.y = 0;
		game->fallTimer = 0;

		TrySpawnNext(game);
	}

	if (game->fallTimer >= FALL_TIME)
	{
		game->controllableBrick->position.y--;
		game->fallTimer = 0;

		TrySpawnNext(game);
	}
}

int RenderBrick(Brick brick)
{
	for (int i = 0; i < sizeof(brick.cells) / sizeof(brick.cells[0]); i++) {
		Vector2 cellPosition = { 0, 0 };

		cellPosition.x += GRID_OFFSET_X + (CELL_SIZE * brick.position.x) + (CELL_SIZE * brick.cells[i].x);
		cellPosition.y += GRID_OFFSET_Y - (CELL_SIZE * brick.position.y) - (CELL_SIZE * brick.cells[i].y);

		DrawRectangle(cellPosition.x, cellPosition.y, CELL_SIZE, CELL_SIZE, BLUE);
	}
}

int RenderBricks(Game* game)
{
	for (int i = 0; i < game->bricks.lenght; i++)
	{
		RenderBrick(game->bricks.value[i]);
	}
}

int Render(Game* game)
{
	RenderBricks(game);
}

int RunLoop(Game* game)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
	SetTargetFPS(60);

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		Update(game);
		Render(game);

		EndDrawing();
	}

	CloseWindow();
}

int main()
{
	Vector2Int test = { 0, 0 };

	Game game = { {(Brick*)malloc(sizeof(Brick)), 1}, NULL, 0};

	game.bricks.value[0].position.x = START_X_POSITION;
	game.bricks.value[0].position.y = START_Y_POSITION;

	memcpy(game.bricks.value[0].cells, L_BRICK, sizeof(L_BRICK));

	game.controllableBrick = &game.bricks.value[0];

	RunLoop(&game);

	return 0;
}