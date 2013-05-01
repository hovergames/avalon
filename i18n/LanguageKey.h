//
//  LanguageKey.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#ifndef __Adventures_on_the_farm__LanguageKey__
#define __Adventures_on_the_farm__LanguageKey__

#include <map>
#include <string>

class LanguageKey
{
    LanguageKey() {}
public:
    static int count;
    std::string name;
    std::string value;
    std::map<std::string, std::string> parameters;

    LanguageKey(const char *keyName, const char *keyValue);
    ~LanguageKey() {}

    LanguageKey* assign(const char *varName, const char *value);
    LanguageKey* assign(const char *varName, int value);
    LanguageKey* assign(const char *varName, float value);

    const char* getCString();
};

#endif /* defined(__Adventures_on_the_farm__LanguageKey__) */