#ifndef LEVELDRAWING_H
#define LEVELDRAWING_H

#include "Objects.h"
#include "playerStuff.h"
#include "raylib.h"
#include <vector>

void DrawTexturedPlatforms(const std::vector<Rectangle>& platforms, Texture2D platformTiles);
void DrawKey(const AnimatedSprite& key, Rectangle keyHitbox, bool hasKey);
void DrawExitDoors(const AnimatedSprite& door, Rectangle yinDoorHitbox, Rectangle yangDoorHitbox);
void DrawLiftWithButton(const LiftData& lift, Texture2D liftTexture, const AnimatedSprite& liftButtonSprite);
void DrawSpikeTextures(const std::vector<Rectangle>& spikeHitboxes, Texture2D spikeTexture);

#endif