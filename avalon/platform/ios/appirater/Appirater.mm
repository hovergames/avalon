#include <avalon/Appirater.h>

#import <avalon/platform/ios/appirater/Appirater/Appirater.h>

namespace avalon {

Appirater* Appirater::getInstance()
{
    static Appirater* instance = new Appirater();
    return instance;
}

void Appirater::setInitialDaysUntilPrompt(const int days)
{
    [::Appirater setDaysUntilPrompt:days];
}

void Appirater::setReminderDaysUntilPrompt(const int days)
{
    [::Appirater setTimeBeforeReminding:days];
}

void Appirater::setSignificantEventsUntilPrompt(const int events)
{
    [::Appirater setSignificantEventsUntilPrompt:events];
}

void Appirater::setDebug(const bool flag)
{
    [::Appirater setDebug:flag];
}

void Appirater::init()
{
    NSString* appID = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
    [::Appirater setAppId:appID];

    // we don't use this aspect and only rely on significant events!
    [::Appirater setUsesUntilPrompt:0];
}

void Appirater::userDidSignificantEvent()
{
    [::Appirater userDidSignificantEvent:NO];
}

void Appirater::showIfNeeded()
{
    [::Appirater appLaunched:YES];
}

} // namespace avalon
