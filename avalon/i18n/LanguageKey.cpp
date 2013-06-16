#include <avalon/i18n/LanguageKey.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/assert.hpp>

namespace avalon {
namespace i18n {

int LanguageKey::count = 0;

LanguageKey::LanguageKey(const char* keyName, const char* keyValue)
: name(keyName)
, value(keyValue)
, parameters()
{
    BOOST_ASSERT_MSG(!name.empty(), "name can't be empty");
}

LanguageKey& LanguageKey::assign(const char* varName, const char* value)
{
    parameters[std::string("{").append(varName).append("}")] = std::string(value);
    return *this;
}

LanguageKey& LanguageKey::assign(const char* varName, int value)
{
    char buffer[25] = {0};
    snprintf(buffer, sizeof(buffer), "%d", value);
    assign(varName, buffer);
    
    return *this;
}

LanguageKey& LanguageKey::assign(const char* varName, float value)
{
    return assign(varName, value, "%.2f");
}

LanguageKey& LanguageKey::assign(const char* varName, float value, const char* format)
{
    if (format) {
        char buffer[25] = {0};
        snprintf(buffer, sizeof(buffer), format, value);
        assign(varName, buffer);
    }
    
    BOOST_ASSERT_MSG(format, "no format given");
    return *this;
}

std::string LanguageKey::get()
{
    if (parameters.empty()) {
        return value;
    }

    std::string formatted = value;

    for (auto& row : parameters) {
        boost::replace_all(formatted, row.first, row.second);
    }
    parameters.clear();

    return formatted;
}

} // namespace i18n
} // namespace avalon