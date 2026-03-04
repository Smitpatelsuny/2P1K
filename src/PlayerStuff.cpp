#include "raylib.h"
#include "PlayerStuff.h"
#include <stdio.h>

//smit commit

void PlayerBorderCollisions(Vector2& player, int radius, int screenWidth, int screenHeight, bool& CanJump)
{
     if (player.x + radius >= screenWidth) 
        {
            player.x = screenWidth - radius;
        }

        if (player.x - radius <= 0) 
        {
            player.x = radius;
        }

    if (player.y + radius >= screenHeight) 
    {
        player.y = screenHeight - radius;
        CanJump = true;
    }
    if (player.y - radius <= 0){
        player.y = radius;
    }
}

void PlayerMotor(Vector2 &player, int speedX)
{
        if (IsKeyDown(KEY_D)) player.x += speedX;
        if (IsKeyDown(KEY_A)) player.x -= speedX;
        
         //DrawText(TextFormat("Speed is: %.1f", Jumpspeed), 10, 50, 20, DARKGRAY);
}

void Player2Motor(Vector2& player, int speedX)
{
    if (IsKeyDown(KEY_RIGHT)) player.x += speedX;
    if (IsKeyDown(KEY_LEFT)) player.x -= speedX;
}

void MergePlayerMotor(Vector2 &player, int speedX)
{
        if (IsKeyDown(KEY_D)) player.x += speedX;
        if (IsKeyDown(KEY_A)) player.x -= speedX;
}

void Jump(float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, bool &merged, float radius, Vector2 &player, Vector2 &ballPosition, Vector2 &obj3)
{ 
    float maxJump = 3.5;
    if (IsKeyDown(KEY_W) && P1CanJump == true){
            if (Jumpspeed <= 0.0) {
                Jumpspeed = 0.0;
            }
                if(Jumpspeed < maxJump){
                    Jumpspeed = Jumpspeed + 0.5;
                    player.y -= Jumpspeed;
                }

            if(Jumpspeed >= maxJump) P1CanJump = false;   
        }

        if(IsKeyReleased(KEY_W)){
            P1CanJump = false;
        }

        if(Jumpspeed <= -6.8){
            Jumpspeed = -6.8;
        }
        player.y -= Jumpspeed;
        Jumpspeed = Jumpspeed - 0.1;

        DrawText(TextFormat("Speed is: %.1f", Jumpspeed), 30, 70, 20, DARKGRAY);
        

        if (IsKeyDown(KEY_UP) && P2CanJump == true && !merged){
           if (Jumpspeed2 <= 0.0){
                Jumpspeed2 = 0.0;
            }
            
            if(Jumpspeed2 < 80){
                Jumpspeed2 = Jumpspeed2 + 0.5;
                ballPosition.y -= Jumpspeed2;
            }

            if(Jumpspeed2 >= 80) P2CanJump = false;  
        }

    if(IsKeyReleased(KEY_UP) && !merged){
        P2CanJump = false;
    }
    
    if(Jumpspeed2 <= -6.8){
        Jumpspeed2 = -6.8;
    }
    ballPosition.y -= Jumpspeed2;
    Jumpspeed2 = Jumpspeed2 - 0.1;

    DrawText(TextFormat("Speed is: %.1f", Jumpspeed2), 50, 90, 20, DARKGRAY);
    

    if (IsKeyDown(KEY_UP) && MergedCanJump == true && merged){
           if (JumpspeedMerged <= 0.0){
                JumpspeedMerged = 0.0;
            }
            
            if(JumpspeedMerged < maxJump){
                JumpspeedMerged = JumpspeedMerged + 0.5;
                obj3.y -= JumpspeedMerged;
            }

            if(JumpspeedMerged >= maxJump) MergedCanJump = false;  
        }

    if(IsKeyReleased(KEY_UP) && merged){
        MergedCanJump = false;
    }
        

    if(JumpspeedMerged <= -6.8){
        JumpspeedMerged = -6.8;
    }
    obj3.y -= JumpspeedMerged;
    JumpspeedMerged = JumpspeedMerged - 0.1;

    DrawText(TextFormat("Speed is: %.1f", JumpspeedMerged), 10, 50, 20, DARKGRAY);  
}

void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, float& radius1, bool& merged, bool invincibility){

    if(invincibility) return; // prevent merging while invincible
    
    
    if(!merged){
        float distance = Vector2Distance(obj1, obj2);

        if (distance <= (radius1 * 2) && IsKeyPressed(KEY_SPACE)){
            obj3.x = (obj1.x + obj2.x) / 2;
            obj3.y = (obj1.y + obj2.y) / 2;
            merged = true;
        }
    }
    else{
        if(IsKeyPressed(KEY_SPACE)){
            merged = false;
            obj1.x = obj3.x - radius1;
            obj1.y = obj3.y;
            obj2.x = obj3.x + radius1;
            obj2.y = obj3.y;
        }
    }
}

void PlayerPlatformCollisions(Vector2& player, int rad, Rectangle platforms[], int platformCount, Vector2& ballPosition, Vector2& obj3, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged)
{
    for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(player, rad, platforms[i])) {
                player.x = oldP1.x;//go back to old position
                break;
            }
        }
        
        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(ballPosition, rad, platforms[i])) {
                ballPosition.x = oldP2.x; //go back to old position
                break;
            }
        }

         for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(obj3, rad, platforms[i])) {
                obj3.x = oldMerged.x;//go back to old position
                break;
            }
        }
        
}

void PlayerJumpCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3, float rad, Rectangle platforms[], int platformCount, bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump, float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged)
{
    for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(player, rad, platforms[i])) {
                player.y = oldP1.y;//go back to old position
                P1CanJump = true;
                Jumpspeed = 0.0;
                break;
            }
        }

        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(ballPosition, rad, platforms[i])) {
                ballPosition.y = oldP2.y; //go back to old position
                P2CanJump = true;
                Jumpspeed2 = 0.0;
                break;
            }
        }
        
          for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(obj3, rad, platforms[i])) {
                obj3.y = oldMerged.y; //go back to old position
                MergedCanJump = true;
                JumpspeedMerged = 0.0;
                break;
            }
        }
}




void PlayerStuff::PlayerMovement() {
}