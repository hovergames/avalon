#include <avalon/io/utils.h>

#include "cocos2d.h"

namespace avalon {
namespace io {
namespace utils {

std::string getStringFromFile(const std::string& file)
{
    long size = 0;
    unsigned char* data = cocos2d::FileUtils::getInstance()->getFileData(file.c_str(), "r", &size);
    if (size == 0) {
        throw new std::invalid_argument("Unable to load from file");
    }

    std::string result(reinterpret_cast<const char*>(data));
    delete[] data;
    return result;
}

} // namespace utils
} // namespace io
} // namespace avalon
