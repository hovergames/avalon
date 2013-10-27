#ifdef AVALON_CONFIG_APPIRATER_ENABLED

#ifndef AVALON_APPIRATER_H
#define AVALON_APPIRATER_H

namespace avalon {

class Appirater
{
public:
    static Appirater* getInstance();

    void setInitialDaysUntilPrompt(const int days);
    void setReminderDaysUntilPrompt(const int days);
    void setSignificantEventsUntilPrompt(const int events);
    void setDebug(const bool flag);

    void init();
    void userDidSignificantEvent();
    void showIfNeeded();
};

} // namespace avalon

#endif /* AVALON_APPIRATER_H */

#endif /* AVALON_CONFIG_APPIRATER_ENABLED */
