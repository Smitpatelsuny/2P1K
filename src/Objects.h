#ifndef OBJECTS_H
#define OBJECTS_H

#include <raylib.h>
#include <raymath.h>
#include <vector>

struct LiftData {
    Rectangle rect;
    Rectangle button;
    float maxHeight;
    float minHeight;
    float speed;
     Vector2 startposition = { 0, 0 };
};

struct BoxData {
    Rectangle rect;
    float x, y, width, height;
    float prevX = 0.0f;
    float gravSpeed;
    bool onGround = false;
    Rectangle ToRect() const { return { x, y, width, height }; }
    float prevY = 0.0f;
    Vector2 startposition = { 0, 0 };
};

struct DoorData {
    // Lever
    bool      switched     = false;
    Rectangle leverBase    = { 0 };
    Rectangle hitboxLeft   = { 0 };
    Rectangle hitboxRight  = { 0 };
    Rectangle spriteOff    = { 0 };
    Rectangle spriteOn     = { 0 };

    // Door
    Rectangle doorClosed   = { 0 };
    Rectangle doorOpen     = { 0 };
    Rectangle doorFrame[2] = { { 0 }, { 0 } };
};


struct ShooterData {
    Rectangle rect       = { 0 };
    Rectangle projectile = { 0 };
    float     speed      = 3.0f;
    float     interval   = 2.0f;
    float     timer      = 0.0f;   
    bool      active     = false;
    float     direction  = 1.0f; // 1 for right/down, -1 for left/up
};

struct SentryData {
    Vector2   position    = { 0, 0 };
    Vector2   spawnPoint  = { 0, 0 };  // where it returns to when idle
    Rectangle smallSight  = { 0 };
    Rectangle largeSight  = { 0 };
    float     speed       = 0.5f;
    float     radius      = 5.0f;
};

struct GlassPlatformData {
    Rectangle rect       = { 0 };
    bool      broken     = false;
    float     breakTimer = 0.0f;
};

void levelLift(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, LiftData& lift);

void LyftHorControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, float rad,
                     Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                     LiftData& lift, const std::vector<BoxData>& box);

void LyftVertControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, float rad,
                      Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                      bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                      float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                      LiftData& lift, BoxData& box);


void levelLiftWithBox(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, LiftData& currentLift, BoxData& box);

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


void UpdateDoor(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, DoorData& ld);


void UpdateVerticalShooter(ShooterData& shooter, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                   float rad, Rectangle platforms[], int platformCount,
                   bool& merged, bool& invincibility, float& invincibilityTimer,
                   bool& restart, Vector2 p1Spawn, Vector2 p2Spawn);

void UpdateHorizontalShooter(ShooterData& shooter, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                   float rad, Rectangle platforms[], int platformCount,
                   bool& merged, bool& invincibility, float& invincibilityTimer,
                   bool& restart, Vector2 p1Spawn, Vector2 p2Spawn);

static void MoveToward(Vector2& sentry, Vector2 target, float speed);

void UpdateSentry(SentryData& sentry, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                  float rad, bool& merged, bool& invincibility,
                  float& invincibilityTimer, bool& restart,
                  Vector2 p1Spawn, Vector2 p2Spawn);

void UpdateGlassPlatform(GlassPlatformData& gp, Vector2& Ying, Vector2& Yan,
                          Vector2& obj3, float rad, bool merged);
void DrawGlassPlatform(GlassPlatformData& gp);

#endif