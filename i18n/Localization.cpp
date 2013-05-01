//
//  Localization.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#include "Localization.h"
#include "LanguageKey.h"
#include <cassert>
#include <string>
#include "StringHelper.h"
#include "Language.h"
#include "Helpers.h"
#include "IniReader.h"

using namespace std;
using namespace cocos2d;
using namespace Helpers;

Localization::Localization() :
    currentLanguage(NULL),
    defaultLanguage(NULL)
{
}

Localization::~Localization()
{
    clear();
}


void Localization::clear()
{
    for (std::map<string, Language*>::iterator it = languages.begin(); it != languages.end(); ++it)
    {
        safeDelete(it->second);
    }

    currentLanguage = NULL;
    defaultLanguage = NULL;
}

LanguageKey* _(const char *section, const char *key)
{
    assert(Localization::getInstance().currentLanguage && "currentLanguage is not set!");
    LanguageKey *langKey = Localization::getInstance().currentLanguage->getLanguageKey(section, key);
    if (!langKey || langKey->name.empty())
    {
        CCLog("WARNING! Language Key not found in current Language '%s': %s/%s", Localization::getInstance().currentLanguage->fileName.c_str(), section, key);
        langKey = Localization::getInstance().defaultLanguage->getLanguageKey(section, key);
        if (!langKey || langKey->name.empty()) {
            if (!langKey)
            {
                langKey = Localization::getInstance().currentLanguage->addLanguageKey(section, key,  (string("_") + string(key)).c_str());
            } else {
                CCLog("Set temporary key");
                langKey->value = (string("_") + string(key)).c_str();
            }
            CCLog("WARNING! Language Key not found in default Language: %s/%s", section, key);
        }
    }
    return langKey;
}

void Localization::addLanguage(const char *languageFile)
{
    Language *lang = new Language();
    lang->fileName = languageFile;

    IniReader file;
    file.loadFile(languageFile);

    CCLog("Start ini reader!");
    IniReader::iniSections *sections = file.getSections();
    for (IniReader::iniSections::iterator it = sections->begin(); it != sections->end(); ++it)
    {
        for (IniReader::iniKeys::iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
        {
            lang->addLanguageKey(it->first.c_str(), innerIt->first.c_str(), innerIt->second.c_str());
        }
    }

    if (!currentLanguage)
        currentLanguage = lang;

    if (!defaultLanguage)
        defaultLanguage = lang;

    languages[languageFile] = lang;
}

void Localization::setCurrentLanguage(const char *languageFile) {
    assert(languages.find(languageFile)->second && "Language not loaded!");
    currentLanguage = languages[languageFile];
}

void Localization::setDefaultLanguage(const char *languageFile)
{
    assert(languages.find(languageFile)->second && "Language not loaded!");
    defaultLanguage = languages[languageFile];
}
