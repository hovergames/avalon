#ifndef AVALON_IO_CCBLOADER_H
#define AVALON_IO_CCBLOADER_H

#include "cocos2d.h"
#include <editor-support/cocosbuilder/CocosBuilder.h>

#include <avalon/io/ccbloader/GenericLoaderInterface.h>
#include <avalon/io/ccbloader/GenericLoader.h>
#include <avalon/io/ccbloader/types.h>

namespace avalon {
namespace io {

class CCBLoader
: public cocosbuilder::CCBMemberVariableAssigner
, public cocos2d::Object
{
public:
    using Dictionary = ccbloader::Dictionary;
    using Configuration = ccbloader::Configuration;

private:
    using Assigner = std::function<void(cocos2d::Object*)>;

    std::unordered_map<std::string, Assigner> nameAssigner;
    const std::string ccbFileName;
    std::shared_ptr<cocosbuilder::NodeLoaderLibrary> nodeLoaderLibrary;
    std::list<ccbloader::GenericLoaderInterface*> genericLoaders;
    avalon::physics::Box2dContainer* box2dContainer = nullptr;

public:
    explicit CCBLoader(const std::string& ccbFileName);
    std::shared_ptr<cocos2d::Node> load();
    void setBox2dContainer(avalon::physics::Box2dContainer& container);
    virtual bool onAssignCCBMemberVariable(cocos2d::Object* target, const char* memberVariableName, cocos2d::Node* node);

    template<typename T>
    void assignObject(const std::string& name, T** destination)
    {
        nameAssigner[name] = [destination](cocos2d::Object* object) {
            *destination = dynamic_cast<T*>(object);
            if (!*destination) {
                throw std::invalid_argument("Wrong Object type");
            }
        };
    }

    template<typename T, typename L>
    void registerCustomClass(const std::string& className)
    {
        auto loader = ccbloader::GenericLoader<T, L>::loader();
        genericLoaders.push_back(loader);
        nodeLoaderLibrary->registerNodeLoader(className.c_str(), loader);
    }
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_CCBLOADER_H */