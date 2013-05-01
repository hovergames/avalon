//
//  IniReader.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__IniReader__
#define __Adventures_on_the_farm__IniReader__

#include <map>

class IniReader
{
public:
    typedef std::map<std::string, std::string> iniKeys;
    typedef std::map<std::string, iniKeys> iniSections;

    iniSections sections;

    IniReader() {}
    ~IniReader() {}

    void loadFile(const char *fileName);

    int getValueAsInt(const char *section, const char *key);
    double getValueAsDouble(const char *section, const char *key);
    float getValueAsFloat(const char *section, const char *key);
    const char * getValue(const char *section, const char *key);
    bool doesSectionExist(const char *section);
    iniKeys* getSection(const char *section);
    iniSections* getSections();
};
#endif /* defined(__Adventures_on_the_farm__IniReader__) */
