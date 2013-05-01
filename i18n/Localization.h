//
//  Localization.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#ifndef __Adventures_on_the_farm__Localization__
#define __Adventures_on_the_farm__Localization__

#include "cocos2d.h"
#include "Singleton.h"

class LanguageKey;
class Language;

class Localization : public Singleton<Localization>
{
    friend class Singleton<Localization>;
    
private:
    Localization();
    ~Localization();

public:
    Language *currentLanguage;
    Language *defaultLanguage;

    std::map<std::string, Language*> languages;
    
    void addLanguage(const char *languageFile);
    void setDefaultLanguage(const char *languageFile);
    void setCurrentLanguage(const char *languageFile);

    void clear();
};

LanguageKey* _(const char *section, const char *key);

#endif /* defined(__Adventures_on_the_farm__Localization__) */
