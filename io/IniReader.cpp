//
//  IniReader.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "IniReader.h"

#include <string>
#include "cocos2d.h";
#include <iostream>
#include "StringHelper.h"
#include "Helpers.h"

void IniReader::loadFile(const char *iniFile)
{
    unsigned long fileSize = 0;
    unsigned char * fileContents = NULL;
    std::string line, fullPath, contents;

    fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(iniFile);

    fileContents = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str( ) , "rb", &fileSize);

    contents.assign(fileContents, fileContents + fileSize - 1);

    std::istringstream fileStringStream(contents);

    std::string currentSection;

    while (std::getline(fileStringStream, line))
    {
        int pos;

        // Remove comments
        pos = line.find(";");
        if (pos >= 0) {
            line.erase(pos, line.length());
        }

        if (line.empty())
            continue;

        // Check for section
        pos = line.find("[");
        if (pos == 0) {
            int sectionEnd = line.find("]");
            assert(sectionEnd > pos && "Invalid Section!");
            if (sectionEnd > pos)
            {
                line.erase(0, pos + 1);
                line.erase(sectionEnd - 1, line.length());
                StringHelper::trim(line);
                currentSection = line;
                continue;
            }
        }

        assert(!currentSection.empty() && "Key without section found");

        pos = line.find("=");
        if (pos >= 0)
        {
            std::string key(line);
            key.erase(pos, line.length());

            std::string value(line);
            value.erase(0, pos + 1);

            StringHelper::trim(key);
            StringHelper::trim(value);

            sections[currentSection][key] = value;
        }
    }
    if (fileContents != NULL) {
        Helpers::safeDeleteArray(fileContents);
    }
}

int IniReader::getValueAsInt(const char *section, const char *key)
{
    std::string val = getValue(section, key);
    return atoi(val.c_str());
}

double IniReader::getValueAsDouble(const char *section, const char *key)
{
    std::string val = getValue(section, key);
    return atof(val.c_str());
}

float IniReader::getValueAsFloat(const char *section, const char *key)
{
    return float(getValueAsDouble(section, key));
}

const char * IniReader::getValue(const char *section, const char *key)
{
    return sections[section][key].c_str();
}

IniReader::iniKeys* IniReader::getSection(const char *section)
{
    return &(sections[section]);
}

bool IniReader::doesSectionExist(const char *section)
{
    return sections.find(section) != sections.end();
}


IniReader::iniSections* IniReader::getSections()
{
    return &sections;
}


