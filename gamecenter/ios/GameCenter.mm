#include "GameCenter.h"

#include "GameCenterIos.h"

namespace Avalon {

void GameCenter::login()
{
    [[GameCenterIos shared] login];
}

#pragma mark -
#pragma mark Achievements

void GameCenter::showAchievements()
{
    [[GameCenterIos shared] showAchievements];
}

void GameCenter::postAchievement(const char* idName, int percentComplete)
{
    [[GameCenterIos shared] postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

void GameCenter::clearAllAchievements()
{
    [[GameCenterIos shared] clearAllAchivements];
}

#pragma mark -
#pragma mark Leaderboard

void GameCenter::showScores()
{
    [[GameCenterIos shared] showScores];
}

void GameCenter::postScore(const char* idName, int score)
{
    [[GameCenterIos shared] postScore:idName score:[NSNumber numberWithInt:score]];
}

void GameCenter::clearAllScores()
{
    [[GameCenterIos shared] clearAllScores];
}

} // namespace Avalon