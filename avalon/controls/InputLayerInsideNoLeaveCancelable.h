#ifndef AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVECANCELABLE_H
#define AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVECANCELABLE_H

#include <avalon/controls/inputfilter.h>
#include <avalon/controls/PolicyInputLayer.h>

namespace avalon {
namespace controls {

using InputLayerInsideNoLeaveCancelable = PolicyInputLayer<
    InputFilterNotInside,
    InputFilterNotPressed,
    InputFilterChain<InputFilterNot<InputFilterTriggerCancelled<InputFilterInside>>, InputFilterNotPressed>,
    InputFilterNotPressed>;

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVECANCELABLE_H */
