#include <avalon/utils/OnCleanupCallback.h>

namespace avalon {
namespace utils {

void OnCleanupCallback::cleanup()
{
    auto parent = getParent();
    if (!parent) {
        throw std::runtime_error("Removed but not attached to any node");
    }

    for (auto& cb : callbacks) {
        cb.second(cb.first, *parent);
    }

    cocos2d::Node::cleanup();
}

void OnCleanupCallback::set(const std::string& name, Callback cb)
{
    callbacks[name] = cb;
}

} // namespace utils
} // namespace avalon
