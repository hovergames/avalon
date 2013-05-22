#import <avalon/platform/ios/gamecenter/GameCenterIos.h>

#import <GameKit/GameKit.h>
#import "AppController.h"
#import "RootViewController.h"

@interface GameCenterIos(intern)
- (void)saveAchievementToDevice:(GKAchievement*)achievement;
- (void)retrieveAchievementsFromDevice;

- (void)saveScoreToDevice:(GKScore*)score;
- (void)retrieveScoresFromDevice;

- (void)registerForAuthenticationNotification;
- (void)authenticationChanged;

- (NSString*)getGameCenterSavePath:(NSString*)postfix;
@end

@implementation GameCenterIos

static NSString* scoresArchiveKey = @"scores";
static NSString* achievementsArchiveKey = @"achievements";
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
        if (error) {
            if (error.code == GKErrorAuthenticationInProgress) {
                // Silently ignore this case
            }

            NSLog(@"[GameCenter] login failed: %@", error.localizedDescription);
            return;
        }

        isAuthenticated = YES;
        [self registerForAuthenticationNotification];
        [self retrieveScoresFromDevice];
        [self retrieveAchievementsFromDevice];
    }];
}

#pragma mark -
#pragma mark Achievements

- (BOOL)showAchievements
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        [self login];
        return NO;
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKAchievementViewController* gkController = [[[GKAchievementViewController alloc] init] autorelease];
    gkController.achievementDelegate = appController->viewController;

    [appController->viewController presentModalViewController:gkController animated:YES];
    return YES;
}

- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete
{
    GKAchievement* achievement = [[[GKAchievement alloc] init] autorelease];
    achievement.identifier = [NSString stringWithUTF8String:idName];
    achievement.percentComplete = [percentComplete doubleValue];
    achievement.showsCompletionBanner = YES;

    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        [self saveAchievementToDevice:achievement];
        return;
    }

    [achievement reportAchievementWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postAchievement for %s failed: %@", idName, error.localizedDescription);
        }
    }];
}

- (void)clearAllAchivements
{
    // clear cached achievements
	NSString* savePath = [self getGameCenterSavePath:achievementsArchiveKey];
    NSError* error;
    [[NSFileManager defaultManager] removeItemAtPath:savePath error:&error];

    // cleare remote achievements
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] clearAllAchivements failed: %@", error.localizedDescription);
        }
    }];
}

#pragma mark -
#pragma mark Leaderboard

- (BOOL)showScores
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        [self login];
        return NO;
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKLeaderboardViewController* gkController = [[[GKLeaderboardViewController alloc] init] autorelease];
    gkController.timeScope = GKLeaderboardTimeScopeAllTime;
    gkController.leaderboardDelegate = appController->viewController;

    [appController->viewController presentModalViewController:gkController animated:YES];
    return YES;
}

- (void)postScore:(const char*)idName score:(NSNumber*)score;
{
    GKScore* gkScore = [[[GKScore alloc] init] autorelease];
    gkScore.category = [NSString stringWithUTF8String:idName];
    gkScore.value = [score longLongValue];
    gkScore.shouldSetDefaultLeaderboard = YES;

    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        [self saveScoreToDevice:gkScore];
        return;
    }
    
    [gkScore reportScoreWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postScore for %s failed: %@", idName, error.localizedDescription);
        }
    }];
}

- (void)clearAllScores
{
    // clear cached scores
	NSString* savePath = [self getGameCenterSavePath:scoresArchiveKey];
    NSError* error;
    [[NSFileManager defaultManager] removeItemAtPath:savePath error:&error];

    // clear remote scores
    // ... is not possible :/
}

#pragma mark -
#pragma mark Cache Achievements

- (void)saveAchievementToDevice:(GKAchievement*)achievement
{
	NSString* savePath = [self getGameCenterSavePath:achievementsArchiveKey];
	NSMutableDictionary* data;

	if ([[NSFileManager defaultManager] fileExistsAtPath:savePath]) {
        data = [[[NSMutableDictionary alloc] initWithContentsOfFile:savePath] autorelease];
	} else {
        data = [[[NSMutableDictionary alloc] init] autorelease];
	}

    [data setObject:[NSNumber numberWithDouble:achievement.percentComplete] forKey:achievement.identifier];
	[data writeToFile:savePath atomically:YES];
}

- (void)retrieveAchievementsFromDevice
{
	NSString* savePath = [self getGameCenterSavePath:achievementsArchiveKey];
	if (![[NSFileManager defaultManager] fileExistsAtPath:savePath]) {
		return;
	}

	NSMutableDictionary* data = [NSMutableDictionary dictionaryWithContentsOfFile:savePath];
	if (!data) {
		return;
	}

    NSError* error;
    [[NSFileManager defaultManager] removeItemAtPath:savePath error:&error];

	for (NSString* key in data) {
        NSNumber* number = [data objectForKey:key];

        GKAchievement* achievement = [[[GKAchievement alloc] init] autorelease];
        achievement.identifier = key;
        achievement.percentComplete = [number doubleValue];
        achievement.showsCompletionBanner = YES;

		[achievement reportAchievementWithCompletionHandler:^(NSError* error){
			if (error) {
				[self saveAchievementToDevice:achievement];
			}
		}];
    }
}

#pragma mark -
#pragma mark Cache Leaderboard

- (void)saveScoreToDevice:(GKScore*)score
{
	NSString* savePath = [self getGameCenterSavePath:scoresArchiveKey];
	NSMutableDictionary* data;

	if ([[NSFileManager defaultManager] fileExistsAtPath:savePath]) {
        data = [[[NSMutableDictionary alloc] initWithContentsOfFile:savePath] autorelease];
	} else {
        data = [[[NSMutableDictionary alloc] init] autorelease];
	}

    [data setObject:[NSNumber numberWithLongLong:score.value] forKey:score.category];
	[data writeToFile:savePath atomically:YES];
}

- (void)retrieveScoresFromDevice
{
	NSString* savePath = [self getGameCenterSavePath:scoresArchiveKey];
	if (![[NSFileManager defaultManager] fileExistsAtPath:savePath]) {
		return;
	}
	
	NSMutableDictionary* data = [NSMutableDictionary dictionaryWithContentsOfFile:savePath];
	if (!data) {
		return;
	}

    NSError* error;
    [[NSFileManager defaultManager] removeItemAtPath:savePath error:&error];
	
	for (NSString* key in data) {
        NSNumber* number = [data objectForKey:key];
        
        GKScore* gkScore = [[[GKScore alloc] init] autorelease];
        gkScore.category = key;
        gkScore.value = [number longLongValue];
        gkScore.shouldSetDefaultLeaderboard = YES;
        
		[gkScore reportScoreWithCompletionHandler:^(NSError* error) {
			if (error) {
				[self saveScoreToDevice:gkScore];
            }
		}];
    }
}

#pragma mark -
#pragma mark Cache Helper

- (NSString*)getGameCenterSavePath:(NSString*)postfix
{
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	return [NSString stringWithFormat:@"%@/avalon.gamecenter.%@.cache",[paths objectAtIndex:0],postfix];
}

- (void)registerForAuthenticationNotification
{
	NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
	[nc addObserver: self selector:@selector(authenticationChanged) name:GKPlayerAuthenticationDidChangeNotificationName object:nil];
}

- (void)authenticationChanged
{
	if ([GKLocalPlayer localPlayer].isAuthenticated) {
		isAuthenticated = YES;
		[self retrieveScoresFromDevice];
		[self retrieveAchievementsFromDevice];
	} else {
        isAuthenticated = NO;
    }
}

@end
