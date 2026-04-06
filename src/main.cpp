 

#include "raylib.h" 

#include "PlayerStuff.h" 

#include "DeaStart.h" 

#include "Objects.h" 

#include "GameState.h" 

#include "json.hpp" 

#include <fstream> 

#include <iostream> 

 

using json = nlohmann::json; 

 

int main(void) 

{ 

    const int screenWidth  = 800; 

    const int screenHeight = 800; 

 

    json data; 

    { 

        std::ifstream file("C:/Users/dalma/projects/2P1K/src/Level.json"); 

        data = json::parse(file); 

    } 

 

    GameState gs;   // <-- everything lives here 

 

    InitWindow(screenWidth, screenHeight, "All code test"); 

 

    Texture2D background_menu = LoadTexture("C:/Users/dalma/projects/2P1K/src/assets/background_menu.jpg"); 

    Texture2D gamePlay2       = LoadTexture("C:/Users/dalma/projects/2P1K/src/assets/gamePlay2.jpg"); 

 

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

        MergeObjects(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.merged, gs.invincibility);
 

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

                              gs.exitDoorHitboxYin, gs.exitDoorHitboxYang, gs.rad)) 

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

                                         gs.Yan, gs.obj3, oldP1, oldP2, oldMerged, currentLift.rect); 

                if (gs.hasLift) 

                    LyftHorControls(gs.Ying, gs.Yan, gs.obj3, gs.rad, 

                                    oldP1, oldP2, oldMerged, gs.currentLift, gs.merged); 

 

                // Vertical collisions 

                Jump(gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged, 

                     gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump, 

                     gs.merged, gs.rad, gs.Ying, gs.Yan, gs.obj3); 

 

                PlayerJumpCollisions(gs.Ying, gs.Yan, gs.obj3, gs.rad, 

                                     gs.platforms.data(), (int)gs.platforms.size(), 

                                     gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump, 

                                     gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged, 

                                     oldP1, oldP2, oldMerged, currentLift.rect); 

                if (gs.hasLift) 

                    LyftVertControls(gs.Ying, gs.Yan, gs.obj3, gs.rad, 

                                     oldP1, oldP2, oldMerged, 

                                     gs.P1CanJump, gs.P2CanJump, gs.MergedCanJump, 

                                     gs.Jumpspeed, gs.Jumpspeed2, gs.JumpspeedMerged, 

                                     gs.currentLift); 

            } break; 

        } 

 

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
                DrawRectangleRec(startbutton, GREEN); 
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

 

                death(gs.Ying, gs.Yan, 

                      gs.spikeHitboxes.data(), (int)gs.spikeHitboxes.size(), 

                      gs.rad, gs.deathOccured, 

                      gs.invincibility, gs.invincibilityTimer, 

                      gs.p1Spawn, gs.p2Spawn); 

 

                forceUnmerge(gs.Ying, gs.Yan, gs.obj3, 

                             gs.spikeHitboxes.data(), (int)gs.spikeHitboxes.size(), 

                             gs.rad, gs.merged, 

                             gs.invincibility, gs.invincibilityTimer); 

 

                if (gs.hasLift) 

                    levelOne(gs.Ying, gs.Yan, gs.obj3, gs.rad, gs.currentLift); 

 

                if (!gs.merged) { 

                    DrawCircleV(gs.Ying, gs.rad, RED); 

                    DrawCircleV(gs.Yan,  gs.rad, BLUE); 

                } else { 

                    DrawCircleV(gs.obj3, gs.rad, PURPLE); 

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

 

    UnloadTexture(background_menu); 

    UnloadTexture(gamePlay2); 

    CloseWindow(); 

    return 0; 
} 