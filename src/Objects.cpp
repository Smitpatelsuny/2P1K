#include "raylib.h"
#include "Objects.h"
#include "PlayerStuff.h"
#include <stdio.h>
#include "json.hpp"

using json = nlohmann::json;

void levelLift(Vector2 Ying, Vector2 Yan, Vector2 obj3, int rad, LiftData& currentLift)
{
    
    DrawRectangleRec(currentLift.button, RED);
    DrawRectangleRec(currentLift.rect, GRAY);

 
      
    if (CheckCollisionCircleRec(Ying, rad, currentLift.button) ||
        CheckCollisionCircleRec(Yan,  rad, currentLift.button) ||
        CheckCollisionCircleRec(obj3, rad, currentLift.button))
    {
        if (currentLift.rect.y < currentLift.maxHeight)
            currentLift.rect.y += currentLift.speed;
    }
    else
    {
        if (currentLift.rect.y > currentLift.minHeight)
            currentLift.rect.y -= currentLift.speed;
    }
}

void LyftVertControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, int rad,
                      Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                      bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                      float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                      LiftData& currentLift, BoxData& box)
{
    // Lift vertical collision + carry
    PlayerJumpCollisions(Ying, Yan, obj3, rad, &currentLift.rect, 1,
                         P1CanJump, P2CanJump, MergedCanJump,
                         Jumpspeed, Jumpspeed2, JumpspeedMerged,
                         oldP1, oldP2, oldMerged, box.rect);

    if (CheckCollisionCircleRec(Ying, rad + 1, currentLift.rect)) Ying.y -= currentLift.speed;
    if (CheckCollisionCircleRec(Yan,  rad + 1, currentLift.rect)) Yan.y  -= currentLift.speed;
    if (CheckCollisionCircleRec(obj3, rad + 1, currentLift.rect)) obj3.y -= currentLift.speed;

}

void PlayerBoxHorizontalCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3,
                                    float rad, BoxData& box,
                                    Rectangle platforms[], int platformCount,
                                    Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged)
{
    box.prevX = box.x;

    Rectangle boxRect = box.ToRect();
    float boxTop = boxRect.y;

    // Check directly: is the player's bottom edge sitting at the box top?
    // Use a small tolerance band instead of relying on old positions
    auto isOnTop = [&](Vector2 pos) -> bool {
        float playerBottom = pos.y + rad;
        bool withinXBounds = (pos.x + rad > boxRect.x) && (pos.x - rad < boxRect.x + boxRect.width);
        bool atTopSurface  = (playerBottom >= boxTop - 4.0f) && (playerBottom <= boxTop + 8.0f);
        return withinXBounds && atTopSurface;
    };

    bool p1OnTop     = isOnTop(player);
    bool p2OnTop     = isOnTop(ballPosition);
    bool mergedOnTop = isOnTop(obj3);

    // Push logic — only if hitting the side, not standing on top
    if (CheckCollisionCircleRec(player, rad, boxRect) && !p1OnTop)
    {
        float dir = (player.x < box.x) ? 2.0f : -2.0f;
        if (CanMoveBox(box, dir, 0, platforms, platformCount))
            box.x += dir;
        else
            player.x = oldP1.x;
    }

    if (CheckCollisionCircleRec(ballPosition, rad, boxRect) && !p2OnTop)
    {
        float dir = (ballPosition.x < box.x) ? 2.0f : -2.0f;
        if (CanMoveBox(box, dir, 0, platforms, platformCount))
            box.x += dir;
        else
            ballPosition.x = oldP2.x;
    }

    if (CheckCollisionCircleRec(obj3, rad, boxRect) && !mergedOnTop)
    {
        float dir = (obj3.x < box.x) ? 2.0f : -2.0f;
        if (CanMoveBox(box, dir, 0, platforms, platformCount))
            box.x += dir;
        else
            obj3.x = oldMerged.x;
    }

    // Carry players that are on top by however far the box moved
    float boxDelta = box.x - box.prevX;
    if (boxDelta != 0.0f)
    {
        // Re-check isOnTop with updated box position
        if (isOnTop(player)) player.x  += boxDelta;
        if (isOnTop(ballPosition)) ballPosition.x += boxDelta;
        if (isOnTop(obj3))  obj3.x  += boxDelta;
    }
}

void PlayerBoxVerticalCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3,
                                  float rad, BoxData& box,
                                  bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                                  float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                                  Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged)
{
    Rectangle boxRect = box.ToRect();
    float boxTop = boxRect.y;

    // Player 1
    if (CheckCollisionCircleRec(player, rad, boxRect))
    {
        player.y = oldP1.y;
        // Coming from above = landing on top
        if (oldP1.y + rad <= boxTop + 5.0f)
        {
            P1CanJump = true;
            Jumpspeed = 0.0f;
        }
    }

    // Player 2
    if (CheckCollisionCircleRec(ballPosition, rad, boxRect))
    {
        ballPosition.y = oldP2.y;
        if (oldP2.y + rad <= boxTop + 5.0f)
        {
            P2CanJump = true;
            Jumpspeed2 = 0.0f;
        }
    }

    // Merged
    if (CheckCollisionCircleRec(obj3, rad, boxRect))
    {
        obj3.y = oldMerged.y;
        if (oldMerged.y + rad <= boxTop + 5.0f)
        {
            MergedCanJump = true;
            JumpspeedMerged = 0.0f;
        }
    }
}


void PlayerBoxCollisions(Vector2& player, float rad,
                         Vector2& ballPosition, Vector2& obj3,
                         int platformCount, Rectangle platforms[],
                         BoxData& box)
{
    DrawRectangleRec(box.ToRect(), BROWN);

    Rectangle oldBox = box.ToRect();

    // Check if box is resting on ANY platform before applying gravity
    bool onAnyPlatform = false;
    for (int i = 0; i < platformCount; i++)
    {
        if (CheckCollisionRecs(box.ToRect(), platforms[i]))
        {
            onAnyPlatform = true;
            break;  // stop at first hit — don't keep looping
        }
    }

    if (!onAnyPlatform)
    {
        if (box.gravSpeed >= -0.8f) 
        {
            box.gravSpeed = -0.8f;
        }

        box.y -= box.gravSpeed;
        box.gravSpeed -= 0.1f;
        box.onGround = false;

        // After moving, snap back if we landed on something
        for (int i = 0; i < platformCount; i++)
        {
            if (CheckCollisionRecs(box.ToRect(), platforms[i]))
            {
                box.x = oldBox.x;
                box.y = oldBox.y;
                box.gravSpeed = 0.0f;
                box.onGround  = true;
                break;
            }
        }
    }
    else
    {
        box.gravSpeed = 0.0f;
        box.onGround  = true;
    }
}

bool CanMoveBox(BoxData& box, float dx, float dy, Rectangle platforms[], int platformCount)
{
    Rectangle moved = { box.x + dx, box.y + dy, box.width, box.height };
    for (int i = 0; i < platformCount; i++)
        if (CheckCollisionRecs(moved, platforms[i]))
        {
            return false;
        }
            
    return true;
}

void LyftHorControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, int rad,
                     Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                     LiftData& currentLift, BoxData& box)
{
    // Lift horizontal collision
    PlayerPlatformCollisions(Ying, rad, &currentLift.rect, 1, Yan, obj3, oldP1, oldP2, oldMerged, box.rect);

}


void UpdateDoor(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, DoorData& ld)
{
    // Draw lever base
    DrawRectangleRec(ld.leverBase, DARKGRAY);

    // Draw correct lever sprite based on state
    DrawRectangleRec(ld.switched ? ld.spriteOn : ld.spriteOff, BROWN);

    // Toggle lever when any player touches either hitbox
    bool touchingLeft  = CheckCollisionCircleRec(Ying, rad, ld.hitboxLeft)  ||
                         CheckCollisionCircleRec(Yan,  rad, ld.hitboxLeft)  ||
                         CheckCollisionCircleRec(obj3, rad, ld.hitboxLeft);

    bool touchingRight = CheckCollisionCircleRec(Ying, rad, ld.hitboxRight) ||
                         CheckCollisionCircleRec(Yan,  rad, ld.hitboxRight) ||
                         CheckCollisionCircleRec(obj3, rad, ld.hitboxRight);

    if (touchingLeft && !ld.switched)  ld.switched = true;
    if (touchingRight && ld.switched)  ld.switched = false;

    // Draw door frame (always visible)
    for (int i = 0; i < 2; i++)
        DrawRectangleRec(ld.doorFrame[i], DARKGRAY);

    // Draw door — open or closed depending on lever state
    if (ld.switched)
        DrawRectangleRec(ld.doorOpen,   BLANK);     // open = invisible/passable
    else
        DrawRectangleRec(ld.doorClosed, DARKBROWN); // closed = solid
}





