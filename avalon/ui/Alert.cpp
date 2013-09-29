#include <avalon/ui/Alert.h>

#include <boost/assert.hpp>
#include "cocos2d.h"

namespace avalon {
namespace ui {

Alert::Alert(Callback delegate)
: title()
, message()
, buttons()
, delegate(delegate)
{
}

void Alert::addButton(const unsigned int index, std::string label)
{
    buttons[index] = label;
}

void Alert::removeButton(const unsigned int index)
{
    buttons.erase(index);
}

std::string Alert::getButtonLabel(const unsigned int index) const
{
    return buttons.at(index);
}

void Alert::show()
{
    showAlert(title, message, buttons, delegate);
}
        
} // namespace ui
} // namespace avalon