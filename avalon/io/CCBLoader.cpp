#include "CCBLoader.h"

#include "cocos2d.h"
#include <avalon/utils/ObjectRetainer.h>

using namespace cocos2d;
using namespace cocosbuilder;

namespace avalon {
namespace io {

CCBLoader::CCBLoader(const std::string& ccbFileName)
: ccbFileName(ccbFileName)
, nodeLoaderLibrary(NodeLoaderLibrary::library())
{
}

void CCBLoader::assignAnimationManager(cocosbuilder::CCBAnimationManager** manager)
{
    managerReceiver = manager;
}

void CCBLoader::setBox2dContainer(avalon::physics::Box2dContainer& container)
{
    box2dContainer = &container;
}

cocos2d::Node* CCBLoader::load()
{
    nodeLoaderLibrary->registerDefaultNodeLoaders();

    CCBReader ccbReader(nodeLoaderLibrary.get(), this, nullptr);
    auto node = ccbReader.readNodeGraphFromFile(ccbFileName.c_str(), this);
    auto animationManager = ccbReader.getAnimationManager();

    // Keep the AnimationManager alive, because with CCBLoader::Configuration we
    // pass it to all registered custom classes!
    auto retainer = avalon::utils::ObjectRetainer::create();
    retainer->add(*animationManager);
    node->addChild(retainer);

    if (managerReceiver) {
        *managerReceiver = animationManager;
    }

    for (auto& loader : genericLoaders) {
        loader->dispatchPendingProperties(*animationManager, box2dContainer);
    }

    return node;
}

bool CCBLoader::onAssignCCBMemberVariable(cocos2d::Object* target, const char* memberVariableName, cocos2d::Node* node)
{
    for (auto& pair : nameAssigner) {
        if (pair.first == memberVariableName) {
            for (auto& callback : pair.second) {
                callback(node);
            }
            return true;
        }
    }
    return false;
}

} // namespace io
} // namespace avalon