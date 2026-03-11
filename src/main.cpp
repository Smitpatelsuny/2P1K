#include "raylib.h"
#include "PlayerStuff.h"
#include "DeaStart.h"
#include <stdio.h>
#include "Objects.h"


typedef enum GameScreen { TITLE = 0, LEVSEL, GAMEPLAY} GameScreen;

//ChickenNuggetPotatoGuy21

int main(void)
{
    float Jumpspeed = 0.0;
    float Jumpspeed2 = 0.0;
    float JumpspeedMerged = 0.0;
    float rad = 20.0;
    bool P1CanJump = true; 
    bool P2CanJump = true;
    bool MergedCanJump = true; 
    bool merged = false;
    const int screenWidth = 800;
    const int screenHeight = 800;
    GameScreen currentScreen = TITLE;
    bool deathOccured = false;
    bool invincibility = false;
    float invincibilityTimer = 1.0f; // 1 second of invincibility after unmerging

    InitWindow(screenWidth, screenHeight, "All code test");

    Vector2 Ying = { 40, 764};
    Vector2 Yan = { 760, 764};
    Vector2 obj3 = {0, 0};

    Rectangle boxCollision = {0};
    Rectangle platform[] = {
    //Max Jump Height is about 2.5 player models.
    {   0, 785, 800, 15 },   // floor
    {   0,   0,  15, 800 },  // left wall
    { 785,   0,  15, 800 },  // right wall
    {   0,   0, 800, 15 },   // ceiling (optional)
    { 325, 500, 150, 300 },   // x, y, width, height, middle wall
    {   0, 700, 250, 15 },   // 1st Left platform
    { 100, 600, 600, 15 },   // 2nd Middle platform
    { 550, 700, 250, 15 },   // 1st Right platform
    {   0, 300, 250, 15 },   // Top left platfrom
    { 550, 300, 250, 15 },   // Top right platform
    };

    Rectangle startbutton = { 250, 220, 300, 40 };
    Rectangle backbutton = { 10, 10, 30, 30 };
    Rectangle levelselectbuttons[] ={
        { 150, 200, 100, 50 },
        { 270, 200, 100, 50 },
        { 390, 200, 100, 50 },
        { 510, 200, 100, 50 },
    };

    Vector2 spikes[][3]{
        {{105, 700}, {145, 700}, {125, 670}}, //spike on first left platform
        {{650, 700}, {690, 700}, {670, 670}}, //spike on first right platform
        {{193, 600}, {233, 600}, {213, 570}}, //spike on middle left platform
        {{558, 600}, {598, 600}, {578, 570}}, //spike on middle right platform
    };

    //spike parameters: first vetice = (x of platform + 15, y of platform), 
    //second vertice = (x of platform + 55, y of platform), 
    //third vertice = (x of platform + 35, y of platform - 30)

    Rectangle spikeHitboxes[] = {
        {110, 685, 30, 15}, //spike hitbox for first left platform
        {655, 685, 30, 15}, //spike hitbox for first right platform
        {198, 585, 30, 15}, //spike hitbox for middle left platform
        {563, 585, 30, 15}, //spike hitbox for middle right platform
    };

    //future spike hitbox: +5 on x (first vertice) and -15 on y from the spike's position, 
    //with a width of 30 and height of 15

    Rectangle exitDoorHitboxYin = { 10, 280, 30, 30 };
    Rectangle exitDoorHitboxYang = { 760, 280, 30, 30 };

    SetTargetFPS(60);     
    
    int platformCount = (int)(sizeof(platform) / sizeof(platform[0]));
    int spikeCount = (int)(sizeof(spikes) / sizeof(spikes[0]));
  
    while (!WindowShouldClose())    
    {
        MergeObjects(Ying, Yan, obj3, rad, merged, invincibility);

        switch(currentScreen)
        {
            case TITLE:
            {
                if(IsGestureDetected(GESTURE_TAP) && CheckCollisionPointRec(GetMousePosition(), startbutton))
                {
                    currentScreen = LEVSEL;
                }
            }break;
            case LEVSEL:
            {
                if(IsGestureDetected(GESTURE_TAP) && CheckCollisionPointRec(GetMousePosition(), backbutton))
                {
                    currentScreen = TITLE;
                }
                else if(IsGestureDetected(GESTURE_TAP) && CheckCollisionPointRec(GetMousePosition(), levelselectbuttons[0]))
                {
                    currentScreen = GAMEPLAY;
                }
            }break;
            case GAMEPLAY:
            {
                if(IsKeyPressed(KEY_P))
                {
                    currentScreen = LEVSEL;
                }
   

        

                // Save old positions BEFORE moving
                Vector2 oldP1 = Ying;
                Vector2 oldP2 = Yan;
                Vector2 oldMerged = obj3;


                //X-axis movement and collision
                if(merged){
                    MergePlayerMotor(obj3, 2);
                }
                else{
                    PlayerMotor(Ying, 2);
                    Player2Motor(Yan, 2);
                }
   
        PlayerPlatformCollisions(Ying, rad, platform, platformCount, Yan, obj3, oldP1, oldP2, oldMerged);
        
        //Y-axis movement and collision
        Jump(Jumpspeed, Jumpspeed2, JumpspeedMerged, P1CanJump, P2CanJump, MergedCanJump, merged, rad, Ying, Yan, obj3);

        PlayerJumpCollisions(Ying, Yan, obj3, rad, platform, platformCount, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged, oldP1, oldP2, oldMerged);

        }
    }

        BeginDrawing();

            switch(currentScreen)

            {
                case TITLE:
                {
                    DrawText("Ying and Yan", 150, 20, 80, WHITE);
                    DrawText("The Second Coming", 200, 100, 40, WHITE);
                    ClearBackground(BLACK);
                    DrawRectangleRec(startbutton, GRAY);
                    DrawText("START", 365, 230, 20, LIGHTGRAY);    
                } break;
                case LEVSEL:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawText("LEVEL SELECT", 150, 40, 60, WHITE);
                    int LevelSelectCount = 4;
                    for (int i = 0; i < LevelSelectCount; i++) DrawRectangleRec(levelselectbuttons[i], DARKGRAY);
                    DrawRectangleRec(backbutton, WHITE);

                } break;
                case GAMEPLAY:
                {
                    ClearBackground(SKYBLUE);


            DrawText("Jump with the up arrow", 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Invincibility left: %.1f", invincibilityTimer), 10, 100, 20, DARKGRAY);


            ExitGame();
            ExitDoors(Ying, Yan, exitDoorHitboxYin, exitDoorHitboxYang, rad);
            death(Ying, Yan, spikeHitboxes, spikeCount, rad, deathOccured, invincibility, invincibilityTimer); 
            forceUnmerge(Ying, Yan, obj3, spikeHitboxes, spikeCount, rad, merged, invincibility, invincibilityTimer);


            if(!merged){
                DrawCircleV(Ying, rad, RED);
                DrawCircleV(Yan, rad, BLUE);
            }
            else{
                DrawCircleV(obj3, rad, PURPLE);
            }

            for (int i = 0; i < platformCount; i++) DrawRectangleRec(platform[i], DARKGRAY);
            for (int i = 0; i < spikeCount; i++) DrawTriangle(spikes[i][0], spikes[i][1], spikes[i][2], BLACK);
            for (int i = 0; i < spikeCount; i++) DrawRectangleRec(spikeHitboxes[i], RED); //draw hitboxes for testing
            DrawRectangleRec(exitDoorHitboxYin, GREEN);
            DrawRectangleRec(exitDoorHitboxYang, GREEN);

            
                    for (int i = 0; i < platformCount; i++) DrawRectangleRec(platform[i], DARKGRAY);

                }break;
                default: break;
            }
        EndDrawing();   
    }

    CloseWindow();       
    return 0;
}