#include <stdio.h>
#include <conio.h>
#include "raylib.h"

#define UP_ARROW 265
#define DOWN_ARROW 264
#define W 87
#define S 83

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BORDERS = 100;

const int PLAYER_SPEED = 200;
const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 100;

const int BALL_SPEED = 300;
const int BALL_SIZE = 20;

const int FRICTION = 0.95;

float deltaTime;
float accelerating_speed;
int gameLost;
int score;

typedef struct
{
    float posX;
    float posY;
    float velX;
    float velY;
    int width;
    int height;
    Color color;    
} Body;

typedef struct
{
    Body body;
    int up_key;
    int down_key;
} Player;

float clamp(float a,float min, float max) {
    a = a < min ? min : a;
    a = a > max ? max : a;
    return a;
}

int getCollisionMag(int pos1, int size1,int pos2,int size2) {
    int col1 = (pos1 + size1) - pos2;
    int col2 = pos1 - (pos2 + size2);
    int collision_on_axis = col1 * col2;

    return -collision_on_axis;
}

void define_body(Body *pBody,int posX,int posY,int width,int height) {
    pBody->posX = (float)posX;
    pBody->posY = (float)posY;
    pBody->width = width;
    pBody->height = height;
    pBody->color = WHITE;
}

void define_player(Player *pPlayer,int init_x,int up_key,int down_key) {
    define_body(&pPlayer->body,init_x,250,PLAYER_WIDTH,PLAYER_HEIGHT);
    pPlayer->up_key = up_key;
    pPlayer->down_key = down_key;
}

void move_ball(Body *pBall,Player *pPlayer1,Player *pPlayer2) {
    float x = pBall->posX;
    float y = pBall->posY;
    x += pBall->velX * (BALL_SPEED+accelerating_speed) * deltaTime;
    y += pBall->velY * (BALL_SPEED+accelerating_speed) * deltaTime;

    float clamped_x = clamp(x,0,SCREEN_WIDTH-BALL_SIZE);
    float clamped_y = clamp(y,0,SCREEN_HEIGHT-BALL_SIZE);
    
    gameLost = clamped_x != x;
    pBall->velY = clamped_y != y ? -pBall->velY : pBall->velY;

    int xCol1 = getCollisionMag( (int) clamped_x , BALL_SIZE  ,  (int) pPlayer1->body.posX , PLAYER_WIDTH );
    int xCol2 = getCollisionMag( (int) clamped_x , BALL_SIZE  ,  (int) pPlayer2->body.posX , PLAYER_WIDTH );
    int yCol1 = getCollisionMag( (int) clamped_y , BALL_SIZE  ,  (int) pPlayer1->body.posY , PLAYER_HEIGHT);
    int yCol2 = getCollisionMag( (int) clamped_y , BALL_SIZE  ,  (int) pPlayer2->body.posY , PLAYER_HEIGHT);

    if((xCol1 >= 0 && yCol1 >= 0)) {
        if(xCol1 <= yCol1) {
            pBall->velX *= -1;
            pBall->velY += pPlayer1->body.velY * FRICTION;
            score +=1;
        } 

        if(xCol1 >= yCol1) {
            pBall->velY += pPlayer1->body.velY;
            if ((pBall->velY * pPlayer1->body.velY) <= 0) {pBall->velY *= -1;}
        }
    }

    if((xCol2 >= 0 && yCol2 >= 0)) {
        if(xCol2 <= yCol2) {
            pBall->velY += pPlayer2->body.velY * FRICTION;
            pBall->velX *= -1;
            score +=1;
        } 
        if(xCol2 >= yCol2) {
            pBall->velY += pPlayer2->body.velY;
            if ((pBall->velY * pPlayer2->body.velY) <= 0) {pBall->velY *= -1;}
        }
    }

    pBall->posX = clamped_x;
    pBall->posY = clamped_y;
}

void move_player(Player *pPlayer) {
    short dir = IsKeyDown(pPlayer->down_key) - IsKeyDown(pPlayer->up_key);

    pPlayer->body.velY = dir * (PLAYER_SPEED+accelerating_speed) * deltaTime;

    float y = pPlayer->body.posY;

    y += pPlayer->body.velY;
    pPlayer->body.posY = clamp(y,0,SCREEN_HEIGHT-PLAYER_HEIGHT);
}

void draw_body(Body *pBody) {
    DrawRectangle(pBody->posX,pBody->posY,pBody->width,pBody->height,pBody->color);
}

void draw(Player *pPlayer1,Player *pPlayer2,Body *pBall,int score) {
    BeginDrawing();

    ClearBackground(BLACK);  

    DrawText(TextFormat("%d",score), 480, 20, 50, GRAY);
    draw_body(&pPlayer1->body);
    draw_body(&pPlayer2->body);
    draw_body(pBall);

    EndDrawing();
}

void process_game(Player *pPlayer1,Player *pPlayer2,Body *pBall) {
    gameLost = 0;
    score = 0;
    printf("hello");
    while (!WindowShouldClose() && !gameLost) {  // Detect window close button or ESC key
        
        deltaTime = GetFrameTime();
        // int k = GetKeyPressed();
        // if(k != 0) {
        //     printf("key:\t%d,\t%c\n",k,k);
        // }

        move_player(pPlayer1);
        move_player(pPlayer2);
        move_ball(pBall,pPlayer1,pPlayer2);
        draw(pPlayer1,pPlayer2,pBall,score);

        accelerating_speed += deltaTime * 5;
    }
}

void init_game() {
    while (!WindowShouldClose()) {
        Player player1, player2;
        define_player(&player1, SCREEN_BORDERS, W, S);
        define_player(&player2, SCREEN_WIDTH - PLAYER_WIDTH - SCREEN_BORDERS, UP_ARROW, DOWN_ARROW);

        Body ball;
        define_body(&ball, 490, 290, BALL_SIZE, BALL_SIZE);
        ball.velX = 1;
        ball.velY = 0.5;

        int k = GetKeyPressed();
        if (k != 0) {
            process_game(&player1, &player2, &ball);
        }
            
        BeginDrawing();
        ClearBackground(BLACK);
        draw_body(&player1.body);
        draw_body(&player2.body);
        draw_body(&ball);
        DrawText(TextFormat("Press any key to start the round"), 300, 280, 20, RED);
        EndDrawing();
        
    }
}

int main(int argc, char** argv) {

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "PONG");
    SetTargetFPS(200);
    init_game();

    CloseWindow();
    return 0;
}