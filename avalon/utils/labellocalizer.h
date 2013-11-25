#ifndef AVALON_UTILS_LABELLOCALIZER_H
#define AVALON_UTILS_LABELLOCALIZER_H

#include "cocos2d.h"

#include "labellocalizer.h"
#include <boost/algorithm/string.hpp>
#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>

namespace avalon {
namespace utils {
namespace labellocalizer {

void localize(const cocos2d::Node& node);

} // namespace labellocalizer
} // namespace utils
} // namespace avalon

#endif /* defined(AVALON_UTILS_LABELLOCALIZER_H) */
