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
    levelData = &data;
    platforms.clear();
    spikes.clear();
    spikeHitboxes.clear();
    lifts.clear();
    boxes.clear();
    vshooters.clear();
    hshooters.clear();
    sentries.clear();
    glassPlatforms.clear();
    hasLift = false;                          
    hasBox  = false;                        
    hasDoor    = false;
    currentDoor = {};
    hasVerticalShooter = false;
    hasHorizontalShooter = false;
    hasSentry = false;
    hasGlassPlatforms = false;


    for (const auto& lvl : data["Level"])
    {
        if (lvl["id"] != levelId) continue;

        // Spawns
        this -> p1Spawn = this -> Ying = { lvl["players"]["p1Start"]["x"].get<float>(),
                           lvl["players"]["p1Start"]["y"].get<float>() };
        this -> p2Spawn = this -> Yan  = { lvl["players"]["p2Start"]["x"].get<float>(),
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

        // Lift 
        if (lvl.contains("lifts"))
    {
        for (const auto& l : lvl["lifts"])
        {
        LiftData ld;
        ld.rect      = { l["x"].get<float>(),      l["y"].get<float>(),
                         l["width"].get<float>(),   l["height"].get<float>() };
        ld.startposition = { ld.rect.x, ld.rect.y };
        ld.button    = { l["button"]["x"].get<float>(), l["button"]["y"].get<float>(),
                         l["button"]["width"].get<float>(), l["button"]["height"].get<float>() };
        ld.maxHeight = l["maxHeight"].get<float>();
        ld.minHeight = l["minHeight"].get<float>();
        ld.speed     = 0.5f;
        lifts.push_back(ld);
        hasLift = true;
        
     }
    }

    //Boxes
    if (lvl.contains("Boxes"))
    {
     for (const auto& b : lvl["Boxes"])
     {
        BoxData newBox;
        newBox.x      = b["x"].get<float>();
        newBox.y      = b["y"].get<float>();
        newBox.startposition = { newBox.x, newBox.y };
        newBox.width  = b["width"].get<float>();
        newBox.height = b["height"].get<float>();
        newBox.onGround  = false;
        newBox.gravSpeed = 0.0f;
        boxes.push_back(newBox);
        hasBox = true;
    }
    }

    //Doors
    if (lvl.contains("Doors"))
    {
        const auto& ld = lvl["Doors"];
        const auto& lv = ld["lever"];
        const auto& d  = ld["door"];

    // Lever
    currentDoor.switched    = lv["switched"].get<bool>();
    currentDoor.leverBase   = { lv["base"]["x"].get<float>(),
                                     lv["base"]["y"].get<float>(),
                                     lv["base"]["width"].get<float>(),
                                     lv["base"]["height"].get<float>() };
    currentDoor.hitboxLeft  = { lv["hitboxLeft"]["x"].get<float>(),
                                     lv["hitboxLeft"]["y"].get<float>(),
                                     lv["hitboxLeft"]["width"].get<float>(),
                                     lv["hitboxLeft"]["height"].get<float>() };
    currentDoor.hitboxRight = { lv["hitboxRight"]["x"].get<float>(),
                                     lv["hitboxRight"]["y"].get<float>(),
                                     lv["hitboxRight"]["width"].get<float>(),
                                     lv["hitboxRight"]["height"].get<float>() };
    currentDoor.spriteOff   = { lv["spriteOff"]["x"].get<float>(),
                                     lv["spriteOff"]["y"].get<float>(),
                                     lv["spriteOff"]["width"].get<float>(),
                                     lv["spriteOff"]["height"].get<float>() };
    currentDoor.spriteOn    = { lv["spriteOn"]["x"].get<float>(),
                                     lv["spriteOn"]["y"].get<float>(),
                                     lv["spriteOn"]["width"].get<float>(),
                                     lv["spriteOn"]["height"].get<float>() };

    // Door
    currentDoor.doorClosed  = { d["closed"]["x"].get<float>(),
                                     d["closed"]["y"].get<float>(),
                                     d["closed"]["width"].get<float>(),
                                     d["closed"]["height"].get<float>() };
    currentDoor.doorOpen    = { d["open"]["x"].get<float>(),
                                     d["open"]["y"].get<float>(),
                                     d["open"]["width"].get<float>(),
                                     d["open"]["height"].get<float>() };
    for (int i = 0; i < 2; i++)
        currentDoor.doorFrame[i] = { d["frame"][i]["x"].get<float>(),
                                          d["frame"][i]["y"].get<float>(),
                                          d["frame"][i]["width"].get<float>(),
                                          d["frame"][i]["height"].get<float>() };
     hasDoor = true;
    }
    
    if (lvl.contains("VerticalShooters"))
    {
        const auto& s = lvl["VerticalShooters"];
        for (const auto& sh : s)
        {
            ShooterData newShooter;
            newShooter.rect       = { sh["rect"]["x"].get<float>(),
                                      sh["rect"]["y"].get<float>(),
                                      sh["rect"]["width"].get<float>(),
                                      sh["rect"]["height"].get<float>() };
            newShooter.projectile = { sh["projectile"]["x"].get<float>(),
                                  sh["projectile"]["y"].get<float>(),
                                  sh["projectile"]["width"].get<float>(),
                                  sh["projectile"]["height"].get<float>() };
            newShooter.speed    = sh["speed"].get<float>();
            newShooter.interval = sh["interval"].get<float>();
            newShooter.timer    = 0.0f;   // always reset on load
            newShooter.active   = sh["active"].get<bool>();
             newShooter.direction = sh["direction"].get<float>();
            vshooters.push_back(newShooter);
        }
        hasVerticalShooter = true;
    }


    if (lvl.contains("HorizontalShooters"))
    {
        const auto& s = lvl["HorizontalShooters"];
        for (const auto& sh : s)
        {
            ShooterData newShooter;
            newShooter.rect       = { sh["rect"]["x"].get<float>(),
                                      sh["rect"]["y"].get<float>(),
                                      sh["rect"]["width"].get<float>(),
                                      sh["rect"]["height"].get<float>() };
            newShooter.projectile = { sh["projectile"]["x"].get<float>(),
                                  sh["projectile"]["y"].get<float>(),
                                  sh["projectile"]["width"].get<float>(),
                                  sh["projectile"]["height"].get<float>() };
            newShooter.speed    = sh["speed"].get<float>();
            newShooter.interval = sh["interval"].get<float>();
            newShooter.timer    = 0.0f;   // always reset on load
            newShooter.active   = sh["active"].get<bool>();
            newShooter.direction = sh["direction"].get<float>();
            hshooters.push_back(newShooter);
        }
        hasHorizontalShooter = true;
    }

if (lvl.contains("sentries"))
{
    for (const auto& s : lvl["sentries"])
    {
        SentryData sd;
        sd.position   = { s["x"].get<float>(), s["y"].get<float>() };
        sd.spawnPoint = sd.position;  // spawn = starting position
        sd.speed      = s["speed"].get<float>();
        sd.radius     = s["radius"].get<float>();
        sd.smallSight = { s["smallSight"]["x"].get<float>(),
                          s["smallSight"]["y"].get<float>(),
                          s["smallSight"]["width"].get<float>(),
                          s["smallSight"]["height"].get<float>() };
        sd.largeSight = { s["largeSight"]["x"].get<float>(),
                          s["largeSight"]["y"].get<float>(),
                          s["largeSight"]["width"].get<float>(),
                          s["largeSight"]["height"].get<float>() };
        sentries.push_back(sd);
    }
    hasSentry = true;  
}

if (lvl.contains("glassPlatforms"))
{
    for (const auto& g : lvl["glassPlatforms"])
    {
        GlassPlatformData gp;
        gp.rect        = { g["x"].get<float>(),     g["y"].get<float>(),
                           g["width"].get<float>(),  g["height"].get<float>() };
        gp.broken      = false;
        gp.breakTimer  = 0.0f;
        glassPlatforms.push_back(gp);
    }
     hasGlassPlatforms = true;
}

        currentLevelId = levelId;
        ResetJumpState(); 
        break;
    }
}


