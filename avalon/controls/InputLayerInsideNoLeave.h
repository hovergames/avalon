#ifndef AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVE_H
#define AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVE_H

#include <avalon/controls/inputfilter.h>
#include <avalon/controls/PolicyInputLayer.h>

namespace avalon {
namespace controls {

using InputLayerInsideNoLeave = PolicyInputLayer<
    InputFilterNotInside,
    InputFilterChain<InputFilterTriggerEnsureBegan<InputFilterNotInside>, InputFilterTriggerEndOnLeave>,
    InputFilterNotPressed,
    InputFilterNotPressed>;

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_INPUTLAYERINSIDENOLEAVE_H */
