#ifndef AVALON_IO_GENERICLOADER_H
#define AVALON_IO_GENERICLOADER_H

#include "cocos2d.h"
#include <editor-support/cocosbuilder/CocosBuilder.h>
#include <avalon/io/GenericLoaderInterface.h>

namespace avalon {
namespace io {

template<typename T, typename L>
class GenericLoader : public L, public GenericLoaderInterface
{
private:
    std::map<T*, Dictionary> nodeSettings;

    T* getTarget(cocos2d::Node* node)
    {
        auto target = dynamic_cast<T*>(node);
        if (!target) {
            throw std::invalid_argument("Node is invalid!");
        }
        return target;
    }

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GenericLoader, loader);

    virtual void parseProperties(cocos2d::Node* pNode, cocos2d::Node* pParent, cocosbuilder::CCBReader* pCCBReader)
    {
        auto target = getTarget(pNode);
        if (!nodeSettings.count(target)) {
            nodeSettings[target] = {};
        }

        L::parseProperties(pNode, pParent, pCCBReader);
    }

    virtual void onHandlePropTypeString(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, const char* value, cocosbuilder::CCBReader* pCCBReader) override
    {
        nodeSettings[getTarget(pNode)][pPropertyName] = std::string(value);
    }

    virtual void onHandlePropTypeInteger(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, int value, cocosbuilder::CCBReader* pCCBReader) override
    {
        nodeSettings[getTarget(pNode)][pPropertyName] = value;
    }

    virtual void onHandlePropTypeFloat(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float value, cocosbuilder::CCBReader* pCCBReader) override
    {
        nodeSettings[getTarget(pNode)][pPropertyName] = value;
    }

    virtual void onHandlePropTypeCheck(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, bool value,  cocosbuilder::CCBReader* pCCBReader) override
    {
        nodeSettings[getTarget(pNode)][pPropertyName] = value;
    }

    virtual void dispatchPendingProperties(avalon::physics::Box2dContainer* box2dContainer) override
    {
        for (auto& pair : nodeSettings) {
            Configuration config{pair.second, box2dContainer};
            pair.first->onConfiguration(config);
        }
    }

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(T);
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_GENERICLOADER_H */