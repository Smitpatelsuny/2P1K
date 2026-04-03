#include "raylib.h"
#include "Objects.h"
#include "PlayerStuff.h"
#include <stdio.h>

Rectangle LyftL1 = {250, 300, 50, 15 };
float LyftSpeed = 0.5f;


void levelOne(Vector2 Ying, Vector2 Yan, Vector2 obj3, int rad)
{
    Rectangle ButtonLyft = {600, 290, 10, 5};

    DrawRectangleRec(ButtonLyft, RED);
    DrawRectangleRec(LyftL1, GRAY);
    //DrawText(TextFormat("Lift Y: %.2f", LyftL1.y), 10, 130, 20, RED);

    if(CheckCollisionCircleRec(Ying, rad, ButtonLyft) || CheckCollisionCircleRec(Yan, rad, ButtonLyft) || CheckCollisionCircleRec(obj3, rad, ButtonLyft))
    {
        if(LyftL1.y <= 500){
            LyftL1.y += LyftSpeed;
        }
    }else if(!CheckCollisionCircleRec(Ying, rad, ButtonLyft) || !CheckCollisionCircleRec(Yan, rad, ButtonLyft) || !CheckCollisionCircleRec(obj3, rad, ButtonLyft))
    {
        if(LyftL1.y >= 300){
            LyftL1.y -= LyftSpeed;
        }
    }

}

void LyftHorControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, Rectangle box){
    PlayerPlatformCollisions(Ying, rad, &LyftL1, 1, Yan, obj3, oldP1, oldP2, oldMerged, box);
}

void LyftVertControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, Rectangle box){
    PlayerJumpCollisions(Ying, Yan, obj3, rad, &LyftL1, 1, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged, oldP1, oldP2, oldMerged, box);

    if (CheckCollisionCircleRec(Ying, 21, LyftL1)) {
        Ying.y -= LyftSpeed;
    }
    if (CheckCollisionCircleRec(Yan, 21, LyftL1)) {
        Yan.y -= LyftSpeed;
    }
    if (CheckCollisionCircleRec(obj3, 21, LyftL1)) {
        obj3.y -= LyftSpeed;
    }
}

void PlayerBoxCollisions(Vector2& player, float rad, Vector2& ballPosition, Vector2& obj3, int platformCount, Rectangle platforms[], Rectangle &box, float &BoxGravSpeed, Rectangle oldBox, Vector2 oldP1, Vector2 oldP2, Vector2 oldMerge)
{
    Rectangle newBox = {box.x, box.y, 54, 50};

    for (int i = 0; i < platformCount; i++) {
        if(!(CheckCollisionRecs(box, platforms[i])))
        {
            if(BoxGravSpeed <= -0.8){
                BoxGravSpeed = -0.8;
            }

            box.y -= BoxGravSpeed;
            BoxGravSpeed = BoxGravSpeed - 0.1f;
        }
    }

    for (int i = 0; i < platformCount; i++)
    {
        if (CheckCollisionRecs(box, platforms[i])) {
            box.x = oldBox.x;
            box.y = oldBox.y;
            BoxGravSpeed = 0;
        }

        if(CheckCollisionCircleRec(player, rad, box)){
            if (CanMoveBox(box, 2.0f, 0, platforms, platformCount) || CanMoveBox(box, -2.0f, 0, platforms, platformCount)){
                if (player.x < box.x){
                    box.x += 2.0f; //push left
                }else{
                    box.x -= 2.0f; // push right
                }
            }
        }

        if(CheckCollisionCircleRec(ballPosition, rad, box)){
            if (CanMoveBox(box, 2.0f, 0, platforms, platformCount) || CanMoveBox(box, -2.0f, 0, platforms, platformCount)){
                if (ballPosition.x < box.x){
                    box.x += 2.0f; //push left
                }else{
                    box.x -= 2.0f; // push right
                }
            }
        }
        
        if(CheckCollisionCircleRec(obj3, rad, box)){
            if (CanMoveBox(box, 2.0f, 0, platforms, platformCount) || CanMoveBox(box, -2.0f, 0, platforms, platformCount)){
                if (obj3.x < box.x){
                    box.x += 2.0f; //push left
                }else{
                    box.x -= 2.0f; // push right
                }
            }
        }
    }
            
}

bool CanMoveBox(Rectangle box, float dx, float dy, Rectangle platforms[], int platformCount)
{
    Rectangle newBox = box;
    newBox.x += dx;
    newBox.y += dy;

    for (int i = 0; i < platformCount; i++)
    {
        if (CheckCollisionRecs(newBox, platforms[i]))
            return false;
    }
    return true;
}
