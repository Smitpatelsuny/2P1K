#ifndef PLAYERSTUFF_H
#define PLAYERSTUFF_H

#include <raylib.h>
#include <raymath.h>


void PlayerMotor(Vector2 &player, int speedX);
void Player2Motor(Vector2& player, int speedX);
void MergePlayerMotor(Vector2 &player, int speedX);
void PlayerBorderCollisions(Vector2& player, int radius, int screenWidth, int screenHeight, bool& CanJump);
void Jump(float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, bool &merged, float radius, Vector2 &player, Vector2 &ballPosition, Vector2 &obj3, bool Ejected);
void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, float& radius1, bool& merged, bool invincibility, float &YanJumpSpee, bool Ejected);
void PlayerPlatformCollisions(Vector2& player, int rad, Rectangle platforms[], int platformCount, Vector2& ballPosition, Vector2& obj3, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, Rectangle box);
void PlayerJumpCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3, float rad, Rectangle platforms[], int platformCount, bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump, float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, Rectangle box);



class PlayerStuff
{
public:
    void PlayerMovement();

};


#endif