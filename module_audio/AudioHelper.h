//
//	AudioHelper.h
//
//	Created by qiong on 14-12-1
//
//

#ifndef __AudioHelper__
#define __AudioHelper__

#include <string>
#include <vector>
#include <base/CCUserDefault.h>

//预加载背景音乐
void preloadBgMusic(const std::vector<std::string>& bgMuscNames);
void preloadBgMusic(const char* bgMuscName);
//预加载音效
void preloadEffects(const std::vector<std::string>& effectsNames);
void preloadEffect(const char* effectName);
//播放某个音效，返回播放的id，如果音效关闭，返回-1
unsigned int playEffect(const char* effectName, bool loop = false);
//停止某个音效，传入该音效的id
void stopEffect(unsigned int);
//停止所有音效
void pauseAllEffects();
//继续所有音效
void resumeAllEffects();
//播放背景音乐
void playBgMusic(const char* MuscName);
///如果此时有背景音乐 不播放/
void playBgMusic_Ex(const char* MuscName);
//结束背景音乐
void stopBgMusic();
//继续背景音乐
void resumeBgMusic();
//暂停背景音乐
void pauseBgMusic();
//判断某个背景音乐是否正在播放
bool isBgMusicPlaying(const char* MuscName);
//! 背景音乐和音效的控制开关
inline bool isMusicOn()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("musicOn",true);
}
inline bool isEffectOn()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("effectOn",true);
}
inline void turnOffMusic()
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("musicOn", false);
}
inline void turnOnMusic()
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("musicOn", true);
}
inline void turnOffEffect()
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("effectOn", false);
}
inline void turnOnEffect()
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("effectOn", true);
}

#endif