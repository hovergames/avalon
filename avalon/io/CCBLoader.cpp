#include "CCBLoader.h"

#include "cocos2d.h"
#include <avalon/utils/ObjectRetainer.h>
#include <avalon/io/ccbloader/PhysicSpriteLoader.h>

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

    if (box2dContainer) {
        auto loader = ccbloader::PhysicSpriteLoader::loader();
        genericLoaders.push_back(loader);
        nodeLoaderLibrary->registerNodeLoader("PhysicsSprite", loader);
    }

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

void CCBLoader::ensureAssignedObjects()
{
    for (auto& pair : nameAssigner) {
        auto& name = pair.first;
        if (!assignedNames.count(name)) {
            throw new std::runtime_error(name + " not assigned!");
        }
    }
}

bool CCBLoader::onAssignCCBMemberVariable(cocos2d::Object* target, const char* memberVariableName, cocos2d::Node* node)
{
    for (auto& pair : nameAssigner) {
        if (pair.first == memberVariableName) {
            for (auto& callback : pair.second) {
                callback(node);
            }
            assignedNames.insert(pair.first);
            return true;
        }
    }
    return false;
}

} // namespace io
} // namespace avalon