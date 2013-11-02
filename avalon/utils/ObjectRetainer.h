#ifndef AVALON_UTILS_OBJECTRETAINER_H
#define AVALON_UTILS_OBJECTRETAINER_H

#include <list>
#include "cocos2d.h"

namespace avalon {
namespace utils {

class ObjectRetainer : public cocos2d::Node
{
private:
    std::list<cocos2d::Object*> objects;

public:
    CREATE_FUNC(ObjectRetainer);
    ~ObjectRetainer();

    void add(cocos2d::Object& obj);
    void clear();
};

} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_OBJECTRETAINER_H */
