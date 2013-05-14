#ifndef AVALON_IO_INIREADER_H
#define AVALON_IO_INIREADER_H

#include <map>
#include <string>

namespace avalon {
namespace io {

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

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_INIREADER_H */