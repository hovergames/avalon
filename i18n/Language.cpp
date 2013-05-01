//
//  Language.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#include "Language.h"
#include "Helpers.h"
#include "LanguageKey.h"

using namespace Helpers;
using namespace std;

Language::~Language() {
    for (std::map<std::string, std::map<std::string, LanguageKey*> >::iterator it = sections.begin(); it != sections.end(); ++it)
    {
        for (std::map<std::string, LanguageKey*>::iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
        {
            safeDelete(innerIt->second);
        }
    }
}


LanguageKey* Language::addLanguageKey(const char *section, const char *varName, const char* value)
{
    if (sections[section][varName])
    {
        safeDelete(sections[section][varName]);
    }
    
    sections[section][varName] = new LanguageKey(varName, value);
    return sections[section][varName];
}

LanguageKey* Language::getLanguageKey(const char *section, const char *varName)
{
    return sections[section][varName];
}