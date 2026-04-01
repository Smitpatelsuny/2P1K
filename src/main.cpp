#include "raylib.h"
#include "PlayerStuff.h"
#include "DeaStart.h"
#include <stdio.h>
#include "Objects.h"
 
 enum GameScreen{ 
    TITLE,
    LEVSEL,
    GAMEPLAY
    };



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
    bool ejected;
    float BoxGravSpeed = 0;
    int levelcom = 0;

    InitWindow(screenWidth, screenHeight, "All code test");
    Texture2D background_menu = LoadTexture("C:/Users/austi/projects/New2P1K/2P1K-2/src/assets/background_menu.jpg");
    // Texture2D gamePlay = LoadTexture("C:/Users/austi/projects/New2P1K/2P1K-2/src/assets/gamePlay.png");
    Texture2D gamePlay2 = LoadTexture("C:/Users/austi/projects/New2P1K/2P1K-2/src/assets/gamePlay2.jpg");
    // Texture2D p1Texture = LoadTexture("resources/white.png");
    // Texture2D p2Texture = LoadTexture ("resources/black.png");

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
    { 100, 580, 600, 15 },   // 2nd Middle platform
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
        {{193, 580}, {233, 580}, {213, 550}}, //spike on middle left platform
        {{558, 580}, {598, 580}, {578, 550}}, //spike on middle right platform
    };

    //spike parameters: first vetice = (x of platform + 15, y of platform), 
    //second vertice = (x of platform + 55, y of platform), 
    //third vertice = (x of platform + 35, y of platform - 30)

    Rectangle spikeHitboxes[] = {
        {110, 685, 30, 15}, //spike hitbox for first left platform
        {655, 685, 30, 15}, //spike hitbox for first right platform
        {198, 565, 30, 15}, //spike hitbox for middle left platform
        {563, 565, 30, 15}, //spike hitbox for middle right platform
    };

    //future spike hitbox: +5 on x (first vertice) and -15 on y from the spike's position, 
    //with a width of 30 and height of 15

    Rectangle exitDoorHitboxYin = { 10, 280, 30, 30 };
    Rectangle exitDoorHitboxYang = { 760, 280, 30, 30 };

    Rectangle Box = {300, 100, 50, 50};

    SetTargetFPS(60);     
    
    int platformCount = (int)(sizeof(platform) / sizeof(platform[0]));
    int spikeCount = (int)(sizeof(spikes) / sizeof(spikes[0]));
  
    while (!WindowShouldClose())    
    {
        MergeObjects(Ying, Yan, obj3, rad, merged, invincibility, Jumpspeed2, ejected);

        switch(currentScreen)
        {
            case TITLE:
        {
                if(IsGestureDetected(GESTURE_TAP) && CheckCollisionPointRec(GetMousePosition(), startbutton))
                {
                    currentScreen = LEVSEL;
                }

            ClearBackground(WHITE);

            DrawText(TextFormat("bg width: %i", background_menu.width), 20, 20, 20, WHITE);
            DrawText(TextFormat("bg height: %i", background_menu.height), 20, 50, 20, WHITE);

            if (background_menu.width > 0 && background_menu.height > 0)
            {
                DrawTexturePro(
                    background_menu,
                    (Rectangle){0, 0, (float)background_menu.width, (float)background_menu.height},
                    (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},
                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
            int startSize = 20;
            int startWidth = MeasureText("START", startSize);

            DrawText("Ying and Yan", 150, 20, 80, WHITE);
            DrawText("The Second Coming", 200, 100, 40, WHITE);
            DrawRectangleRec(startbutton, GREEN);
            

    DrawText(
        "START",
        startbutton.x + startbutton.width/2 - startWidth/2,
        startbutton.y + startbutton.height/2 - startSize/2,
        startSize,
        LIGHTGRAY);
        }
        break;
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
                if(IsKeyPressed(KEY_P)||ExitDoors(Ying, Yan, exitDoorHitboxYin, exitDoorHitboxYang, rad, levelcom))
                {
                    currentScreen = LEVSEL;
                }     

                // Save old positions BEFORE moving
                Vector2 oldP1 = Ying;
                Vector2 oldP2 = Yan;
                Vector2 oldMerged = obj3;
                Rectangle oldBox = Box;


                //X-axis movement and collision
                if(merged){
                    MergePlayerMotor(obj3, 2);
                }
                else{
                    PlayerMotor(Ying, 2);
                    Player2Motor(Yan, 2);
                }
   
        PlayerPlatformCollisions(Ying, rad, platform, platformCount, Yan, obj3, oldP1, oldP2, oldMerged);
        LyftHorControls(Ying, Yan, obj3, rad, oldP1, oldP2, oldMerged);

        //Y-axis movement and collision
        Jump(Jumpspeed, Jumpspeed2, JumpspeedMerged, P1CanJump, P2CanJump, MergedCanJump, merged, rad, Ying, Yan, obj3, ejected);

        PlayerJumpCollisions(Ying, Yan, obj3, rad, platform, platformCount, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged, oldP1, oldP2, oldMerged);
        LyftVertControls(Ying, Yan, obj3, rad, oldP1, oldP2, oldMerged, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged);

        BoxPhy(Box, platformCount, platform, BoxGravSpeed);
        PlayerBoxCollisions(Ying, rad, Yan, obj3, platformCount, platform, Box, BoxGravSpeed, oldBox);
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
            DrawTexturePro(
                gamePlay2, {0, 0, (float)gamePlay2.width, (float)gamePlay2.height},
                {0, 0, (float)screenWidth, (float)screenHeight}, (Vector2){0, 0}, 0.0f, WHITE);

            ExitGame();
            ExitDoors(Ying, Yan, exitDoorHitboxYin, exitDoorHitboxYang, rad, levelcom);
            death(Ying, Yan, spikeHitboxes, spikeCount, rad, deathOccured, invincibility, invincibilityTimer);
            forceUnmerge(Ying, Yan, obj3, spikeHitboxes, spikeCount, rad, merged, invincibility, invincibilityTimer);
            levelOne(Ying, Yan, obj3, rad);
           
            DrawText(TextFormat("X Speed is: %.1f", Box.x), 20, 20, 20, WHITE);
            DrawText(TextFormat("Speed is: %.1f", BoxGravSpeed), 30, 70, 20, DARKGRAY);

            if (!merged)
            {
                DrawCircleV(Ying, rad, RED);
                DrawCircleV(Yan, rad, BLUE);
            }
            else
            {
                DrawCircleV(obj3, rad, PURPLE);
            }

            for (int i = 0; i < platformCount; i++)
                DrawRectangleRec(platform[i], DARKGRAY);
            for (int i = 0; i < spikeCount; i++)
                DrawTriangle(spikes[i][0], spikes[i][1], spikes[i][2], RED);
           /* for (int i = 0; i < spikeCount; i++)
                DrawRectangleRec(spikeHitboxes[i], RED); // draw hitboxes for testing */
            DrawRectangleRec(exitDoorHitboxYin, GREEN);
            DrawRectangleRec(exitDoorHitboxYang, GREEN);
            DrawRectangleRec(Box, BROWN);

            for (int i = 0; i < platformCount; i++)
                DrawRectangleRec(platform[i], DARKGRAY);
        }
        break;
        default:
            break;
        }
        EndDrawing();
    }
    UnloadTexture(background_menu);
    CloseWindow();
    return 0;
}