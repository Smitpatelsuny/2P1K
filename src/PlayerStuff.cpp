#include "raylib.h"
#include "PlayerStuff.h"
#include <stdio.h>
#include "Objects.h"
#include <vector>

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

void Jump(float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, bool &merged, float radius, Vector2 &player, Vector2 &ballPosition, Vector2 &obj3, bool Ejected)
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

       // DrawText(TextFormat("Speed is: %.1f", Jumpspeed), 30, 70, 20, DARKGRAY);
        

        if (IsKeyDown(KEY_UP) && P2CanJump == true && !merged){
           if (Jumpspeed2 <= 0.0){
                Jumpspeed2 = 0.0;
            }
            
            if(Jumpspeed2 < maxJump){
                Jumpspeed2 = Jumpspeed2 + 0.5;
                ballPosition.y -= Jumpspeed2;
            }

            if(Jumpspeed2 >= maxJump) P2CanJump = false;  
        }

    if(IsKeyReleased(KEY_UP) && !merged){
        P2CanJump = false;
    }
    
    if(Jumpspeed2 <= -6.8){
        Jumpspeed2 = -6.8;
    }
    ballPosition.y -= Jumpspeed2;
    Jumpspeed2 = Jumpspeed2 - 0.1;

    //DrawText(TextFormat("Speed is: %.1f", Jumpspeed2), 50, 90, 20, DARKGRAY);
    

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
    JumpspeedMerged = JumpspeedMerged - 0.25;

    //DrawText(TextFormat("Speed is: %.1f", JumpspeedMerged), 10, 50, 20, DARKGRAY);  
}


void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, float& radius1, bool& merged, bool invincibility, float &YanJumpSpeed, bool Ejected){

    if(invincibility){
        return; // prevent merging while invincible
    } 
    
    if(!merged){
        float distance = Vector2Distance(obj1, obj2);

        if (distance <= (radius1 * 2) && IsKeyPressed(KEY_SPACE)){
            obj3.x = (obj1.x + obj2.x) / 2;
            obj3.y = (obj1.y + obj2.y) / 2;
            merged = true;
            obj1.x = 0;
            obj1.y = 0;
            obj2.x = 0;
            obj2.y = 0;
        }
    }
    else{
        if(IsKeyPressed(KEY_SPACE)){
            merged = false;
            obj1.x = obj3.x - radius1;
            obj1.y = obj3.y;
            obj2.x = obj3.x + radius1;
            obj2.y = obj3.y;
            obj3.x = 0;
            obj3.y = 0;
            if (IsKeyDown(KEY_UP)){
                Ejected = true;
                YanJumpSpeed = YanJumpSpeed + 6;
                obj2.y -= YanJumpSpeed;
            }
        }
    }
}

void PlayerPlatformCollisions(Vector2& player, int rad, Rectangle platforms[], int platformCount, Vector2& ballPosition, Vector2& obj3, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, const std::vector<BoxData>& box)
{
    for (int i = 0; i < platformCount; i++) {
        for (const auto& b : box)
        {
            Rectangle boxRect = b.ToRect();

            if (CheckCollisionCircleRec(player, rad, platforms[i]) || CheckCollisionCircleRec(player, rad-0.1, boxRect)) {
                player.x = oldP1.x;//go back to old position
               
            }
            if (CheckCollisionCircleRec(ballPosition, rad, platforms[i]) || CheckCollisionCircleRec(ballPosition, rad-0.1, boxRect)) {
                ballPosition.x = oldP2.x; //go back to old position
               
            }

            if (CheckCollisionCircleRec(obj3, rad, platforms[i]) || CheckCollisionCircleRec(obj3, rad-0.1, boxRect)) {
                obj3.x = oldMerged.x;//go back to old position
               
            }
        }    
    }  
}

void PlayerJumpCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3, float rad, Rectangle platforms[], int platformCount, bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump, float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, Rectangle box)
{
    for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(player, rad, platforms[i]) || CheckCollisionCircleRec(player, rad, box)) {
                player.y = oldP1.y;//go back to old position
    
                if((platforms[i].height+platforms[i].y) >= player.y){
                    P1CanJump = true;
                }else{
                    P1CanJump = false;
                }
                Jumpspeed = 0.0;
                break;
            }
        }

        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(ballPosition, rad, platforms[i]) || CheckCollisionCircleRec(ballPosition, rad, box)) {
                ballPosition.y = oldP2.y; //go back to old position
                if((platforms[i].height+platforms[i].y) >= ballPosition.y){
                    P2CanJump = true;
                }else{
                    P2CanJump = false;
                }
                Jumpspeed2 = 0.0;
                break;
            }
        }
        
          for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionCircleRec(obj3, rad, platforms[i]) || CheckCollisionCircleRec(obj3, rad, box)) {
                obj3.y = oldMerged.y; //go back to old position
                if((platforms[i].height+platforms[i].y) >= obj3.y){
                    MergedCanJump = true;
                }else{
                    MergedCanJump = false;
                }
                JumpspeedMerged = 0.0;
                break;
            }
        }
}


AnimatedSprite LoadAnimatedSprite(const char* filePath, float scale, int columns, int rows, float frameDuration)
{
    AnimatedSprite sprite = {};

    Image img = LoadImage(filePath);

    int newWidth = (int)(img.width * scale);
    int newHeight = (int)(img.height * scale);

    ImageResizeNN(&img, newWidth, newHeight);

    sprite.texture = LoadTextureFromImage(img);
    UnloadImage(img);

    sprite.columns = columns;
    sprite.rows = rows;
    sprite.frameWidth = sprite.texture.width / columns;
    sprite.frameHeight = sprite.texture.height / rows;
    sprite.frameRec = {0, 0, (float)sprite.frameWidth, (float)sprite.frameHeight};
    sprite.currentFrame = 0;
    sprite.currentRow = 0;
    sprite.frameTimer = 0.0f;
    sprite.frameDuration = frameDuration;

    return sprite;
}

void UpdateAnimatedSprite(AnimatedSprite& sprite, AnimState state)
{
    sprite.currentRow = (int)state;

    int numFrames = 1;

    if (state == ANIM_IDLE || state == ANIM_RUN)
        numFrames = sprite.columns;
    else
        numFrames = 1;

    sprite.frameTimer += GetFrameTime();

    if (sprite.frameTimer >= sprite.frameDuration)
    {
        sprite.frameTimer = 0.0f;
        sprite.currentFrame++;

        if (sprite.currentFrame >= numFrames)
            sprite.currentFrame = 0;
    }

    if (numFrames == 1)
        sprite.currentFrame = 0;

    sprite.frameRec.x = (float)(sprite.currentFrame * sprite.frameWidth);
    sprite.frameRec.y = (float)(sprite.currentRow * sprite.frameHeight);
}

void DrawAnimatedSpriteCentered(const AnimatedSprite& sprite, Vector2 centerPos, bool facingRight, Color tint)
{
    Rectangle source = sprite.frameRec;

    if (!facingRight)
    {
        source.x += sprite.frameWidth;
        source.width = -(float)sprite.frameWidth;
    }

    Vector2 drawPos = {
        centerPos.x - sprite.frameWidth / 2.0f,
        centerPos.y - sprite.frameHeight / 2.0f
    };

    DrawTextureRec(sprite.texture, source, drawPos, tint);
}

void UnloadAnimatedSprite(AnimatedSprite& sprite)
{
    if (sprite.texture.id != 0)
    {
        UnloadTexture(sprite.texture);
        sprite.texture = {0};
        
    }
}

void TutText(Vector2 Ying, Vector2 Yan, Vector2 obj3, int& framesCounter){
    const char YingMovement[128] = "You are Ying,\nYou use A, & D to move\nand W to jump";
    const char YanMovement[128] = "You are Yan,\nYou use the arrow keys\nto move and jump";
    const char Merge[248] = "When Ying and Yan overlap press space to merge,\nUse A & D to move, and Up arrow to jump\nIn this merged state, you'll be heavy and have decreased\njump height but you'll be able to take another hit";
    const char MergeJump[128] = "If Yan holds jump while you unmerge\nin air, he'll get extra height";
    const char Door[128] = "To complete the level both players\nwill have to get to their respective doors";

    if (Ying.x < 300 && Ying.y > 700 && Yan.x > 500 && Yan.y > 700)
    {
        DrawText(TextSubtext(YingMovement, 0, framesCounter/10), 25, 650, 12, WHITE);
        DrawText(TextSubtext(YanMovement, 0, framesCounter/10), 625, 650, 12, WHITE);
    }else if(Ying.y < 700 && Yan.y < 700 && Yan.y != 0 && Ying.y > 550 && Yan.y > 550){
        DrawText(TextSubtext(Merge, 0, framesCounter/10), 250, 200, 12, WHITE);
    }else if(obj3.x != 0){
        DrawText(TextSubtext(MergeJump, 0, framesCounter/10), 250, 250, 12, WHITE);
    }else if(Yan.y < 450){
        DrawText(TextSubtext(Door, 0, framesCounter/10), 250, 250, 12, WHITE);
    }else{
        framesCounter = 0; 
        return;
    } 

    if(IsKeyPressed(KEY_SPACE)) framesCounter = 0;
    framesCounter += 4;
}




void PlayerStuff::PlayerMovement() {
}

<<<<<<< HEAD
=======
void TutText(Vector2 Ying, Vector2 Yan, Vector2 obj3, int& framesCounter){
    const char YingMovement[128] = "You are Ying,\nYou use A, & D to move\nand W to jump";
    const char YanMovement[128] = "You are Yan,\nYou use the arrow keys\nto move and jump";
    const char Merge[248] = "When Ying and Yan overlap press space to merge,\nUse A & D to move, and Up arrow to jump\nIn this merged state, you'll be heavy and have decreased\njump height but you'll be able to take another hit";
    const char MergeJump[128] = "If Yan holds jump while you unmerge\nin air, he'll get extra height";
    const char Door[128] = "To complete the level both players\nwill have to get to their respective doors";

    if (Ying.x < 300 && Ying.y > 700 && Yan.x > 500 && Yan.y > 700)
    {
        DrawText(TextSubtext(YingMovement, 0, framesCounter/10), 25, 650, 12, WHITE);
        DrawText(TextSubtext(YanMovement, 0, framesCounter/10), 625, 650, 12, WHITE);
    }else if(Ying.y < 700 && Yan.y < 700 && Yan.y != 0 && Ying.y > 550 && Yan.y > 550){
        DrawText(TextSubtext(Merge, 0, framesCounter/10), 250, 200, 12, WHITE);
    }else if(obj3.x != 0){
        DrawText(TextSubtext(MergeJump, 0, framesCounter/10), 250, 250, 12, WHITE);
    }else if(Yan.y < 450){
        DrawText(TextSubtext(Door, 0, framesCounter/10), 250, 250, 12, WHITE);
    }else{
        framesCounter = 0; 
        return;
    } 

    if(IsKeyPressed(KEY_SPACE)) framesCounter = 0;
    framesCounter += 4;
}

>>>>>>> 2b5a552a69a9cf903b87bd6492b4f855a05af381
