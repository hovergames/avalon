#ifndef AVALON_UI_PARENTALGATE_H
#define AVALON_UI_PARENTALGATE_H

#include <functional>

namespace avalon {
namespace ui {
namespace parentalgate {

using Callback = std::function<void()>;

void show(Callback successCallback, Callback failureCallback = nullptr);
void showOnlyIos(Callback successCallback, Callback failureCallback = nullptr);

} // namespace parentalgate
} // namespace ui
} // namespace avalon

#endif /* AVALON_UI_PARENTALGATE_H */