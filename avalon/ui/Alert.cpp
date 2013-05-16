#include <avalon/ui/Alert.h>

#include <boost/assert.hpp>

namespace avalon {
namespace ui {

Alert::Alert(AlertDelegate* delegate)
: title()
, message()
, buttons()
, delegate(delegate)
{
}

Alert::Alert(const char* title, const char* message, const char* dismissButton, AlertDelegate* delegate)
: title(title)
, message(message)
, buttons()
, delegate(delegate)
{
    addButton(0, dismissButton);
}

Alert::~Alert()
{
}

std::string Alert::getTitle() const
{
    return title;
}

void Alert::setTitle(const char* title)
{
    this->title = title;
}

std::string Alert::getMessage() const
{
    return message;
}

void Alert::setMessage(const char* message)
{
    this->message = message;
}

void Alert::addButton(const unsigned int index, const char* label)
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

AlertDelegate* Alert::getDelegate() const
{
    return delegate;
}

void Alert::setDelegate(AlertDelegate* delegate)
{
    this->delegate = delegate;
}

void Alert::show() const
{
    showAlert(title, message, buttons, delegate);
}
        
} // namespace ui
} // namespace avalon