#ifndef PLAYERSTUFF_H
#define PLAYERSTUFF_H

#include <raylib.h>
#include <raymath.h>


void PlayerMotor(Vector2 &player, int speedX);
void Player2Motor(Vector2& player, int speedX);
void MergePlayerMotor(Vector2 &player, int speedX);
void PlayerBorderCollisions(Vector2& player, int radius, int screenWidth, int screenHeight, bool& CanJump);
void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, int radius1, int radius2, bool& merged);
void Jump(float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, bool &merged, float radius, Vector2 &player, Vector2 &ballPosition, Vector2 &obj3);
void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, float& radius1, bool& merged);

class PlayerStuff
{
public:
    void PlayerMovement();

};


#endif