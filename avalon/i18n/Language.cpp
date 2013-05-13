#include <avalon/i18n/Language.h>

#include <avalon/i18n/LanguageKey.h>

namespace avalon {
namespace i18n {

Language::~Language()
{
    for (auto section : sections) {
        for (auto key : section.second) {
            delete key.second;
        }
    }
}

LanguageKey* Language::addLanguageKey(const char* section, const char* varName, const char* value)
{
    if (sections[section][varName]) {
        delete sections[section][varName];
    }
    
    sections[section][varName] = new LanguageKey(varName, value);
    return sections[section][varName];
}

LanguageKey* Language::getLanguageKey(const char* section, const char* varName)
{
    return sections[section][varName];
}

} // namespace i18n
} // namespace avalon