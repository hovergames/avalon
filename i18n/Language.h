//
//  Language.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#ifndef __Adventures_on_the_farm__Language__
#define __Adventures_on_the_farm__Language__

#include <map>
#include <string>

class LanguageKey;

class Language
{
public:
    std::string fileName;
    
    Language() {};
    ~Language();

    std::map<std::string, std::map<std::string, LanguageKey*> >sections;

    LanguageKey* addLanguageKey(const char *section, const char *varName, const char* value);
    LanguageKey* getLanguageKey(const char *section, const char *varName);
};

#endif /* defined(__Adventures_on_the_farm__Language__) */
