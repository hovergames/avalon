#ifndef AVALON_UTILS_ONCLEANUPCALLBACK_H
#define AVALON_UTILS_ONCLEANUPCALLBACK_H

#include <map>
#include "cocos2d.h"

namespace avalon {
namespace utils {

class OnCleanupCallback : public cocos2d::Node
{
public:
    using Callback = std::function<void(const std::string& name, cocos2d::Node&)>;

private:
    std::map<std::string, Callback> callbacks;

public:
    CREATE_FUNC(OnCleanupCallback);

    virtual void cleanup() override;
    void set(const std::string& name, Callback cb);
};

} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_ONCLEANUPCALLBACK_H */
