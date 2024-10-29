#include <stdio.h>
#include "raylib.h"

#define UP_ARROW 265
#define DOWN_ARROW 264
#define W 87
#define S 83

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BORDERS = 20;
const int PLAYER_SPEED = 300;
const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 100;

int virtual_width = SCREEN_WIDTH-2*SCREEN_BORDERS;

float deltaTime;

struct Body
{
    float posX;
    float posY;
    int width;
    int height;    
};

struct Player
{
    struct Body body;
    int up_key;
    int down_key;
};

float clamp(float a,float min, float max) {
    a = a < min ? min : a;
    a = a > max ? max : a;
    return a;
}

void define_body(struct Body *pBody,int posX,int posY,int width,int height) {
    pBody->posX = (float)posX;
    pBody->posY = (float)posY;
    pBody->width = width;
    pBody->height = height;
}

void define_player(struct Player *pPlayer,int init_x,int up_key,int down_key) {
    define_body(&pPlayer->body,init_x,250,PLAYER_WIDTH,PLAYER_HEIGHT);
    pPlayer->up_key = up_key;
    pPlayer->down_key = down_key;
}

void init_players(struct Player *pPlayer1,struct Player *pPlayer2) {  
    define_player(pPlayer1,0,W,S);
    define_player(pPlayer2,virtual_width-PLAYER_WIDTH,UP_ARROW,DOWN_ARROW);
}

void move_player(struct Player *pPlayer) {
    short dir = IsKeyDown(pPlayer->down_key) - IsKeyDown(pPlayer->up_key);

    float y = pPlayer->body.posY;
    y += dir * PLAYER_SPEED * deltaTime;
    pPlayer->body.posY = clamp(y,0,SCREEN_HEIGHT-PLAYER_HEIGHT);
}

void draw_body(struct Body *pBody) {
    int x = (int)pBody->posX + SCREEN_BORDERS;
    int y = (int)pBody->posY; 
    DrawRectangle(x,y,pBody->width,pBody->height,WHITE);
}

void draw(struct Player *pPlayer1,struct Player *pPlayer2) {
    BeginDrawing();

    ClearBackground(BLACK);  

    draw_body(&pPlayer1->body);
    draw_body(&pPlayer2->body);

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