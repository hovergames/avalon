@interface GameCenterIos : NSObject
{
    BOOL isAuthenticated;
}

+ (GameCenterIos*)shared;

- (void)login;

- (BOOL)showAchievements;
- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;
- (void)clearAllAchivements;

- (BOOL)showScores;
- (void)postScore:(const char*)idName score:(NSNumber*)score;
- (void)clearAllScores;

@end
