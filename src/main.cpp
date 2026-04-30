#include "raylib.h"
#include "PlayerStuff.h"
#include "DeaStart.h"
#include "Objects.h"
#include "GameState.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

//spike placement cheat sheet 
/*
 //spike parameters: first vetice = (x of platform + 15, y of platform), 
    //second vertice = (x of platform + 55, y of platform), 
    //third vertice = (x of platform + 35, y of platform - 30)
//future spike hitbox: +5 on x (first vertice) and -15 on y from the spike's position, 
    //with a width of 30 and height of 15


*/
int main(void)
{
    const int screenWidth  = 800;
    const int screenHeight = 800;

    json data;
    {
        std::ifstream file("C:/Users/dalma/projects/2P1K/src/Level.json");
        data = json::parse(file);
    }

    GameState gs;   

    InitWindow(screenWidth, screenHeight, "All code test");

    Texture2D background_menu = LoadTexture("C:/Users/dalma/projects/2P1K/src/assets/background_menu.jpg");
    Texture2D gamePlay2       = LoadTexture("C:/Users/dalma/projects/2P1K/src/assets/gamePlay2.jpg");

    AnimState yingState = ANIM_IDLE;
    AnimState yanState = ANIM_IDLE;
    AnimState mergedState = ANIM_IDLE;
    
    
       AnimatedSprite w_player = LoadAnimatedSprite(
        "C:/Users/dalma/projects/2P1K/src/assets/player_w.png",
        1.5f, 3, 4, 0.15f);

     AnimatedSprite b_player = LoadAnimatedSprite(
        "C:/Users/dalma/projects/2P1K/src/assets/Player_b.png",
        1.5f, 3, 4, 0.15f);

    AnimatedSprite m_player = LoadAnimatedSprite(
        "C:/Users/dalma/projects/2P1K/src/assets/player_m.png",
        1.5f, 3, 4, 0.15f);

    
    

    Rectangle startbutton = { 250, 220, 300, 40 };
    Rectangle backbutton  = { 10,  10,  30,  30 };
    Rectangle levelselectbuttons[] = {
        { 150, 200, 100, 50 },
        { 270, 200, 100, 50 },
        { 390, 200, 100, 50 },
        { 510, 200, 100, 50 },
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        
        MergeObjects(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged, gs.invincibility, gs.Jumpspeed2, gs.Ejected);

        // ── Update ───────────────────────────────────────────────────────
        switch (gs.currentScreen)
        {
            case GameState::Screen::TITLE:
            {
                if (IsGestureDetected(GESTURE_TAP) &&
                    CheckCollisionPointRec(GetMousePosition(), startbutton))
                    gs.currentScreen = GameState::Screen::LEVSEL;
            } break;

            case GameState::Screen::LEVSEL:
            {
                if (IsGestureDetected(GESTURE_TAP) &&
                    CheckCollisionPointRec(GetMousePosition(), backbutton))
                {
                    gs.currentScreen = GameState::Screen::TITLE;
                }
                for (int i = 0; i < 4; i++)
                {
                    if (IsGestureDetected(GESTURE_TAP) &&
                        CheckCollisionPointRec(GetMousePosition(), levelselectbuttons[i]))
                    {
                        gs.LoadLevel(data, i + 1);
                        gs.currentScreen = GameState::Screen::GAMEPLAY;
                        break;
                    }
                }
            } break;

            case GameState::Screen::GAMEPLAY:
            {
                if (IsKeyPressed(KEY_P) ||
                    ExitDoors(gs.Ying, gs.Yan,
                              gs.exitDoorHitboxYin, gs.exitDoorHitboxYang, gs.rad, gs.currentLevelId))
                {
                    gs.currentScreen = GameState::Screen::LEVSEL;  
                    break;
                }

                Vector2 oldP1     = gs.Ying;
                Vector2 oldP2     = gs.Yan;
                Vector2 oldMerged = gs.obj3;

                // Movement
                if (gs.merged)
                    MergePlayerMotor(gs.obj3, 2);
                else {
                    PlayerMotor(gs.Ying, 2);
                    Player2Motor(gs.Yan, 2);
                }

                // Horizontal collisions
                    PlayerPlatformCollisions(gs.Ying, gs.rad,
                                             gs.platforms.data(), (int)gs.platforms.size(),
                                             gs.Yan, gs.obj3, oldP1, oldP2, oldMerged, gs.boxRect);
                

                if (gs.hasDoor && !gs.currentDoor.switched)
                    gs.platforms.push_back(gs.currentDoor.doorClosed);
                
                if (gs.hasBox)
                {
                    for (auto& box : gs.boxes){
                    PlayerBoxHorizontalCollisions(gs.Ying, gs.Yan, gs.obj3,
                                                 gs.rad, box,
                                                 gs.platforms.data(), (int)gs.platforms.size(),
                                                 oldP1, oldP2, oldMerged);
                    }
                }
                
             if (gs.hasLift)
             {       

               for (auto& lift : gs.lifts) {
                    for(auto& box : gs.boxes){
                    LyftHorControls(gs.Ying, gs.Yan, gs.obj3, gs.rad,
                    oldP1, oldP2, oldMerged,
                        lift, box);

                     }
              }
                
             }
                // Vertical collisions
                Jump(gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged,
                     gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump,
                     gs.merged, gs.rad, gs.Ying, gs.Yan, gs.obj3, gs.Ejected);


                    
                     PlayerJumpCollisions(gs.Ying, gs.Yan, gs.obj3, gs.rad,
                                     gs.platforms.data(), (int)gs.platforms.size(),
                                     gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump,
                                     gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged,
                                     oldP1, oldP2, oldMerged, gs.boxRect);
                
               
        
                if (gs.hasBox)
                {
                    for (auto& box : gs.boxes){
                        PlayerBoxCollisions(gs.Ying, gs.rad, gs.Yan, gs.obj3,    // gravity only
                        (int)gs.platforms.size(), gs.platforms.data(),
                        box);
                        PlayerBoxVerticalCollisions(gs.Ying, gs.Yan, gs.obj3, gs.rad, box,
                                 gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump,
                                 gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged,
                                 oldP1, oldP2, oldMerged);
                    }
                    
                }
                if (gs.hasLift)
                {
                   for (auto& lift : gs.lifts)
                    {
                        for (auto& box : gs.boxes)
                        {
                            LyftVertControls(gs.Ying, gs.Yan, gs.obj3, gs.rad,
                                oldP1, oldP2, oldMerged,
                                gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump,
                                gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged,
                                lift, box);
                        }
                    }
                }
             
       
            if (!gs.merged)
            {
                if (IsKeyDown(KEY_D))
                    gs.yingFacingRight = true;
                if (IsKeyDown(KEY_A))
                    gs.yingFacingRight = false;

                if (IsKeyDown(KEY_RIGHT))
                    gs.yanFacingRight = true;
                if (IsKeyDown(KEY_LEFT))
                    gs.yanFacingRight = false;

                if (!gs.P1CanJump)
                {
                    if (gs.Jumpspeed > 0)
                        yingState = ANIM_JUMP;
                    else
                        yingState = ANIM_FALL;
                }
                else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
                {
                    yingState = ANIM_RUN;
                }
                else
                {
                    yingState = ANIM_IDLE;
                }

                if (!gs.P2CanJump)
                {
                    if (gs.Jumpspeed2 > 0)
                        yanState = ANIM_JUMP;
                    else
                        yanState = ANIM_FALL;
                }
                else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
                {
                    yanState = ANIM_RUN;
                }
                else
                {
                    yanState = ANIM_IDLE;
                }

                UpdateAnimatedSprite(w_player, yingState);
                UpdateAnimatedSprite(b_player, yanState);
            }
            else
            {
                if (IsKeyDown(KEY_D))
                    gs.mergedFacingRight = true;
                if (IsKeyDown(KEY_A))
                    gs.mergedFacingRight = false;

                if (!gs.MergedCanJump)
                {
                    if (gs.JumpspeedMerged > 0)
                        mergedState = ANIM_JUMP;
                    else
                        mergedState = ANIM_FALL;
                }
                else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
                {
                    mergedState = ANIM_RUN;
                }
                else
                {
                    mergedState = ANIM_IDLE;
                }

                UpdateAnimatedSprite(m_player, mergedState);
            }

        if (gs.hasDoor && !gs.currentDoor.switched)
                gs.platforms.pop_back();

        if (gs.hasDoor){
            UpdateDoor(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.currentDoor);
        }

            } break;
        }

        for (const Rectangle& spike : gs.spikeHitboxes)
        {
            // Ying
            if (CheckCollisionCircleRec(gs.Ying, gs.rad, spike) ||CheckCollisionCircleRec(gs.Yan, gs.rad, spike) )
            {
                death(gs.Ying, gs.Yan, gs.p1Spawn, gs.p2Spawn, gs.invincibility, gs.invincibilityTimer, gs.restart);
                break;
            }

            // Yan
            if (CheckCollisionCircleRec(gs.Yan, gs.rad, spike))
            {
                death(gs.Ying, gs.Yan, gs.p1Spawn, gs.p2Spawn, gs.invincibility, gs.invincibilityTimer, gs.restart);
                break;
            }
                // Merged
            if (gs.merged && CheckCollisionCircleRec(gs.obj3, gs.rad, spike))
            {
                forceUnmerge(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged, gs.invincibility, gs.invincibilityTimer);
                break;
            }
        }

        if (gs.hasVerticalShooter)
                for (auto& shooter : gs.vshooters)
                    UpdateVerticalShooter(shooter, gs.Ying, gs.Yan, gs.obj3,
                   gs.rad, gs.platforms.data(), (int)gs.platforms.size(),
                   gs.merged, gs.invincibility, gs.invincibilityTimer,
                   gs.restart, gs.p1Spawn, gs.p2Spawn);

        if (gs.hasHorizontalShooter)
                for (auto& shooter : gs.hshooters)
                    UpdateHorizontalShooter(shooter, gs.Ying, gs.Yan, gs.obj3,
                   gs.rad, gs.platforms.data(), (int)gs.platforms.size(),
                   gs.merged, gs.invincibility, gs.invincibilityTimer,
                   gs.restart, gs.p1Spawn, gs.p2Spawn);
        
        if(gs.hasSentry)
            for (auto& sentry : gs.sentries)
                UpdateSentry(sentry, gs.Ying, gs.Yan, gs.obj3,
                  gs.rad, gs.merged, gs.invincibility,
                  gs.invincibilityTimer, gs.restart,
                  gs.p1Spawn, gs.p2Spawn);

        // ── Draw ─────────────────────────────────────────────────────────
        BeginDrawing();

        switch (gs.currentScreen)
        {
            case GameState::Screen::TITLE:
            {
                ClearBackground(BLACK);
                if (background_menu.width > 0)
                    DrawTexturePro(background_menu,
                        { 0, 0, (float)background_menu.width, (float)background_menu.height },
                        { 0, 0, (float)screenWidth, (float)screenHeight },
                        { 0, 0 }, 0.0f, WHITE);

                int startSize  = 20;
                int startWidth = MeasureText("START", startSize);
                DrawText("Ying and Yan",      150, 20,  80, WHITE);
                DrawText("The Second Coming", 200, 100, 40, WHITE);
                DrawRectangleRec(startbutton, RED);
                DrawText("START",
                    (int)(startbutton.x + startbutton.width  / 2 - startWidth / 2),
                    (int)(startbutton.y + startbutton.height / 2 - startSize  / 2),
                    startSize, LIGHTGRAY);
            } break;

            case GameState::Screen::LEVSEL:
            {
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("LEVEL SELECT", 150, 40, 60, WHITE);
                for (int i = 0; i < 4; i++)
                {
                    DrawRectangleRec(levelselectbuttons[i], DARKGRAY);
                    char label[8];
                    snprintf(label, sizeof(label), "Lv %d", i + 1);
                    DrawText(label,
                        (int)(levelselectbuttons[i].x + 20),
                        (int)(levelselectbuttons[i].y + 15),
                        20, WHITE);
                }
                DrawRectangleRec(backbutton, WHITE);
            } break;

            case GameState::Screen::GAMEPLAY:
            {
                ClearBackground(SKYBLUE);
                DrawTexturePro(gamePlay2,
                    { 0, 0, (float)gamePlay2.width, (float)gamePlay2.height },
                    { 0, 0, (float)screenWidth, (float)screenHeight },
                    { 0, 0 }, 0.0f, WHITE);

                ExitGame();

                InvisCountDown(gs.invincibility, gs.invincibilityTimer);
                Restart(gs.Ying, gs.Yan, gs.obj3, gs.p1Spawn, gs.p2Spawn, gs.merged, gs.restart);

                
         if (gs.hasLift && gs.hasBox)
                {
                    for (auto& lift : gs.lifts)
                        for (auto& box : gs.boxes){
                        levelLiftWithBox(gs.Ying, gs.Yan, gs.obj3, gs.rad, lift, box);
                        }
                    }else if (gs.hasLift){
                    for (auto& lift : gs.lifts)
                        levelLift(gs.Ying, gs.Yan, gs.obj3, gs.rad, lift);
                }
                   

                if (gs.hasBox)
                {
                    for (auto& box : gs.boxes){
                        PlayerBoxCollisions(gs.Ying, gs.rad, gs.Yan, gs.obj3,    // gravity only
                        (int)gs.platforms.size(), gs.platforms.data(),
                        box);
                    }
                }
                

        if (gs.hasDoor){
            UpdateDoor(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.currentDoor);
        }

               if (gs.hasVerticalShooter){
                    for (auto& shooter : gs.vshooters)
                        UpdateVerticalShooter(shooter, gs.Ying, gs.Yan, gs.obj3,
                       gs.rad, gs.platforms.data(), (int)gs.platforms.size(),
                       gs.merged, gs.invincibility, gs.invincibilityTimer,
                       gs.restart, gs.p1Spawn, gs.p2Spawn);
                   }
               
            if (gs.hasHorizontalShooter){
                        for (auto& shooter : gs.hshooters){ 
                            UpdateHorizontalShooter(shooter, gs.Ying, gs.Yan, gs.obj3,
                        gs.rad, gs.platforms.data(), (int)gs.platforms.size(),
                        gs.merged, gs.invincibility, gs.invincibilityTimer,
                        gs.restart, gs.p1Spawn, gs.p2Spawn);
                        }
                    }
             
                      
        if(gs.hasSentry){
            for (auto& sentry : gs.sentries)
                UpdateSentry(sentry, gs.Ying, gs.Yan, gs.obj3,
                  gs.rad, gs.merged, gs.invincibility,
                  gs.invincibilityTimer, gs.restart,
                  gs.p1Spawn, gs.p2Spawn);
        }
            

/*IDk where else to put this, and it's 100% going to get lost here... but
for the button on the left side for the rigth door {
          "x": 120,
          "y": 675,
          "width": 10,
          "height": 5
    }
*/

        if(gs.restart){
            gs.LoadLevel(*gs.levelData, gs.currentLevelId);
            gs.restart = false;
        }


           if (!gs.merged)
            {
                
                DrawAnimatedSpriteCentered(w_player, gs.Ying, gs.yingFacingRight, WHITE);

                DrawAnimatedSpriteCentered(b_player, gs.Yan, gs.yanFacingRight, WHITE);
            }
            else
            {
                DrawAnimatedSpriteCentered(m_player, gs.obj3, gs.mergedFacingRight, WHITE);
            }

             
                for (const auto& p : gs.platforms)
                    DrawRectangleRec(p, DARKGRAY);

                for (const auto& s : gs.spikes)
                    DrawTriangle(s[0], s[1], s[2], RED);

                DrawRectangleRec(gs.exitDoorHitboxYin,  GREEN);
                DrawRectangleRec(gs.exitDoorHitboxYang, GREEN);
            } break;

            default: break;
        }

        EndDrawing();
    }


    UnloadAnimatedSprite(w_player);
    UnloadAnimatedSprite(b_player);
    UnloadAnimatedSprite(m_player);


    UnloadTexture(background_menu);
    UnloadTexture(gamePlay2);
    CloseWindow();
    return 0;
    
}

