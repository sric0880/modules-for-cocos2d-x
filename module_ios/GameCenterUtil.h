//
//  GameCenterUtil.h
//
//  Created by qiong on 14-12-1
//
//
//

#ifndef __GameCenterUtil__
#define __GameCenterUtil__

bool isGameCenterLogin();               //是否已经登录GameCenter
void loginGameCenter();                 //登录GameCenter获得player id
void showGameGKLeaderboardView();       //显示排行榜
void reportLeaderboard(const char* category, int64_t score);    //更新排行版
void reportAchievement(const char* achid, float percent);       //更新成就

#endif