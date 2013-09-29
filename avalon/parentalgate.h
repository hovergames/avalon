#ifndef AVALON_PARENTALGATE_H
#define AVALON_PARENTALGATE_H

#include <functional>

namespace avalon {
namespace parentalgate {

using Callback = std::function<void()>;

void show(Callback successCallback, Callback failureCallback = nullptr);

} // namespace parentalgate
} // namespace avalon

#endif /* AVALON_PARENTALGATE_H */