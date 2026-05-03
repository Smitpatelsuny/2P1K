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
    enum class Screen { TITLE, LEVSEL, GAMEPLAY};
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
    bool restart          = false;

    // ── Level geometry ────────────────────────────────────────────────────
    std::vector<Rectangle>             platforms;
    std::vector<std::array<Vector2,3>> spikes;
    std::vector<Rectangle>             spikeHitboxes;
    std::vector<LiftData>              lifts;
    std::vector<ShooterData>           vshooters;
    std::vector<ShooterData>           hshooters;
    std::vector<BoxData>               boxes;
    std::vector<SentryData>            sentries;
    std::vector<GlassPlatformData> glassPlatforms;
    std::vector<DoorData>               doors;
    Rectangle exitDoorHitboxYin  = { 0 };
    Rectangle exitDoorHitboxYang = { 0 };

    // ── Animation Stuff ─────────────────────────────────────────────────────

    bool yingFacingRight = true;
    bool yanFacingRight = true;
    bool mergedFacingRight = true;

    //Doors

        bool       hasDoor   = false;
        
 
    // ── Shooter stuff ─────────────────────────────────────────────────────────

        bool       hasVerticalShooter = false;
        bool       hasHorizontalShooter = false;
    


    // ── Lift ──────────────────────────────────────────────────────────────
    bool     hasLift    = false;

    // ── Box ───────────────────────────────────────────────────────────────
    bool    hasBox    = false; 
    Rectangle boxRect = boxes.empty() ? Rectangle{0, 0, 0, 0} : boxes[0].ToRect();


    // ── Sentry ───────────────────────────────────────────────────────────────
   
     bool    hasSentry = false;

// ── Glass ───────────────────────────────────────────────────────────────
     bool    hasGlassPlatforms = false;

    // ── Methods ───────────────────────────────────────────────────────────
    void LoadLevel(const json& data, int levelId);
    void ResetJumpState();
    void ReloadLevel();
    
    
    const json* levelData = nullptr; 
};