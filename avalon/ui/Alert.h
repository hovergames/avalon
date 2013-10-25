#ifndef AVALON_UI_ALERT_H
#define AVALON_UI_ALERT_H

#include <string>
#include <map>
#include <functional>

namespace avalon {
namespace ui {

class Alert
{
public:
    using ButtonList = std::map<const unsigned int, std::string>;
    using Callback = std::function<void(const unsigned int, const std::string)>;

    std::string title;
    std::string message;
    Callback delegate;

    Alert();
    explicit Alert(Callback delegate);

    void addButton(const unsigned int index, const std::string label);
    void removeButton(const unsigned int index);
    std::string getButtonLabel(const unsigned int index) const;

    void show();

private:
    ButtonList buttons;
};

// Only this function has to be implemented in native code and NOT the class
// above! Why? Because Alert is simply just a OOP interface for this function.
// You could, for instance, reuse and re-show a single alert multiple times. On
// the other hand the interface with the native code should be as simple as
// possible and we'd like to stick in C++ as long as possible too.
void showAlert(const std::string& title, const std::string& message, const Alert::ButtonList& buttons, Alert::Callback& delegate);

} // namespace ui
} // namespace avalon

#endif /* AVALON_UI_ALERT_H */
