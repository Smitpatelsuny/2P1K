#include "DeaStart.h" 
#include <raymath.h> 
#include <raylib.h>
#include "Objects.h"
 


void death(Vector2 &player, Vector2 &player2, Vector2 P1Spawn, Vector2 P2Spawn, bool &invincibility, float &invincibilityTimer, bool &restart) 
{   
    if (invincibility)
    {
        return;
    }
    restart = true;
}

void forceUnmerge(Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, float &rad, bool &merged, bool &invincibility, float &invincibilityTimer)
{
            merged = false;
            player.x = MergedPlayer.x - rad;
            player.y = MergedPlayer.y;

            player2.x = MergedPlayer.x + rad;
            player2.y = MergedPlayer.y;

            MergedPlayer = {0, 0};

            invincibility = true;
            invincibilityTimer = 1.0f; // reset timer for invincibility
}

void ExitGame() 
{
    if (IsKeyPressed(KEY_ESCAPE)) 
    {
        CloseWindow();
    }
}

bool ExitDoors(Vector2 &player, Vector2 &player2, Rectangle exitDoorHitboxYin, Rectangle exitDoorHitboxYang, float &rad, int& levelcom) 
{
    if (CheckCollisionCircleRec(player, rad, exitDoorHitboxYin) 
    && CheckCollisionCircleRec(player2, rad, exitDoorHitboxYang)) 
    {
        if(levelcom = 0){
            levelcom = 1;
        }
        return true;
    }
    return false;
}

void MergeTot(Vector2 Ying, Vector2 Yan, Vector2 obj3, int& wordCount, int& wordCount2){
    
    //DrawText(TextSubtext("Use WASD to move", 0, wordCount), 100, 600, 15, WHITE);
    //DrawText(TextSubtext("Use Arrow Keys to move", 0, wordCount), 525, 600, 15, WHITE);
    
    Rectangle TotBox{325, 0, 150, 800};

    wordCount += 1;

    if(CheckCollisionCircleRec(Ying, 20, TotBox) || CheckCollisionCircleRec(Yan, 20, TotBox) || CheckCollisionCircleRec(obj3, 20, TotBox)){
        MergeTot2(wordCount2);
        wordCount = 0;
    }

   
}

void MergeTot2(int& wordCount2){
    //DrawText(TextSubtext("Press spacebar to merge \nPress again to unmerge \nUse A and D to move \nUse Up arrow to jump",L 0, wordCount2), 550, 325, 15, WHITE);
    wordCount2 += 1;
}

void InvisCountDown(bool &invis, float &inviTimer){
    inviTimer -= GetFrameTime();

        if (inviTimer <= 0.0f)
        {
            invis = false;
            inviTimer = 1.0f; // reset timer for next time
        }
}

void DeaStart::Initialize() {
}


void Restart (Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, Vector2 p1spawn, Vector2 p2spawn, bool &merged, bool &restart)  
{ 
    if (IsKeyPressed(KEY_R))  
    {
        player =  p1spawn; 
        player2 = p2spawn; 
        MergedPlayer = {0, 0}; 
        merged = false; 

        restart = true;
        

    }
}
