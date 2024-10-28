#include <stdio.h>
#include "raylib.h"

#define UP_ARROW 265
#define DOWN_ARROW 264
#define W 87
#define S 83

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 300;
const int PLAYER_WIDTH = 10;
const int PLAYER_HEIGHT = 100;
const int x_offset = 20;

float deltaTime;

struct Player
{
    float posX;
    float posY;
    int width;
    int height;
    int up_key;
    int down_key;
};

void init_player(struct Player *pPlayer,int init_posX,int up_key,int down_key) {
    pPlayer->posX = init_posX;
    pPlayer->posY = 250;
    pPlayer->width = PLAYER_WIDTH;
    pPlayer->height = PLAYER_HEIGHT;
    pPlayer->up_key = up_key;
    pPlayer->down_key = down_key;
}
void init_players(struct Player *pPlayer1,struct Player *pPlayer2) {
    int player2_startX = SCREEN_WIDTH - (x_offset+PLAYER_WIDTH);   
    init_player(pPlayer1,x_offset,W,S);
    init_player(pPlayer2,player2_startX,UP_ARROW,DOWN_ARROW);
}

void move_player(struct Player *pPlayer) {
    short dir = IsKeyDown(pPlayer->down_key) - IsKeyDown(pPlayer->up_key);
    pPlayer->posY += dir * PLAYER_SPEED * deltaTime;
}

void draw_player(struct Player *pPlayer) {
    struct Player player = *pPlayer;
    DrawRectangle((int)player.posX,(int)player.posY,player.width,player.height,WHITE);
}
void draw(struct Player *pPlayer1,struct Player *pPlayer2) {
    BeginDrawing();

    ClearBackground(BLACK);  

    draw_player(pPlayer1);
    draw_player(pPlayer2);

    EndDrawing();
}

void process_game(struct Player *pPlayer1,struct Player *pPlayer2) {
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        deltaTime = GetFrameTime();
        // int k = GetKeyPressed();
        // if(k != 0) {
        //     printf("key:\t%d,\t%c\n",k,k);
        // }

        move_player(pPlayer1);
        move_player(pPlayer2);
        draw(pPlayer1,pPlayer2);
    }
}

int main(int argc, char** argv) {

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "PONG");
    SetTargetFPS(200);

    struct Player player1,player2;
    init_players(&player1,&player2);
    process_game(&player1,&player2);

    CloseWindow();
    return 0;
}