#include "GameState.h"
#include <iostream>

void GameState::ResetJumpState()
{
    Jumpspeed = Jumpspeed2 = JumpspeedMerged = 0.0f;
    P1CanJump = P2CanJump = MergedCanJump = true;
    merged           = false;
    deathOccured     = false;
    invincibility    = false;
    invincibilityTimer = 1.0f;
    obj3 = { 0, 0 };
}

void GameState::LoadLevel(const json& data, int levelId)
{
    platforms.clear();
    spikes.clear();
    spikeHitboxes.clear();
    hasLift = false;

    for (const auto& lvl : data["Level"])
    {
        if (lvl["id"] != levelId) continue;

        // Spawns
        p1Spawn = Ying = { lvl["players"]["p1Start"]["x"].get<float>(),
                           lvl["players"]["p1Start"]["y"].get<float>() };
        p2Spawn = Yan  = { lvl["players"]["p2Start"]["x"].get<float>(),
                           lvl["players"]["p2Start"]["y"].get<float>() };

        // Platforms
        for (const auto& p : lvl["platforms"])
            platforms.push_back({ p["x"].get<float>(), p["y"].get<float>(),
                                  p["width"].get<float>(), p["height"].get<float>() });

        // Spikes
        for (const auto& s : lvl["spikes"])
        {
            if (!s.contains("vertices")) continue;

            std::array<Vector2, 3> tri;
            for (int i = 0; i < 3; i++)
                tri[i] = { s["vertices"][i]["x"].get<float>(),
                            s["vertices"][i]["y"].get<float>() };
            spikes.push_back(tri);

            spikeHitboxes.push_back({ s["hitbox"]["x"].get<float>(),
                                      s["hitbox"]["y"].get<float>(),
                                      s["hitbox"]["width"].get<float>(),
                                      s["hitbox"]["height"].get<float>() });
        }

        // Exit doors
        exitDoorHitboxYin  = { lvl["exitDoors"]["yin"]["x"].get<float>(),
                               lvl["exitDoors"]["yin"]["y"].get<float>(),
                               lvl["exitDoors"]["yin"]["width"].get<float>(),
                               lvl["exitDoors"]["yin"]["height"].get<float>() };
        exitDoorHitboxYang = { lvl["exitDoors"]["yang"]["x"].get<float>(),
                               lvl["exitDoors"]["yang"]["y"].get<float>(),
                               lvl["exitDoors"]["yang"]["width"].get<float>(),
                               lvl["exitDoors"]["yang"]["height"].get<float>() };

        // Lift (optional)
        if (lvl.contains("lifts"))
        {
            const auto& l = lvl["lifts"];
            currentLift.rect   = { l["x"].get<float>(),    l["y"].get<float>(),
                                   l["width"].get<float>(), l["height"].get<float>() };
            currentLift.button = { l["button"]["x"].get<float>(), l["button"]["y"].get<float>(),
                                   l["button"]["width"].get<float>(), l["button"]["height"].get<float>() };
            currentLift.maxHeight = l["maxHeight"].get<float>();
            currentLift.minHeight = l["minHeight"].get<float>();
            currentLift.speed     = 0.5f;
            hasLift = true;
        }

        currentLevelId = levelId;
        ResetJumpState(); 
        break;
    }
}