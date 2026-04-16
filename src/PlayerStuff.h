#ifndef PLAYERSTUFF_H
#define PLAYERSTUFF_H

#include <raylib.h>
#include <raymath.h>


void PlayerMotor(Vector2 &player, int speedX);
void Player2Motor(Vector2& player, int speedX);
void MergePlayerMotor(Vector2 &player, int speedX);
void PlayerBorderCollisions(Vector2& player, int radius, int screenWidth, int screenHeight, bool& CanJump);
void Jump(float &Jumpspeed, float &Jumpspeed2, float &JumpspeedMerged, bool &P1CanJump, bool &P2CanJump, bool &MergedCanJump, bool &merged, float radius, Vector2 &player, Vector2 &ballPosition, Vector2 &obj3, bool Ejected);
void MergeObjects(Vector2& obj1, Vector2& obj2, Vector2& obj3, float& radius1, bool& merged, bool invincibility, float &YanJumpSpee, bool Ejected);
void PlayerPlatformCollisions(Vector2& player, int rad, Rectangle platforms[], int platformCount, Vector2& ballPosition, Vector2& obj3, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, Rectangle box);
void PlayerJumpCollisions(Vector2& player, Vector2& ballPosition, Vector2& obj3, float rad, Rectangle platforms[], int platformCount, bool& P1CanJump, bool& P2CanJump, bool& MergedCanJump, float& Jumpspeed, float& Jumpspeed2, float& JumpspeedMerged, Vector2& oldP1, Vector2& oldP2, Vector2& oldMerged, Rectangle box);


enum AnimState
{
    ANIM_IDLE = 0,
    ANIM_RUN = 1,
    ANIM_JUMP = 2,
    ANIM_FALL = 3
};

struct AnimatedSprite
{
    Texture2D texture = {0};
    Rectangle frameRec = {0,0,0,0};

    int frameWidth = 0;
    int frameHeight = 0;

    int currentFrame = 0;
    int currentRow = 0;

    float frameTimer = 0.0f;
    float frameDuration = 0.2f;

    int columns = 0;
    int rows = 0;
};


class PlayerStuff
{
public:
    void PlayerMovement();

};
AnimatedSprite LoadAnimatedSprite(const char* filePath, float scale, int columns, int rows, float frameDuration);
void UpdateAnimatedSprite(AnimatedSprite& sprite, AnimState state);
void DrawAnimatedSpriteCentered(const AnimatedSprite& sprite, Vector2 centerPos, bool facingRight, Color tint);
void UnloadAnimatedSprite(AnimatedSprite& sprite);

#endif