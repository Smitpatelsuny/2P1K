#ifndef DEASTART_H 
#define DEASTART_H 

#include <raylib.h> 
#include <raymath.h> 

void death(Vector2 &player, Vector2 &player2, Vector2 P1Spawn, Vector2 P2Spawn, bool &invincibility, float &invincibilityTimer);
void forceUnmerge(Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, float &rad, bool &merged, bool &invincibility, float &invincibilityTimer);
void ExitGame(); 
void Restart (Vector2 &player, Vector2 &player2, Vector2 &MergedPlayer, Vector2 p1spawn, Vector2 p2spawn, bool &merged);
bool ExitDoors(Vector2 &player, Vector2 &player2, Rectangle exitDoorHitboxYin, Rectangle exitDoorHitboxYang, float &rad, int& levelcom); 
void MergeTot(Vector2 Ying, Vector2 Yan, Vector2 obj3, int& wordCount, int& wordCount2);
void MergeTot2(int& wordCount2);

class DeaStart 
{ 
public: 

    void Initialize(); 

}; 

#endif 