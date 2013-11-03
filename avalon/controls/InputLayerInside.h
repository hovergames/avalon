#ifndef AVALON_CONTROLS_INPUTLAYERINSIDE_H
#define AVALON_CONTROLS_INPUTLAYERINSIDE_H

#include <avalon/controls/inputfilter.h>
#include <avalon/controls/PolicyInputLayer.h>

namespace avalon {
namespace controls {

using InputLayerInside = PolicyInputLayer<
    InputFilterNotInside,
    InputFilterNotInside,
    InputFilterNotPressed,
    InputFilterNotPressed>;

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_INPUTLAYERINSIDE_H */
