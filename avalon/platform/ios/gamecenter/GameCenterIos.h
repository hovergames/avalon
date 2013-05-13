@interface GameCenterIos : NSObject
{
}

+ (GameCenterIos*)shared;

- (void)login;

- (void)showAchievements;
- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;
- (void)clearAllAchivements;

- (void)showScores;
- (void)postScore:(const char*)idName score:(NSNumber*)score;
- (void)clearAllScores;

@end
