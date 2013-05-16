#ifndef AVALON_UI_ALERTDELEGATE_H
#define AVALON_UI_ALERTDELEGATE_H

#include <string>

namespace avalon {
namespace ui {

class AlertDelegate
{
public:
    virtual void onAlertButtonClick(const unsigned int index, const std::string title) = 0;
};

} // namespace ui
} // namespace avalon

#endif /* AVALON_UI_ALERTDELEGATE_H */