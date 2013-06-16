#include <avalon/i18n/Localization.h>

#include <cassert>
#include <string>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Language.h>
#include <avalon/io/IniReader.h>
#include <boost/assert.hpp>

using namespace cocos2d;

namespace avalon {
namespace i18n {

Localization::Localization()
: currentLanguage(NULL)
, defaultLanguage(NULL)
{
}

Localization::~Localization()
{
    clear();
}


void Localization::clear()
{
    for (auto& language : languages ) {
        delete language.second;
    }
    languages.clear();

    currentLanguage = NULL;
    defaultLanguage = NULL;
}

LanguageKey& _(const char* section, const char* key)
{
    assert(Localization::getInstance().currentLanguage && "currentLanguage is not set!");

    LanguageKey *langKey = Localization::getInstance().currentLanguage->getLanguageKey(section, key);
    if (!langKey || langKey->name.empty()) {
        CCLog(
            "WARNING! Language Key not found in current Language '%s': %s/%s",
            Localization::getInstance().currentLanguage->fileName.c_str(), section, key
        );

        langKey = Localization::getInstance().defaultLanguage->getLanguageKey(section, key);
        if (!langKey || langKey->name.empty()) {
            if (!langKey) {
                langKey = Localization::getInstance().currentLanguage->addLanguageKey(section, key,  (std::string("_") + std::string(key)).c_str());
            } else {
                CCLog("Set temporary key");
                langKey->value = (std::string("_") + std::string(key)).c_str();
            }
            CCLog("WARNING! Language Key not found in default Language: %s/%s", section, key);
        }
    }
    
    return *langKey;
}

void Localization::addLanguage(const char* languageFile)
{
    if (hastLanguage(languageFile)) {
        return;
    }
    
    Language *lang = new Language();
    lang->fileName = languageFile;

    avalon::io::IniReader file;
    file.loadFile(languageFile);

    for (auto& section : *file.getSections()) {
        for (auto& row : section.second) {
            lang->addLanguageKey(section.first.c_str(), row.first.c_str(), row.second.c_str());
        }
    }

    if (!currentLanguage) {
        currentLanguage = lang;
    }

    if (!defaultLanguage) {
        defaultLanguage = lang;
    }

    languages[languageFile] = lang;
}

void Localization::setCurrentLanguage(const char* languageFile)
{
    if (hastLanguage(languageFile)) {
        currentLanguage = languages[languageFile];
    } else {
        BOOST_ASSERT_MSG(false, "Language not loaded!");
    }
}

void Localization::setDefaultLanguage(const char* languageFile)
{
    if (hastLanguage(languageFile)) {
        defaultLanguage = languages[languageFile];
    } else {
        BOOST_ASSERT_MSG(false, "Language not loaded!");
    }
}

bool Localization::hastLanguage(const char* languageFile) const
{
    return (languages.count(languageFile) > 0);
}

} // namespace i18n
} // namespace avalon