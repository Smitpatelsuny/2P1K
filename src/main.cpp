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
    bool paused = false;
    int window = 2;
    Vector2 mouse = GetMousePosition();

    json data;
    {
        std::ifstream file("src/Level.json");
        data = json::parse(file);
    }

    GameState gs;   

    InitWindow(1000, 800, "Ying & Yan the Second Coming");

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);


    Texture2D background_menu = LoadTexture("assets/background_menu.jpg");
    Texture2D gamePlay2       = LoadTexture("assets/gamePlay2.jpg");

    AnimState yingState = ANIM_IDLE;
    AnimState yanState = ANIM_IDLE;
    AnimState mergedState = ANIM_IDLE;
    
    
       AnimatedSprite w_player = LoadAnimatedSprite(
        "assets/player_w.png",
        1.5f, 3, 4, 0.15f);

     AnimatedSprite b_player = LoadAnimatedSprite(
        "C:/Users/austi/projects/New2P1K/2P1K-2/assets/Player_b.png",
        1.5f, 3, 4, 0.15f);

    AnimatedSprite m_player = LoadAnimatedSprite(
        "assets/player_m.png",
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
      if(true) ToggleFullscreen();

    while (!WindowShouldClose())
    {
            // Compute scale + offsets
    float scaleX = (float)GetScreenWidth()  / screenWidth;
    float scaleY = (float)GetScreenHeight() / screenHeight;
    float scale  = fmin(scaleX, scaleY);

    // Smooth scaling fix: prevent fractional rounding clipping
    scale = floorf(scale * 100.0f) / 100.0f;

    
    float drawWidth  = screenWidth  * scale;
    float drawHeight = screenHeight * scale;

    float offsetX = (GetScreenWidth()  - drawWidth)  * 0.5f;
    float offsetY = (GetScreenHeight() - drawHeight) * 0.5f;

    // Convert mouse to GAME SPACE
    Vector2 mouse = GetMousePosition();
    float gameX = (mouse.x - offsetX) / scale;
    float gameY = (mouse.y - offsetY) / scale;


        MergeObjects(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged, gs.invincibility, gs.Jumpspeed2, gs.Ejected);

        // Global pause toggle
        if(IsKeyPressed(KEY_Q) && gs.currentScreen == GameState::Screen::GAMEPLAY)
        {
            paused = !paused;
        }
        if (paused && IsKeyPressed(KEY_W))
        {
            gs.currentScreen = GameState::Screen::LEVSEL;
            paused = false;
        }

      



        // ── Update ───────────────────────────────────────────────────────
       switch (gs.currentScreen)
        {
            case GameState::Screen::TITLE:
            {
                if (IsGestureDetected(GESTURE_TAP) &&
                    CheckCollisionPointRec({gameX, gameY}, startbutton))
                    gs.currentScreen = GameState::Screen::LEVSEL;
            } break;

            case GameState::Screen::LEVSEL:
            {
                if (IsGestureDetected(GESTURE_TAP) &&
                    CheckCollisionPointRec({gameX, gameY}, backbutton))
                {
                    gs.currentScreen = GameState::Screen::TITLE;
                }
                for (int i = 0; i < 4; i++)
                {
                    if (IsGestureDetected(GESTURE_TAP) &&
                        CheckCollisionPointRec({gameX, gameY}, levelselectbuttons[i]))
                    {
                        gs.LoadLevel(data, i + 1);
                        gs.currentScreen = GameState::Screen::GAMEPLAY;
                        break;
                    }
                }
            } break;
            

            case GameState::Screen::GAMEPLAY:
            {
                // Skip gameplay if paused
                if (paused) break;

                if (ExitDoors(gs.Ying, gs.Yan, gs.exitDoorHitboxYin, gs.exitDoorHitboxYang, gs.rad, gs.currentLevelId))
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

                if (gs.hasGlassPlatforms)
                {
                    for (auto& gp : gs.glassPlatforms){
                        if (!gp.broken){
                            gs.platforms.push_back(gp.rect);
                        }
                    }
                }
 
                // Horizontal collisions
                PlayerPlatformCollisions(gs.Ying, gs.rad,
                                 gs.platforms.data(), (int)gs.platforms.size(),
                                 gs.Yan, gs.obj3, oldP1, oldP2, oldMerged, gs.boxes);

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
                            LyftHorControls(gs.Ying, gs.Yan, gs.obj3, gs.rad,
                                    oldP1, oldP2, oldMerged,
                                    lift, gs.boxes);                      
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
                                     
                    if (gs.hasGlassPlatforms){
                             for (int i = 0; i < (int)gs.glassPlatforms.size(); i++){
                              if (!gs.glassPlatforms[i].broken){
                                     gs.platforms.pop_back();
                              }
                               
                        }
                    }     
               
        
                if (gs.hasBox)
                {
                    for (auto& box : gs.boxes){
                        PlayerBoxCollisions(gs.Ying, gs.rad, gs.Yan, gs.obj3,
                                    (int)gs.platforms.size(), gs.platforms.data(),
                                    box);
                        PlayerBoxVerticalCollisions(gs.Ying, gs.Yan, gs.obj3, gs.rad, box,
                                            gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump,
                                            gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged,
                                            oldP1, oldP2, oldMerged);
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

               /*  if (gs.hasVerticalShooter){
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
                    }*/
             

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

                // Spike / death checks
                for (const Rectangle& spike : gs.spikeHitboxes)
                {
                    if (CheckCollisionCircleRec(gs.Ying, gs.rad, spike) ||
                    CheckCollisionCircleRec(gs.Yan, gs.rad, spike))
                    {
                        death(gs.Ying, gs.Yan, gs.p1Spawn, gs.p2Spawn,
                            gs.invincibility, gs.invincibilityTimer, gs.restart);
                        break;
                    }

                    if (gs.merged && CheckCollisionCircleRec(gs.obj3, gs.rad, spike))
                    {
                        forceUnmerge(gs.Ying, gs.Yan, gs.obj3, gs.rad,
                             gs.merged, gs.invincibility, gs.invincibilityTimer);
                        break;
                    }
                }

                if (gs.hasSentry)
                    for (auto& sentry : gs.sentries)
                        UpdateSentry(sentry, gs.Ying, gs.Yan, gs.obj3,
                             gs.rad, gs.merged, gs.invincibility,
                             gs.invincibilityTimer, gs.restart,
                             gs.p1Spawn, gs.p2Spawn);

                if (gs.hasDoor && !gs.currentDoor.switched)
                    gs.platforms.pop_back();

        if (gs.hasDoor){
            UpdateDoor(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.currentDoor);
        }

          if (gs.hasGlassPlatforms){
            for (auto& gp : gs.glassPlatforms){
                UpdateGlassPlatform(gp, gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged);
            }
        }
    

            } break;
        }

        
          if (gs.hasGlassPlatforms){
            for (auto& gp : gs.glassPlatforms){
                UpdateGlassPlatform(gp, gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged);
            }
        }
    
        // ── Draw ─────────────────────────────────────────────────────────
        BeginTextureMode(target);
        ClearBackground(BLACK);


        switch (gs.currentScreen)
{
    case GameState::Screen::TITLE:
    {
        // Background (drawn in GAME SPACE)
        
        if (background_menu.width > 0)
        {
            DrawTexturePro(
                background_menu,
                { 0, 0, (float)background_menu.width, (float)background_menu.height },
                { 0, 0, (float)screenWidth, (float)screenHeight },   
                { 0, 0 }, 0.0f, WHITE
            );
        }

        // UI (GAME SPACE)
        int startSize  = 20;
        int startWidth = MeasureText("START", startSize);

        DrawText("Ying and Yan",      150, 20,  80, WHITE);
        DrawText("The Second Coming", 200, 100, 40, WHITE);

        DrawRectangleRec(startbutton, RED);
        DrawText("START",
            (int)(startbutton.x + startbutton.width  / 2 - startWidth / 2),
            (int)(startbutton.y + startbutton.height / 2 - startSize  / 2),
            startSize, LIGHTGRAY);

    }
    break;

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
    }
    break;

    case GameState::Screen::GAMEPLAY:
    {
        if (paused)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0,0,0,180});

            int titleSize = 60;
            int titleWidth = MeasureText("PAUSED", titleSize);
            DrawText("PAUSED", screenWidth/2 - titleWidth/2, 200, titleSize, WHITE);

            int instrSize = 20;
            int instrWidth = MeasureText("Press Q or R to resume", instrSize);
            DrawText("Press Q or R to resume", screenWidth/2 - instrWidth/2, 350, instrSize, LIGHTGRAY);

            int instr2Width = MeasureText("Press W to return to level select", instrSize);
            DrawText("Press W to return to level select", screenWidth/2 - instr2Width/2, 400, instrSize, LIGHTGRAY);

            break;
        }

        // Gameplay background (GAME SPACE)
        DrawTexturePro(
            gamePlay2,
            { 0, 0, (float)gamePlay2.width, (float)gamePlay2.height },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 }, 0.0f, WHITE
        );


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

        if ((gs.hasGlassPlatforms)){
            for (auto& gp : gs.glassPlatforms) {
                    DrawGlassPlatform(gp);
                    UpdateGlassPlatform(gp, gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged);
            }
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
             
               if(gs.hasVerticalShooter){
                for(auto& vertShoot : gs.vshooters){
                    DrawRectangleRec(vertShoot.rect, DARKGRAY);
                }
               }

               if(gs.hasHorizontalShooter){
                for(auto& horShoot : gs.hshooters){
                    DrawRectangleRec(horShoot.rect, DARKGRAY);
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
        EndTextureMode();

        
        
// Nudge to avoid bottom clipping
Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
Rectangle dst = {
    offsetX,
    offsetY,
    drawWidth,
    drawHeight - 30   // <- shave 1 pixel off the bottom
};

DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);

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

