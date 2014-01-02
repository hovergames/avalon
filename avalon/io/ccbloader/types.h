#ifndef AVALON_IO_CCBLOADER_TYPES_H
#define AVALON_IO_CCBLOADER_TYPES_H

#include <string>
#include <map>
#include <boost/any.hpp>
#include "cocosbuilder/CocosBuilder.h"

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace io {
namespace ccbloader {

using Dictionary = cocos2d::ValueMap;
struct Configuration
{
    const Dictionary& settings;
    cocosbuilder::CCBAnimationManager& animationManager;
    avalon::physics::Box2dContainer* box2dContainer;
};

} // namespace ccbloader
} // namespace io
} // namespace avalon

#endif /* AVALON_IO_CCBLOADER_TYPES_H */