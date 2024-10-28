#include <stdio.h>
#include "raylib.h"

#define UP 265
#define DOWN 264

const int PLAYER_SPEED = 300;

float deltaTime;

struct Player
{
    int posX;
    int posY;
    int width;
    int height;
};


void move_player(struct Player *pPlayer);
void draw_player(struct Player *pPlayer);

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raycaster");
    SetTargetFPS(60);


    struct Player player1;
    player1.posX = 20;
    player1.posY = 250;
    player1.width = 10;
    player1.height = 100;

    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        deltaTime = GetFrameTime();

        move_player(&player1);
        BeginDrawing();
        ClearBackground(BLACK);
        draw_player(&player1);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void move_player(struct Player *pPlayer) {
    short dir = IsKeyDown(DOWN) - IsKeyDown(UP);
    pPlayer->posY += dir * PLAYER_SPEED * deltaTime;
}

void draw_player(struct Player *pPlayer) {
    struct Player player = *pPlayer;
    DrawRectangle(player.posX,player.posY,player.width,player.height,WHITE);
}
