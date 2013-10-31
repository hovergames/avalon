#include "CCBLoader.h"

#include "cocos2d.h"
#include <avalon/utils/cocos.h>

using namespace cocos2d;
using namespace cocosbuilder;

namespace avalon {
namespace io {

CCBLoader::CCBLoader(std::string ccbFileName)
: ccbFileName(ccbFileName)
, nodeLoaderLibrary(NodeLoaderLibrary::library())
{
}

void CCBLoader::setBox2dContainer(avalon::physics::Box2dContainer& container)
{
    box2dContainer = &container;
}

std::shared_ptr<cocos2d::Node> CCBLoader::load()
{
    nodeLoaderLibrary->registerDefaultNodeLoaders();

    CCBReader ccbReader(nodeLoaderLibrary.get(), this, nullptr);
    auto node = ccbReader.readNodeGraphFromFile(ccbFileName.c_str(), this);

    for (auto& loader : genericLoaders) {
        loader->dispatchPendingProperties(box2dContainer);
    }

    return std::shared_ptr<Node>(node);
}

bool CCBLoader::onAssignCCBMemberVariable(cocos2d::Object* target, const char* memberVariableName, cocos2d::Node* node)
{
    for (auto& pair : nameAssigner) {
        if (pair.first == memberVariableName) {
            pair.second(node);
            return true;
        }
    }
    return false;
}

} // namespace io
} // namespace avalon