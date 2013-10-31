#ifndef AVALON_IO_GENERICLOADER_H
#define AVALON_IO_GENERICLOADER_H

#include "cocos2d.h"
#include <editor-support/cocosbuilder/CocosBuilder.h>
#include <avalon/io/GenericLoaderInterface.h>
#include <boost/any.hpp>

namespace avalon {
namespace io {

template<typename T, typename L>
class GenericLoader : public L, public GenericLoaderInterface
{
private:
    T* target = nullptr;
    std::map<std::string, boost::any> properties;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GenericLoader, loader);

    virtual void parseProperties(cocos2d::Node* node, cocos2d::Node* parent, cocosbuilder::CCBReader* reader)
    {
        target = dynamic_cast<T*>(node);
        if (!target) throw std::invalid_argument("Node is invalid!");

        L::parseProperties(node, parent, reader);
    }

    virtual void onHandlePropTypeString(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, const char* value, cocosbuilder::CCBReader* pCCBReader) override
    {
        properties[pPropertyName] = std::string(value);
    }

    virtual void onHandlePropTypeInteger(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, int value, cocosbuilder::CCBReader* pCCBReader) override
    {
        properties[pPropertyName] = value;
    }

    virtual void onHandlePropTypeFloat(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float value, cocosbuilder::CCBReader* pCCBReader) override
    {
        properties[pPropertyName] = value;
    }

    virtual void onHandlePropTypeCheck(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, bool value,  cocosbuilder::CCBReader* pCCBReader) override
    {
        properties[pPropertyName] = value;
    }

    virtual void dispatchPendingProperties() override
    {
        target->onCCBConfiguration(properties);
    }

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(T);
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_GENERICLOADER_H */