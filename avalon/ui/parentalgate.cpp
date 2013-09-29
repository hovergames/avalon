#include <avalon/ui/parentalgate.h>

#include <algorithm>
#include <random>
#include <avalon/ui/Alert.h>
#include "cocos2d.h"

namespace avalon {
namespace ui {
namespace parentalgate {

void show(Callback successCallback, Callback failureCallback)
{
    auto dice = std::bind(
        std::uniform_int_distribution<>{1, 5},
        std::mt19937(std::random_device{}())
    );

    const auto a = dice();
    const auto b = dice();
    const auto right = a + b;
    auto wrong = dice() + dice();
    if (wrong == right) {
        ++wrong;
    }

    unsigned int rightIndex = 1;
    unsigned int wrongIndex = 2;
    if (dice() >= 3) {
        std::swap(rightIndex, wrongIndex);
    }

    auto callback = [rightIndex, wrongIndex, successCallback, failureCallback](const unsigned int index, const std::string label) {
        if (successCallback && index == rightIndex) {
            successCallback();
        } else if (failureCallback && index == wrongIndex) {
            failureCallback();
        } else {
            // cancel is just ignored
        }
    };

    avalon::ui::Alert alert(callback);
    alert.message = std::to_string(a) + " + " + std::to_string(b) + " = ?";
    alert.addButton(rightIndex, std::to_string(right));
    alert.addButton(wrongIndex, std::to_string(wrong));

    const auto language = cocos2d::Application::getInstance()->getCurrentLanguage();
    if (language == cocos2d::LanguageType::GERMAN) {
        alert.title = "Kindersicherung";
        alert.addButton(0, "Abbrechen");
    } else {
        alert.title = "Are You An Adult?";
        alert.addButton(0, "Cancel");
    }

    alert.show();
}

} // namespace parentalgate
} // namespace ui
} // namespace avalon
