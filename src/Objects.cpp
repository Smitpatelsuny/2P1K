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

void LyftHorControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged){
    PlayerPlatformCollisions(Ying, rad, &LyftL1, 1, Yan, obj3, oldP1, oldP2, oldMerged);
}

void LyftVertControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged){
    PlayerJumpCollisions(Ying, Yan, obj3, rad, &LyftL1, 1, P1CanJump, P2CanJump, MergedCanJump, Jumpspeed, Jumpspeed2, JumpspeedMerged, oldP1, oldP2, oldMerged);

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

