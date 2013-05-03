#include "GameCenter.h"

#include "GameCenterLauncher.h"

namespace Avalon {

void GameCenter::login()
{
    [[GameCenterLauncher shared] login];
}

#pragma mark -
#pragma mark Achievements

void GameCenter::showAchievements()
{
    [[GameCenterLauncher shared] showAchievements];
}

void GameCenter::postAchievement(const char* idName, int percentComplete)
{
    [[GameCenterLauncher shared] postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

void GameCenter::clearAllAchievements()
{
    [[GameCenterLauncher shared] clearAllAchivements];
}

#pragma mark -
#pragma mark Leaderboard

void GameCenter::showScores()
{
    [[GameCenterLauncher shared] showScores];
}

void GameCenter::postScore(const char* idName, int score)
{
    [[GameCenterLauncher shared] postScore:idName score:[NSNumber numberWithInt:score]];
}

void GameCenter::clearAllScores()
{
    [[GameCenterLauncher shared] clearAllScores];
}

} // namespace Avalon