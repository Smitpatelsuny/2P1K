#include "raylib.h"
#include "PlayerStuff.h"
#include <stdio.h>

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
        {{95, 500}, {135, 500}, {115, 470}}, //spike on first platform
        {{265, 440}, {305, 440}, {285, 410}}, //spike on second platform
        {{435, 380}, {475, 380}, {455, 350}}, //spike on third platform
        {{605, 320}, {645, 320}, {625, 290}}, //spike on fourth platform
    };

    //spike parameters: first vetice = (x of platform + 15, y of platform), 
    //second vertice = (x of platform + 55, y of platform), 
    //third vertice = (x of platform + 35, y of platform - 30)

    Rectangle spikeHitboxes[] = {
        {100, 485, 30, 15}, //spike hitbox for first platform
        {270, 425, 30, 15}, //spike hitbox for second platform
        {440, 365, 30, 15}, //spike hitbox for third platform
        {610, 305, 30, 15}, //spike hitbox for fourth platform
    };

    //future spike hitbox: +5 on x (first vertice) and -15 on y from the spike's position, 
    //with a width of 30 and height of 15


    SetTargetFPS(60);     
    
    int platformCount = (int)(sizeof(platform) / sizeof(platform[0]));
    int spikeCount = (int)(sizeof(spikes) / sizeof(spikes[0]));
  
    while (!WindowShouldClose())    
    {
        MergeObjects(Ying, Yan, obj3, rad, merged);

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
   
                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(Ying, rad, platform[i])) {
                        Ying.x = oldP1.x;//go back to old position
                        break;
                    }
                }
        
                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(Yan, rad, platform[i])) {
                        Yan.x = oldP2.x; //go back to old position
                        break;
                    }
                }

                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(obj3, rad, platform[i])) {
                        obj3.x = oldMerged.x;//go back to old position
                        break;
                    }
                }
        
                //Y-axis movement and collision
                Jump(Jumpspeed, Jumpspeed2, JumpspeedMerged, P1CanJump, P2CanJump, MergedCanJump, merged, rad, Ying, Yan, obj3);

                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(Ying, rad, platform[i])) {
                        Ying.y = oldP1.y;//go back to old position
                        P1CanJump = true;
                        Jumpspeed = 0.0;
                        break;
                    }
                }

                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(Yan, rad, platform[i])) {
                        Yan.y = oldP2.y; //go back to old position
                        P2CanJump = true;
                        Jumpspeed2 = 0.0;
                        break;
                    }
                }
        
                for (int i = 0; i < platformCount; i++) {
                    if (CheckCollisionCircleRec(obj3, rad, platform[i])) {
                        obj3.y = oldMerged.y; //go back to old position
                        MergedCanJump = true;
                        JumpspeedMerged = 0.0;
                        break;
                    }
                }
            }break;
            default: break;
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
            
                    if(!merged){
                        DrawCircleV(Ying, rad, RED);
                        DrawCircleV(Yan, rad, BLUE);
                    }
                    else{
                        DrawCircleV(obj3, rad, PURPLE);
                    }

                    for (int i = 0; i < platformCount; i++) DrawRectangleRec(platform[i], DARKGRAY);

                    } break;
                default: break;
            }
        EndDrawing();   
    }
    CloseWindow();       

    return 0;
}