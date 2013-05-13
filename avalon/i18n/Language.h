#ifndef AVALON_I18N_LANGUAGE_H
#define AVALON_I18N_LANGUAGE_H

#include <map>
#include <string>

namespace avalon {
namespace i18n {

class LanguageKey;

class Language
{
public:
    std::string fileName;
    
    Language() {};
    ~Language();

    std::map<std::string, std::map<std::string, LanguageKey*> >sections;

    LanguageKey* addLanguageKey(const char* section, const char* varName, const char* value);
    LanguageKey* getLanguageKey(const char* section, const char* varName);
};

} // namespace i18n
} // namespace avalon

#endif /* AVALON_I18N_LANGUAGE_H */