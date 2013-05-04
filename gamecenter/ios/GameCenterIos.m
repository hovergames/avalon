#import "GameCenterIos.h"

#import <GameKit/GameKit.h>
#import "AppController.h"
#import "RootViewController.h"

@implementation GameCenterIos

static GameCenterIos* instance = nil;

+ (GameCenterIos*)shared
{
	@synchronized(self) {
        if (instance == nil) {
            instance = [[self alloc] init];
        }
    }
    return instance;
}

- (void)login
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated) {
        return;
    }

    [localPlayer authenticateWithCompletionHandler:^(NSError* error) {
        if (!error) {
            return;
        }

        if (error.code == GKErrorAuthenticationInProgress) {
            // ignore this case
        } else {
            NSLog(@"[GameCenter] login failed: %@", error.localizedDescription);
        }
    }];
}

#pragma mark -
#pragma mark Achievements

- (void)showAchievements
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return [self login];
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKAchievementViewController* gkController = [[[GKAchievementViewController alloc] init] autorelease];
    gkController.achievementDelegate = appController->viewController;

    [appController->viewController presentModalViewController:gkController animated:YES];
}

- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return [self login];
    }

    GKAchievement* achievement = [[[GKAchievement alloc] init] autorelease];
    achievement.identifier = [NSString stringWithUTF8String:idName];
    achievement.percentComplete = [percentComplete doubleValue];
    achievement.showsCompletionBanner = YES;

    [achievement reportAchievementWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postAchievement failed: %@", error.localizedDescription);
        }
    }];
}

- (void)clearAllAchivements
{
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] clearAllAchivements failed: %@", error.localizedDescription);
        }
    }];
}

#pragma mark -
#pragma mark Leaderboard

- (void)showScores
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return [self login];
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKLeaderboardViewController* gkController = [[[GKLeaderboardViewController alloc] init] autorelease];
    gkController.timeScope = GKLeaderboardTimeScopeAllTime;
    gkController.leaderboardDelegate = appController->viewController;

    [appController->viewController presentModalViewController:gkController animated:YES];
}

- (void)postScore:(const char*)idName score:(NSNumber*)score;
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return [self login];
    }

    GKScore* gkScore = [[[GKScore alloc] init] autorelease];
    gkScore.category = [NSString stringWithUTF8String:idName];
    gkScore.value = [score longLongValue];
    gkScore.shouldSetDefaultLeaderboard = YES;

    [gkScore reportScoreWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postScore failed: %@", error.localizedDescription);
        }
    }];
}

- (void)clearAllScores
{
    // this is important or we would later create tons of login attempts
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return;
    }

    [GKLeaderboard loadCategoriesWithCompletionHandler:^(NSArray* categories, NSArray* titles, NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] clearAllScores failed: %@", error.localizedDescription);
            return;
        }

        for (NSString* categoryName in categories) {
            [self postScore:[categoryName cStringUsingEncoding:NSASCIIStringEncoding] score:[NSNumber numberWithInt:0]];
        }
    }];
}

@end