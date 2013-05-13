#ifndef AVALON_I18N_LOCALIZATION_H
#define AVALON_I18N_LOCALIZATION_H

#include <avalon/utils/Singleton.h>
#include "cocos2d.h"

namespace avalon {
namespace i18n {

class LanguageKey;
class Language;

class Localization : public avalon::utils::Singleton<Localization>
{
    friend class Singleton<Localization>;
    
private:
    Localization();
    ~Localization();

public:
    Language *currentLanguage;
    Language *defaultLanguage;

    std::map<std::string, Language*> languages;
    
    void addLanguage(const char* languageFile);
    void setDefaultLanguage(const char* languageFile);
    void setCurrentLanguage(const char* languageFile);
    bool hastLanguage(const char* languageFile) const;

    void clear();
};

LanguageKey& _(const char* section, const char* key);

} // namespace i18n
} // namespace avalon

#endif /* AVALON_I18N_LOCALIZATION_H */
