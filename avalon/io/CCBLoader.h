#ifndef AVALON_IO_CCBLOADER_H
#define AVALON_IO_CCBLOADER_H

#include "cocos2d.h"
#include <avalon/io/GenericLoaderInterface.h>
#include <avalon/io/GenericLoader.h>
#include <editor-support/cocosbuilder/CCBSelectorResolver.h>
#include <editor-support/cocosbuilder/CCBMemberVariableAssigner.h>
#include <editor-support/cocosbuilder/CCNodeLoaderLibrary.h>
#include <editor-support/cocosbuilder/CocosBuilder.h>
#include <boost/any.hpp>

namespace avalon {
namespace io {

class CCBLoader
: public cocosbuilder::CCBMemberVariableAssigner
, public cocos2d::Object
{
private:
    typedef std::function<void(cocos2d::Object*)> Assigner;
    std::map<std::string, Assigner> nameAssigner;
    const std::string ccbFileName;
    std::shared_ptr<cocosbuilder::NodeLoaderLibrary> nodeLoaderLibrary;
    std::list<GenericLoaderInterface*> genericLoaders;

public:
    CCBLoader(const std::string ccbFileName);
    std::shared_ptr<cocos2d::Node> load();
    
    template<typename T>
    void assignObject(const std::string& name, T** destination)
    {
        nameAssigner[name] = [destination](cocos2d::Object* object) {
            *destination = dynamic_cast<T*>(object);
            if (!destination) throw std::invalid_argument("Wrong Object type");
        };
    }

    template<typename T, typename L>
    void registerCustomClass(const std::string& className)
    {
        auto loader = GenericLoader<T, L>::loader();
        genericLoaders.push_back(loader);
        nodeLoaderLibrary->registerNodeLoader(className.c_str(), loader);
    }

    /* CCB Member Assigner */
    virtual bool onAssignCCBMemberVariable(cocos2d::Object* target, const char* memberVariableName, cocos2d::Node* node);
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_CCBLOADER_H */