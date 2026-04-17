#pragma once
#include "raylib.h"
#include "Objects.h"
#include "json.hpp"
#include <vector>
#include <array>
#include <string>
#include <stdio.h>

using json = nlohmann::json;

struct GameState
{
    // ── Screen ────────────────────────────────────────────────────────────
    enum class Screen { TITLE, LEVSEL, GAMEPLAY };
    Screen currentScreen = Screen::TITLE;
    int    currentLevelId = -1;

    // ── Players ───────────────────────────────────────────────────────────
    Vector2 Ying       = { 0, 0 };
    Vector2 Yan        = { 0, 0 };
    Vector2 obj3       = { 0, 0 };
    Vector2 p1Spawn    = { 0, 0 };
    Vector2 p2Spawn    = { 0, 0 };

    float rad            = 20.0f;
    bool  merged         = false;

    // ── Jump state ────────────────────────────────────────────────────────
    float Jumpspeed        = 0.0f;
    float Jumpspeed2       = 0.0f;
    float JumpspeedMerged  = 0.0f;
    bool  P1CanJump        = true;
    bool  P2CanJump        = true;
    bool  MergedCanJump    = true;
    bool Ejected           = false;

    // ── Death / invincibility ─────────────────────────────────────────────
    bool  deathOccured       = false;
    bool  invincibility      = false;
    float invincibilityTimer = 1.0f;

    // ── Level geometry ────────────────────────────────────────────────────
    std::vector<Rectangle>             platforms;
    std::vector<std::array<Vector2,3>> spikes;
    std::vector<Rectangle>             spikeHitboxes;
    std::vector<LiftData>              lifts;
    Rectangle exitDoorHitboxYin  = { 0 };
    Rectangle exitDoorHitboxYang = { 0 };

    // ── Animation Stuff ─────────────────────────────────────────────────────

    bool yingFacingRight = true;
    bool yanFacingRight = true;
    bool mergedFacingRight = true;

    //Doors

        bool       hasDoor   = false;
        DoorData   currentDoor   = {};
 
    // ── Shooter stuff ─────────────────────────────────────────────────────────

        bool       hasShooter = false;
        ShooterData currentShooter = {};

    // ── Lift ──────────────────────────────────────────────────────────────
    bool     hasLift    = false;

    // ── Box ───────────────────────────────────────────────────────────────
    bool    hasBox    = false;      // <-- new
    BoxData currentBox = {};  

    // ── Sentry ───────────────────────────────────────────────────────────────
    /*struct SentryData
    {
        Vector2 pos;            // current position
        int startX;             // starting X (for returning)
        int startY;             // starting Y
        Rectangle smallSight;   // small detection radius
        Rectangle largeSight;   // large detection radius
    };*/

    // ── Methods ───────────────────────────────────────────────────────────
    void LoadLevel(const json& data, int levelId);
    void ResetJumpState();
};