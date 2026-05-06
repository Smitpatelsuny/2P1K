/*#include "GameAudio.h"

GameAudio LoadGameAudio()
{
    GameAudio audio;

    audio.keyPickup = LoadSound("src/assets/sounds/key_pickup.mp3");
    audio.doorOpen = LoadSound("src/assets/sounds/Exitdoor.mp3");
    audio.buttonPress = LoadSound("src/assets/sounds/button_pressed.wav");
    audio.jump = LoadSound("src/assets/sounds/jump.wav");
    audio.death1 = LoadSound("src/assets/sounds/death(1).wav");
    audio.death2 = LoadSound("src/assets/sounds/death(2).wav");
    audio.merge = LoadSound("src/assets/sounds/merge.wav");
    audio.walk = LoadSound("src/assets/sounds/walk.mp3");
    audio.liftUp = LoadSound("src/assets/sounds/lift_up.mp3");
    audio.liftDown= LoadSound("src/assets/sounds/lift.mp3");
    audio.mergeDeath= LoadSound("src/assets/sounds/death.wav");
    audio.shooter = LoadSound("src/assets/sounds/shootingSound.mp3");

    //audio.gameplayMusic = LoadMusicStream("src/assets/sounds/gameplay_music.mp3");
    audio.gameplayMusic = LoadMusicStream("src/assets/sounds/ambience.wav");

    return audio;
}

void UnloadGameAudio(GameAudio& audio)
{
    UnloadSound(audio.keyPickup);
    UnloadSound(audio.doorOpen);
    UnloadSound(audio.buttonPress);
    UnloadSound(audio.jump);
    UnloadSound(audio.death1);
    UnloadSound(audio.death2);
    UnloadSound(audio.merge);
    UnloadSound(audio.walk);
    UnloadSound(audio.liftUp);
    UnloadSound(audio.liftDown);
    UnloadSound(audio.mergeDeath);
    UnloadSound(audio.shooter);


    UnloadMusicStream(audio.gameplayMusic);
}gameAudio.cpp*/