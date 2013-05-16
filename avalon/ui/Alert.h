#ifndef AVALON_UI_ALERT_H
#define AVALON_UI_ALERT_H

#include <string>
#include <map>

namespace avalon {
namespace ui {

class AlertDelegate;
typedef std::map<unsigned int, std::string> ButtonList;

class Alert
{
public:
    Alert(AlertDelegate* delegate = NULL);
    Alert(const char* title, const char* message, const char* dismissButton, AlertDelegate* delegate = NULL);
    ~Alert();

    std::string getTitle() const;
    void setTitle(const char* title);

    std::string getMessage() const;
    void setMessage(const char* message);

    void addButton(const unsigned int index, const char* label);
    void removeButton(const unsigned int index);
    std::string getButtonLabel(const unsigned int index) const;

    AlertDelegate* getDelegate() const;
    void setDelegate(AlertDelegate* delegate);

    void show() const;

private:
    std::string title;
    std::string message;
    ButtonList buttons;
    AlertDelegate* delegate;
};

// Only this function has to be implemented in native code and NOT the class
// above! Why? Because Alert is simply just a OOP interface for this function.
// You could, for instance, reuse and re-show a single alert multiple times. On
// the other hand the interface with the native code should be as simple as
// possible and we'd like to stick in C++ as long as possible too.
void showAlert(const std::string& title, const std::string& message, const ButtonList& buttons, AlertDelegate* delegate);

} // namespace ui
} // namespace avalon

#endif /* AVALON_UI_ALERT_H */