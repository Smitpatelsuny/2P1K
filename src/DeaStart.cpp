#include "DeaStart.h" 
#include <raymath.h> 
#include <raylib.h> 

 

 

 

 

 

 

void death(Vector2 &player, Vector2 &player2, Rectangle spikeHitboxes[], int numSpikes, float &rad, bool &deathOccured, bool &invincibility, float &invincibilityTimer, Vector2 p1spawn, Vector2 p2spawn)  
{ 

     deathOccured = false; 

     

    if (invincibility) 

    { 

        invincibilityTimer -= GetFrameTime(); 

 

        if (invincibilityTimer <= 0.0f) 

        { 
            invincibility = false; 
            invincibilityTimer = 1.0f; // reset timer for next time 
        } 

        return; 

    } 

 

    for (int i = 0; i < numSpikes; i++)  
    { 

        if (CheckCollisionCircleRec(player, rad, spikeHitboxes[i])  
        || CheckCollisionCircleRec(player2, rad, spikeHitboxes[i]) 
        || IsKeyPressed(KEY_R))  
        { 
            player =  p1spawn; 
            player2 = p2spawn; 
            deathOccured = true; 
            break; 
        } 
    } 
} 

 

void forceUnmerge(Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, Rectangle spikeHitboxes[], int numSpikes, float &rad, bool &merged, bool &invincibility, float &invincibilityTimer) 

{ 

    for (int i = 0; i < numSpikes; i++)  

    { 

        if (CheckCollisionCircleRec(MergedPlayer, rad, spikeHitboxes[i]))  

        { 

            merged = false; 

            player.x = MergedPlayer.x - rad; 

            player.y = MergedPlayer.y; 

 

            player2.x = MergedPlayer.x + rad; 

            player2.y = MergedPlayer.y; 

 

            MergedPlayer = {0, 0}; 

 

            invincibility = true; 

            invincibilityTimer = 1.0f; // reset timer for invincibility 

 

            break; 

        } 

    } 
} 

 

void ExitGame()  
{ 
    if (IsKeyPressed(KEY_ESCAPE))  
    { 
        CloseWindow(); 
    } 

} 

 

bool ExitDoors(Vector2 &player, Vector2 &player2, Rectangle exitDoorHitboxYin, Rectangle exitDoorHitboxYang, float &rad)  
{ 

    if (CheckCollisionCircleRec(player, rad, exitDoorHitboxYin)  
    && CheckCollisionCircleRec(player2, rad, exitDoorHitboxYang))  
    { 

        return true; 

    } 
    return false; 

} 

 
void DeaStart::Initialize() { 

} 