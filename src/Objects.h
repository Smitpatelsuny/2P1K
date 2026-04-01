#ifndef OBJECTS_H
#define OBJECTS_H

#include <raylib.h>
#include <raymath.h>


void levelOne(Vector2 Ying, Vector2 Yan, Vector2 obj3, int rad);
void LyftHorControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged);
void LyftVertControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged);
void BoxPhy(Rectangle &Box, int platformCount, Rectangle platforms[], float &BoxGravSpeed);
void PlayerBoxCollisions(Vector2& player, float rad, Vector2& ballPosition, Vector2& obj3, int platformcount, Rectangle platfrom[], Rectangle& box, float& BoxGravSpeed, Rectangle oldBox);

#endif