#ifndef AVALON_GAMECENTER_H
#define AVALON_GAMECENTER_H

namespace avalon {

class GameCenter
{
public:
    void login();

    void showAchievements();
    void postAchievement(const char* idName, int percentComplete);
    void clearAllAchievements();

    void showScores();
    void postScore(const char* idName, int score);
    void clearAllScores();
};

} // namespace avalon

#endif /* AVALON_GAMECENTER_H */
