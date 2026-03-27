

#ifndef DEASTART_H
#define DEASTART_H

#include <raylib.h>
#include <raymath.h>

void death(Vector2 &player, Vector2 &player2, Rectangle spikeHitboxes[], int numSpikes, float &rad, bool &deathOccured, bool &invincibility, float &invincibilityTimer);
void forceUnmerge(Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, Rectangle spikeHitboxes[], int numSpikes, float &rad, bool &merged, bool &invincibility, float &invincibilityTimer);
void ExitGame();
bool ExitDoors(Vector2 &player, Vector2 &player2, Rectangle exitDoorHitboxYin, Rectangle exitDoorHitboxYang, float &rad);
    
   
class DeaStart
{
public:
    void Initialize();


};
#endif
