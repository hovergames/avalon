#ifndef AVALON_I18N_LANGUAGEKEY_H
#define AVALON_I18N_LANGUAGEKEY_H

#include <map>
#include <string>

namespace avalon {
namespace i18n {

class LanguageKey
{
private:
    LanguageKey() {}

public:
    static int count;
    std::string name;
    std::string value;
    std::map<std::string, std::string> parameters;

    LanguageKey(const char* keyName, const char* keyValue);
    ~LanguageKey() {}

    LanguageKey& assign(const char* varName, const char* value);
    LanguageKey& assign(const char* varName, int value);
    LanguageKey& assign(const char* varName, float value);
    LanguageKey& assign(const char* varName, float value, const char* format);

    std::string get();
};

} // namespace i18n
} // namespace avalon

#endif /* AVALON_I18N_LANGUAGEKEY_H */
