#ifndef OBJECTS_H
#define OBJECTS_H

#include <raylib.h>
#include <raymath.h>

struct LiftData {
    Rectangle rect;
    Rectangle button;
    float maxHeight;
    float minHeight;
    float speed;
};

void levelOne(Vector2 Ying, Vector2 Yan, Vector2 obj3, int rad, LiftData &currentLift);
void LyftHorControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, LiftData &currentLift, bool merged);
void LyftVertControls(Vector2 &Ying, Vector2 &Yan, Vector2 &obj3, int rad, Vector2 &oldP1, Vector2 &oldP2, Vector2 &oldMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, LiftData &currentLift);

#endif