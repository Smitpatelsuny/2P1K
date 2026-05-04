#include "raylib.h"
#include "PlayerStuff.h"
#include <stdio.h>
#include "Objects.h"
#include "levelDrawing.h"
#include <vector>


void DrawTexturedPlatforms(const std::vector<Rectangle>& platforms, Texture2D platformTiles)
{

// Draw textured platforms
Rectangle platformLeftSrc    = { 0, 0, 32, 32 };   // left end tile
Rectangle platformMiddleASrc = { 32, 0, 32, 32 };  // normal middle tile
Rectangle platformMiddleBSrc = { 64, 0, 32, 32 };  // alternate middle tile
Rectangle platformRightSrc   = { 96, 0, 32, 32 };  // right end tile

float tileSize = 32.0f;

for (const auto &p : platforms)
{
if (p.height > p.width)
{
    DrawRectangleRec(p,DARKGRAY);
    
    continue; //skip normal platform drawing 

}

    int tilesAcross = (int)(p.width / tileSize);

    if (tilesAcross < 2)
    {
        tilesAcross = 2;
    }

    for (int i = 0; i < tilesAcross; i++)
    {
        Rectangle source;

        if (i == 0)
        {
            source = platformLeftSrc; // first tile
        }
        else if (i == tilesAcross - 1)
        {
            source = platformRightSrc; // last tile
        }
        else
        {
            // Alternate between middle A and middle B
            if (i % 3 == 0)
            {
                source = platformMiddleBSrc; // use variation sometimes
            }
            else
            {
                source = platformMiddleASrc; // main repeating tile
            }
        }

        Rectangle dest = {
            p.x + i * tileSize,
            p.y - 20,      // visual offset so platform image sits on collision
            60,
            70
        };

        DrawTexturePro(
            platformTiles,
            source,
            dest,
            { 0, 0 },
            0.0f,
            WHITE
        );
        
    }
   
   //debug linefor collision
   // DrawRectangleLinesEx(p,2,GREEN);
    }

}
void DrawKey(const AnimatedSprite& key, Rectangle keyHitbox, bool hasKey)
{
     // Draw key if it has not been collected yet
            if (!hasKey)

            {
                DrawTexturePro(key.texture, 
                    key.frameRec, 
                    keyHitbox, 
                    {0, 0}, 
                    0.0f, 
                    WHITE
                               // origin ,rotation ,tint/color
                );
            }

}

void DrawExitDoors(const AnimatedSprite& door, Rectangle yinDoorHitbox, Rectangle yangDoorHitbox)
{
// Draw animated exit doors.
            // The hitboxes stay small for collision,
            // but the visual doors are bigger so they do not look tiny.
            Rectangle yinDoorVisual = {
                yinDoorHitbox.x - 15,
                yinDoorHitbox.y - 120,
                120,
                140
            };

            Rectangle yangDoorVisual = {
                yangDoorHitbox.x - 75,
                yangDoorHitbox.y - 120,
                120,
                140
            };

            DrawTexturePro(
                door.texture,
                door.frameRec,
                yinDoorVisual,
                {0, 0},
                0.0f,
                WHITE);

            DrawTexturePro(
                door.texture,
                door.frameRec,
                yangDoorVisual,
                {0, 0},
                0.0f,
                WHITE);
}

void DrawLiftWithButton(const LiftData& lift, Texture2D liftTexture,
                        const AnimatedSprite& liftButtonSprite)
{
    // Scale the visual to match the lift rect size
    // The offsets add visual padding around the collision rect
    float padX = 3.0f;   // horizontal padding on each side
    float padY = 10.0f;  // vertical padding on top

    Rectangle liftVisual = {
        lift.rect.x - padX,
        lift.rect.y - padY,
        lift.rect.width  + (padX * 2),  // stretch to fill width + both side pads
        lift.rect.height + padY         // stretch to fill height + top pad
    };

    DrawTexturePro(
        liftTexture,
        { 0, 0, (float)liftTexture.width, (float)liftTexture.height },
        liftVisual,
        { 0, 0 },
        0.0f,
        WHITE);

    // Button visual scales around the button rect
    Rectangle biggerButton = {
        lift.button.x  - 8,
        lift.button.y  - 7,
        lift.button.width  + 32,
        lift.button.height + 26
    };

    DrawTexturePro(
        liftButtonSprite.texture,
        liftButtonSprite.frameRec,
        biggerButton,
        { 0, 0 },
        0.0f,
        WHITE);
}

void DrawSpikeTextures(const std::vector<Rectangle>& spikeHitboxes, Texture2D spikeTexture)
{
    for (const auto &spike : spikeHitboxes)
    {
        Rectangle spikeVisual = {
            spike.x -20,          // move visual slightly left
            spike.y - 20,         // move visual upward so it sits on the platform
            spike.width + 40,     // make visual wider than collision hitbox
            spike.height + 40     // make visual taller than collision hitbox
        };

        DrawTexturePro(
            spikeTexture,
            {0, 0, (float)spikeTexture.width, (float)spikeTexture.height},
            spikeVisual,
            {0, 0},
            0.0f,
            WHITE
        );

        //debugging
        //DrawRectangleLinesEx(spike, 2, RED);
    }
}