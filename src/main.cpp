#include "raylib.h"
#include "PlayerStuff.h"
#include <stdio.h>

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
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "All code test");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 player = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 obj3 = {0, 0};

    Rectangle boxCollision = {0};
    Rectangle platform[] = {
    // FLOOR + BOUNDARIES
    {   0, 585, 800, 15 },   // floor
    {   0,   0,  15, 600 },  // left wall
    { 785,   0,  15, 600 },  // right wall
    {   0,   0, 800, 15 },   // ceiling (optional)
    {  80, 500, 140, 15 },  //x, y, width, height
    { 250, 440, 140, 15 },  //platforms
    { 420, 380, 140, 15 },
    { 590, 320, 140, 15 },
    { 260, 520, 280, 15 },
    { 520, 240, 240, 15 },   
    { 120, 360, 180, 15 },
    {  60, 280, 120, 15 },
    };

    SetTargetFPS(60);     
    
    int platformCount = (int)(sizeof(platform) / sizeof(platform[0]));
  
    while (!WindowShouldClose())    
    {
        MergeObjects(player, ballPosition, obj3, rad, merged);

        boxCollision = (Rectangle){0};

        // Save old positions BEFORE moving
        Vector2 oldP1 = player;
        Vector2 oldP2 = ballPosition;
        Vector2 oldMerged = obj3;


        //X-axis movement and collision
        if(merged){
            MergePlayerMotor(obj3, 2);
        }
        else{
            PlayerMotor(player, 2);
            Player2Motor(ballPosition, 2);
        }
   
        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(player, rad, platform[i])) {
                player.x = oldP1.x;//go back to old position
                break;
            }
        }
        
        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(ballPosition, rad, platform[i])) {
                ballPosition.x = oldP2.x; //go back to old position
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
        Jump(Jumpspeed, Jumpspeed2, JumpspeedMerged, P1CanJump, P2CanJump, MergedCanJump, merged, rad, player, ballPosition, obj3);

        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(player, rad, platform[i])) {
                player.y = oldP1.y;//go back to old position
                P1CanJump = true;
                Jumpspeed = 0.0;
                break;
            }
        }

        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(ballPosition, rad, platform[i])) {
                ballPosition.y = oldP2.y; //go back to old position
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


        BeginDrawing();

            ClearBackground(SKYBLUE);

            DrawText("Jump with the up arrow", 10, 10, 20, DARKGRAY);
            
            if(!merged){
                DrawCircleV(player, rad, RED);
                DrawCircleV(ballPosition, rad, BLUE);
            }
            else{
                DrawCircleV(obj3, rad, PURPLE);
            }

            for (int i = 0; i < platformCount; i++) DrawRectangleRec(platform[i], DARKGRAY);
        EndDrawing();   
    }
    CloseWindow();       

    return 0;
}
