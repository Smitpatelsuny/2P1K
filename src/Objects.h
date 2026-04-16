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

struct BoxData {
    Rectangle rect;
    float x, y, width, height;
    float prevX = 0.0f;
    float gravSpeed;
    bool onGround = false;
    Rectangle ToRect() const { return { x, y, width, height }; }
};


void levelLift(Vector2 Ying, Vector2 Yan, Vector2 obj3, int rad, LiftData& lift);

void LyftHorControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, int rad,
                     Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                     LiftData& lift, BoxData& box);

void LyftVertControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, int rad,
                      Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                      bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                      float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                      LiftData& lift, BoxData& box);

void PlayerBoxVerticalCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3,
                                  float rad, BoxData& box,
                                  bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                                  float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                                  Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged);

void PlayerBoxHorizontalCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3,
                                    float rad, BoxData& box,
                                    Rectangle platforms[], int platformCount,
                                    Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged);

void PlayerBoxCollisions(Vector2& player, float rad,
                         Vector2& ballPosition, Vector2& obj3,
                         int platformCount, Rectangle platforms[],
                         BoxData& box);

bool CanMoveBox(BoxData& box, float dx, float dy, Rectangle platforms[], int platformCount);
#endif