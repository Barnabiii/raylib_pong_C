#include "raylib.h"

struct Player
{
    int posX;
    int posY;
    int width;
    int height;
};


int draw();

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raycaster");
    SetTargetFPS(60);


    struct Player player1;
    player1.posX = 100;
    player1.posY = 100;
    player1.width = 10;
    player1.height = 50;

    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        draw(&player1);
    }

    CloseWindow();
    return 0;
}

int draw(struct Player *pPlayer) {
    struct Player player = *pPlayer;

    BeginDrawing();

    DrawRectangle(player.posX,player.posY,player.width,player.height,WHITE);

    EndDrawing();
    return 0;
}
