#include "raylib.h"
#include "Objects.h"
#include <stdio.h>


void levelOne(Vector2 Ying, Vector2 Yan, Vector2 obj3){
    Rectangle Lyft = {250, 300, 50, 15 };
    Rectangle ButtonLyft = {600, 280, 10, 5};

    DrawRectangleRec(Lyft, GRAY);
    DrawRectangleRec(ButtonLyft, DARKGRAY);

    if(CheckCollisionPointRec(Ying, ButtonLyft) || CheckCollisionPointRec(Yan, ButtonLyft) || CheckCollisionPointRec(obj3, ButtonLyft))
    {
        if(Lyft.y > 200){
            Lyft.y -= 20;
        }

    }
}



