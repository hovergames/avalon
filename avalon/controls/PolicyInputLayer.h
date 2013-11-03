#ifndef AVALON_CONTROLS_POLICYINPUTLAYER_H
#define AVALON_CONTROLS_POLICYINPUTLAYER_H

#include "cocos2d.h"
#include <avalon/controls/InputLayer.h>

namespace avalon {
namespace controls {

template<typename BeganFilter, typename MovedFilter, typename EndedFilter, typename CancelledFilter>
class PolicyInputLayer : public InputLayer
{
protected:
    virtual bool isFilteredBegan(cocos2d::Touch* touch, cocos2d::Event* event) override
    {
        return BeganFilter::isFilteredBegan(*this, touch, event);
    }

    virtual bool isFilteredMoved(cocos2d::Touch* touch, cocos2d::Event* event) override
    {
        return MovedFilter::isFilteredMoved(*this, touch, event);
    }

    virtual bool isFilteredEnded(cocos2d::Touch* touch, cocos2d::Event* event) override
    {
        return EndedFilter::isFilteredEnded(*this, touch, event);
    }

    virtual bool isFilteredCancelled(cocos2d::Touch* touch, cocos2d::Event* event) override
    {
        return CancelledFilter::isFilteredCancelled(*this, touch, event);
    }

public:
    CREATE_FUNC(PolicyInputLayer);
};

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_POLICYINPUTLAYER_H */
