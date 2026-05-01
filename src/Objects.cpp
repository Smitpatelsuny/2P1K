#include "raylib.h"
#include "Objects.h"
#include "PlayerStuff.h"
#include "DeaStart.h"
#include <stdio.h>
#include "json.hpp"

using json = nlohmann::json;

void levelLift(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, LiftData& currentLift)
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

void LyftVertControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, float rad,
                      Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                      bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump,
                      float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged,
                      LiftData& currentLift, BoxData& box)
{
   Rectangle liftRect = currentLift.rect;
Rectangle boxRect  = box.ToRect();

    // Lift vertical collision + carry

    PlayerJumpCollisions(Ying, Yan, obj3, rad, &currentLift.rect, 1,
                         P1CanJump, P2CanJump, MergedCanJump,
                         Jumpspeed, Jumpspeed2, JumpspeedMerged,
                         oldP1, oldP2, oldMerged, box.rect);


    if (CheckCollisionCircleRec(Ying, rad + 1, liftRect)) Ying.y -= currentLift.speed;
    if (CheckCollisionCircleRec(Yan,  rad + 1, liftRect)) Yan.y  -= currentLift.speed;
    if (CheckCollisionCircleRec(obj3, rad + 1, liftRect)) obj3.y -= currentLift.speed;

    auto preventPlayerUnderLift = [&](Vector2& p, Vector2& oldP)
    {

    //Players top edge
    float playerTop = p.y - rad;

    if ((playerTop < liftRect.y + liftRect.height) && (playerTop > liftRect.y + liftRect.height - 6) && (p.x > liftRect.x) && (p.x < liftRect.x + liftRect.width))
    {
        //Falls slightly faster than the lift moves
        p.y = oldP.y + currentLift.speed * 2;
    }
    };

    preventPlayerUnderLift(Ying, oldP1);
    preventPlayerUnderLift(Yan, oldP2);
    preventPlayerUnderLift(obj3, oldMerged);

if((boxRect.y + boxRect.height > liftRect.y) && (boxRect.y < liftRect.y) && CheckCollisionRecs(boxRect, liftRect))
{
    box.y = liftRect.y - box.height;
    if (CheckCollisionCircleRec(Ying, rad + 1, boxRect)) Ying.y -= currentLift.speed;
    if (CheckCollisionCircleRec(Yan,  rad + 1, boxRect)) Yan.y  -= currentLift.speed;
    if (CheckCollisionCircleRec(obj3, rad + 1, boxRect)) obj3.y -= currentLift.speed;

    box.gravSpeed = 0.0f;
    box.onGround  = true;

}
}


void LyftHorControls(Vector2& Ying, Vector2& Yan, Vector2& obj3, float rad,
                     Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged,
                     LiftData& currentLift, BoxData& box)
{
    // Lift horizontal collision
    PlayerPlatformCollisions(Ying, rad, &currentLift.rect, 1, Yan, obj3, oldP1, oldP2, oldMerged, box.rect);

    box.prevX = box.x;
    

}

void levelLiftWithBox(Vector2 Ying, Vector2 Yan, Vector2 obj3, float rad, LiftData& currentLift, BoxData& box)
{

    DrawRectangleRec(currentLift.button, RED);
    DrawRectangleRec(currentLift.rect, GRAY);

 

    if (CheckCollisionCircleRec(Ying, rad, currentLift.button) || 
        CheckCollisionCircleRec(Yan,  rad, currentLift.button) ||
        CheckCollisionCircleRec(obj3, rad, currentLift.button) ||
        CheckCollisionRecs(box.ToRect(), currentLift.button))

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


void UpdateVerticalShooter(ShooterData& shooter, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                   float rad, Rectangle platforms[], int platformCount,
                   bool& merged, bool& invincibility, float& invincibilityTimer,
                   bool& restart, Vector2 p1Spawn, Vector2 p2Spawn)
{
    // Draw the shooter base
    DrawRectangleRec(shooter.rect, DARKGRAY);

    // Tick the shoot timer
    shooter.timer += GetFrameTime();

    // Spawn a new projectile when timer exceeds interval and none is active
    if (shooter.timer >= shooter.interval && !shooter.active)
    {
        shooter.active       = true;
        shooter.projectile.x = shooter.rect.x + shooter.rect.width  / 2
                               - shooter.projectile.width / 2;
        if (shooter.direction > 0){
             shooter.projectile.y = shooter.rect.y + shooter.rect.height;
        } // down
        else {
            shooter.projectile.y = shooter.rect.y - shooter.projectile.height;
        }// up
        shooter.timer        = 0.0f;
    }

    if (!shooter.active) return;

    // Move projectile vertically
    shooter.projectile.y += shooter.speed * shooter.direction; // direction is 1 for down, -1 for up

    // Draw projectile
    DrawRectangleRec(shooter.projectile, RED);

    // Deactivate on platform hit
    for (int i = 0; i < platformCount; i++)
    {
        if (CheckCollisionRecs(shooter.projectile, platforms[i]))
        {
            shooter.active = false;
            return;
        }
    }

    if (invincibility) return;

    // Hit unmerged players — respawn at level spawns
    if (CheckCollisionCircleRec(Ying, rad, shooter.projectile) ||
        CheckCollisionCircleRec(Yan,  rad, shooter.projectile))
    {
        Restart(Ying, Yan, obj3, p1Spawn, p2Spawn, merged, restart);
        restart = true;
        shooter.active = false;
        return;
    }

    // Hit merged player — force unmerge with invincibility
    if (CheckCollisionCircleRec(obj3, rad, shooter.projectile))
    {
        merged = false;
        Ying.x = obj3.x - rad;
        Ying.y = obj3.y;
        Yan.x  = obj3.x + rad;
        Yan.y  = obj3.y;
        obj3   = { 0, 0 };

        invincibility      = true;
        invincibilityTimer = 1.0f;
        shooter.active     = false;
    }
}

void UpdateHorizontalShooter(ShooterData& shooter, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                   float rad, Rectangle platforms[], int platformCount,
                   bool& merged, bool& invincibility, float& invincibilityTimer,
                   bool& restart, Vector2 p1Spawn, Vector2 p2Spawn)
{
    // Draw the shooter base
    DrawRectangleRec(shooter.rect, DARKGRAY);

    // Tick the shoot timer
    shooter.timer += GetFrameTime();

    // Spawn a new projectile when timer exceeds interval and none is active
    if (shooter.timer >= shooter.interval && !shooter.active)
    {
        shooter.active       = true;
        if (shooter.direction > 0){
             shooter.projectile.x = shooter.rect.x + shooter.rect.width;
        } // right
        else {
            shooter.projectile.x = shooter.rect.x - shooter.projectile.width;
        }// left
            
    shooter.projectile.y = shooter.rect.y + (shooter.rect.height / 2)
                           - (shooter.projectile.height / 2);
    shooter.timer = 0.0f;
        shooter.timer        = 0.0f;
    }

    if (!shooter.active) return;

    // Move projectile sideways
    shooter.projectile.x += shooter.speed * shooter.direction; // direction is set in level editor — 1 for right, -1 for left

    // Draw projectile
    DrawRectangleRec(shooter.projectile, RED);

    // Deactivate on platform hit
    for (int i = 0; i < platformCount; i++)
    {
        if (CheckCollisionRecs(shooter.projectile, platforms[i]))
        {
            shooter.active = false;
            return;
        }
    }

    if (invincibility) return;

    // Hit unmerged players — respawn at level spawns
    if (CheckCollisionCircleRec(Ying, rad, shooter.projectile) ||
        CheckCollisionCircleRec(Yan,  rad, shooter.projectile))
    {
        Restart(Ying, Yan, obj3, p1Spawn, p2Spawn, merged, restart);
        restart = true;
        shooter.active = false;
        return;
    }

    // Hit merged player — force unmerge with invincibility
    if (CheckCollisionCircleRec(obj3, rad, shooter.projectile))
    {
        merged = false;
        Ying.x = obj3.x - rad;
        Ying.y = obj3.y;
        Yan.x  = obj3.x + rad;
        Yan.y  = obj3.y;
        obj3   = { 0, 0 };

        invincibility      = true;
        invincibilityTimer = 1.0f;
        shooter.active     = false;
    }
}

// Helper to move sentry toward a target position
static void MoveToward(Vector2& sentry, Vector2 target, float speed)
{
    if (sentry.x < target.x) sentry.x += speed;
    else if (sentry.x > target.x) sentry.x -= speed;

    if (sentry.y < target.y) sentry.y += speed;
    else if (sentry.y > target.y) sentry.y -= speed;
}

void UpdateSentry(SentryData& sentry, Vector2& Ying, Vector2& Yan, Vector2& obj3,
                  float rad, bool& merged, bool& invincibility,
                  float& invincibilityTimer, bool& restart,
                  Vector2 p1Spawn, Vector2 p2Spawn)
{
       // Draw sight rectangles 
    DrawRectangleRec(sentry.smallSight, Fade(YELLOW, 0.2f));
    DrawRectangleRec(sentry.largeSight, Fade(ORANGE, 0.1f));

    // Draw sentry
    DrawCircleV(sentry.position, sentry.radius, YELLOW);

    bool p1InSight  = CheckCollisionCircleRec(Ying, rad, sentry.smallSight) ||
                      CheckCollisionCircleRec(Ying, rad, sentry.largeSight);
    bool p2InSight  = CheckCollisionCircleRec(Yan,  rad, sentry.smallSight) ||
                      CheckCollisionCircleRec(Yan,  rad, sentry.largeSight);
    bool merInSight = CheckCollisionCircleRec(obj3, rad, sentry.smallSight) ||
                      CheckCollisionCircleRec(obj3, rad, sentry.largeSight);

    if (p1InSight || p2InSight || merInSight)
    {
        // Chase whichever player is in sight — priority: merged > p1 > p2
        if (merInSight) MoveToward(sentry.position, obj3,  sentry.speed);
        else if (p1InSight) MoveToward(sentry.position, Ying, sentry.speed);
        else if (p2InSight) MoveToward(sentry.position, Yan,  sentry.speed);
    }
    else
    {
        // No player in sight — return to spawn
        MoveToward(sentry.position, sentry.spawnPoint, sentry.speed);
    }

    if (invincibility) return;

    // Hit unmerged players
    if (CheckCollisionCircles(Ying, rad, sentry.position, sentry.radius) ||
        CheckCollisionCircles(Yan,  rad, sentry.position, sentry.radius))
    {
        Restart(Ying, Yan, obj3, p1Spawn, p2Spawn, merged, restart);
        restart = true;
        return;
    }

    // Hit merged player — force unmerge
    if (CheckCollisionCircles(obj3, rad, sentry.position, sentry.radius))
    {
        merged = false;
        Ying.x = obj3.x - rad;
        Ying.y = obj3.y;
        Yan.x  = obj3.x + rad;
        Yan.y  = obj3.y;
        obj3   = { 0, 0 };

        invincibility      = true;
        invincibilityTimer = 1.0f;
    }
}


void UpdateGlassPlatform(GlassPlatformData& gp, Vector2& Ying, Vector2& Yan,
                          Vector2& obj3, float rad, bool merged)
{
    if (gp.broken)
    {
        if (gp.breakTimer > 0.0f)
            gp.breakTimer -= GetFrameTime();
        return;
    }

    bool mergedOnGlass = merged &&
                         CheckCollisionCircleRec(obj3, rad + 1, gp.rect) &&
                         obj3.y < gp.rect.y;

    // testing on unmerged players if needed — currently only breaks from merged player
    // bool p1OnGlass = CheckCollisionCircleRec(Ying, rad, gp.rect) && Ying.y < gp.rect.y;
    // bool p2OnGlass = CheckCollisionCircleRec(Yan,  rad, gp.rect) && Yan.y  < gp.rect.y;

    if (mergedOnGlass)
    {
        gp.broken     = true;
        gp.breakTimer = 3.0f;
    }
}

void DrawGlassPlatform(GlassPlatformData& gp)
{
    if (gp.broken)
    {
        if (gp.breakTimer > 0.0f)
        {
            float alpha = gp.breakTimer / 3.0f;
            Color brokenColor = { 173, 216, 230, (unsigned char)(alpha * 255) };
            float a = 3.0f - gp.breakTimer;

            Rectangle frags[] = {
                { gp.rect.x,       gp.rect.y + a * 60.0f, 35, 8 },
                { gp.rect.x + 40,  gp.rect.y + a * 80.0f, 35, 8 },
                { gp.rect.x + 80,  gp.rect.y + a * 70.0f, 35, 8 },
                { gp.rect.x + 120, gp.rect.y + a * 90.0f, 35, 8 },
                { gp.rect.x + 160, gp.rect.y + a * 65.0f, 35, 8 }
            };

            for (int i = 0; i < 5; i++)
                DrawRectangleRec(frags[i], brokenColor);
        }
        return;
    }

    // Draw intact glass
    Color glassColor = { 173, 216, 230, 150 };
    Color shineColor = { 255, 255, 255, 200 };

    DrawRectangleRec(gp.rect, glassColor);

    Rectangle shine = gp.rect;
    shine.height = 3;
    DrawRectangleRec(shine, shineColor);

    DrawRectangleLinesEx(gp.rect, 1.0f, { 135, 206, 235, 200 });
}


