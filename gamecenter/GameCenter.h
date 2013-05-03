#ifndef __Avalon__GameCenter__
#define __Avalon__GameCenter__

namespace Avalon {

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

} // namespace Avalon

#endif /* __Avalon__GameCenter__ */