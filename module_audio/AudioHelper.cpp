//
//	AudioHelper.cpp
//
//	Created by qiong on 14-12-1
//
//
#include "AudioHelper.h"
#include <SimpleAudioEngine.h>

static char bgmusicPlaying[30];
void preloadBgMusic(const std::vector<std::string>& bgMuscNames)
{
    for(auto& name: bgMuscNames){
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
    }
}
void preloadBgMusic(const char* bgMuscName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMuscName);
}

void preloadEffects(const std::vector<std::string>& effectsNames)
{
    for(auto& name: effectsNames){
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name.c_str());
    }
}
void preloadEffect(const char* effectName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(effectName);
}
unsigned int playEffect(const char* effectName, bool loop /*= false*/)
{
    if (isEffectOn())
        return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectName,loop);
    else
        return 0xFEEDBAB;
}

void stopEffect(unsigned int soundid)
{
    if (soundid != 0xFEEDBAB) {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(soundid);
    }
}
void pauseAllEffects()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void resumeAllEffects()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}
void playBgMusic(const char* MuscName)
{
    if (isMusicOn()) {
        strcpy(bgmusicPlaying, MuscName);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(MuscName, true);
    }
}

void playBgMusic_Ex(const char* MuscName)
{
    if(!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        playBgMusic(MuscName);
}

void stopBgMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void resumeBgMusic(){
    if (isMusicOn()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
}

void pauseBgMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

bool isBgMusicPlaying(const char* MuscName)
{
    return strcmp(bgmusicPlaying, MuscName) == 0;
}
