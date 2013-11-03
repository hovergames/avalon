#ifndef AVALON_CONTROLS_INPUTFILTER_H
#define AVALON_CONTROLS_INPUTFILTER_H

#include "cocos2d.h"
#include <avalon/controls/InputLayer.h>

namespace avalon {
namespace controls {

class InputFilterAlways
{
public:
    static bool isFilteredBegan(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return true; }
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return true; }
    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return true; }
    static bool isFilteredCancelled(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event) { return true; }
};

class InputFilterPressed
{
public:
    static bool isFilteredBegan(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isPressed(); }
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isPressed(); }
    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isPressed(); }
    static bool isFilteredCancelled(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event) { return layer.isPressed(); }
};

class InputFilterInside
{
public:
    static bool isFilteredBegan(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isTouchInside(*touch); }
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isTouchInside(*touch); }
    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return layer.isTouchInside(*touch); }
    static bool isFilteredCancelled(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event) { return layer.isTouchInside(*touch); }
};

template<typename Filter>
class InputFilterNot
{
public:
    static bool isFilteredBegan(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return !Filter::isFilteredBegan(layer, touch, event); }
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return !Filter::isFilteredMoved(layer, touch, event); }
    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)     { return !Filter::isFilteredEnded(layer, touch, event); }
    static bool isFilteredCancelled(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event) { return !Filter::isFilteredCancelled(layer, touch, event); }
};

template<typename Filter>
class InputFilterTriggerEndOnMoved
{
public:
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (Filter::isFilteredMoved(layer, touch, event)) {
            return true;
        }

        layer.onTouchLayerEnded(touch, event);
        return true;
    }
};

template<typename Filter>
class InputFilterTriggerEnsureBegan
{
public:
    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (Filter::isFilteredMoved(layer, touch, event)) {
            return true;
        }

        if (!layer.isPressed()) {
            layer.onTouchLayerBegan(touch, event);
            return true;
        }

        return false;
    }
};

template<typename Filter>
class InputFilterTriggerCancelled
{
public:
    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (Filter::isFilteredEnded(layer, touch, event)) {
            return true;
        }

        if (!layer.isTouchInside(*touch)) {
            layer.onTouchLayerCancelled(touch, event);
            return true;
        }

        return false;
    }
};

template<typename A, typename B>
class InputFilterChain
{
public:
    static bool isFilteredBegan(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto a = A::isFilteredBegan(layer, touch, event);
        auto b = B::isFilteredBegan(layer, touch, event);
        return (a || b);
    }

    static bool isFilteredMoved(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto a = A::isFilteredMoved(layer, touch, event);
        auto b = B::isFilteredMoved(layer, touch, event);
        return (a || b);
    }

    static bool isFilteredEnded(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto a = A::isFilteredEnded(layer, touch, event);
        auto b = B::isFilteredEnded(layer, touch, event);
        return (a || b);

    }

    static bool isFilteredCancelled(avalon::controls::InputLayer& layer, cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto a = A::isFilteredCancelled(layer, touch, event);
        auto b = B::isFilteredCancelled(layer, touch, event);
        return (a || b);
    }
};

using InputFilterNone = InputFilterNot<InputFilterAlways>;
using InputFilterNotPressed = InputFilterNot<InputFilterPressed>;
using InputFilterNotInside = InputFilterNot<InputFilterInside>;
using InputFilterTriggerEndOnLeave = InputFilterTriggerEndOnMoved<InputFilterInside>;

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_INPUTFILTER_H */
