#include "raylib.h"
#include "PlayerStuff.h"
#include "DeaStart.h"
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
    bool deathOccured = false;
    bool invincibility = false;
    float invincibilityTimer = 5.0f; // 5 seconds of invincibility after unmerging

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
        MergeObjects(player, ballPosition, obj3, rad, merged);

        

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
   
        PlayerPlatformCollisions(player, rad, platform, platformCount, ballPosition, obj3, oldP1, oldP2, oldMerged);
        
        //Y-axis movement and collision
        Jump(Jumpspeed, Jumpspeed2, JumpspeedMerged, P1CanJump, P2CanJump, MergedCanJump, merged, rad, player, ballPosition, obj3);


        PlayerJumpCollisions(player, ballPosition, obj3, rad, platform, platformCount, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged, oldP1, oldP2, oldMerged);

        BeginDrawing();

            ClearBackground(SKYBLUE);

            DrawText("Jump with the up arrow", 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Invincibility left: %.1f", invincibilityTimer), 10, 100, 20, DARKGRAY);
            
            death(player, ballPosition, spikeHitboxes, spikeCount, rad, deathOccured, invincibility, invincibilityTimer); 
            forceUnmerge(player, ballPosition, obj3, spikeHitboxes, spikeCount, rad, merged, invincibility, invincibilityTimer);


            if(!merged){
                DrawCircleV(player, rad, RED);
                DrawCircleV(ballPosition, rad, BLUE);
            }
            else{
                DrawCircleV(obj3, rad, PURPLE);
            }

            for (int i = 0; i < platformCount; i++) DrawRectangleRec(platform[i], DARKGRAY);
            for (int i = 0; i < spikeCount; i++) DrawTriangle(spikes[i][0], spikes[i][1], spikes[i][2], BLACK);
            for (int i = 0; i < spikeCount; i++) DrawRectangleRec(spikeHitboxes[i], RED); //draw hitboxes for testing

            

        EndDrawing();   
    }
    CloseWindow();       

    return 0;
}
