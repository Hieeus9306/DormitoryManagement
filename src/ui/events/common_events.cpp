#include "common_events.hpp"

#include "../ui.hpp"

#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui::events {

ftxui::Component createNavigation(std::vector<std::string>& sections,
                                  int& selectedSection) {
    auto option = ftxui::MenuOption::VerticalAnimated();
    option.entries_option.transform = [](const ftxui::EntryState& entry) {
        auto item = ftxui::hbox({
            ftxui::text(entry.active ? "  >  " : "     "),
            ftxui::text(entry.label) | ftxui::flex,
        });

        if (entry.active) {
            item = item | ftxui::bold | ftxui::color(theme::accent) |
                   ftxui::bgcolor(theme::panelLight);
        } else if (entry.focused) {
            item = item | ftxui::color(ftxui::Color::White) |
                   ftxui::bgcolor(theme::panel);
        } else {
            item = item | ftxui::color(theme::mutedText);
        }

        return item | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3);
    };

    return ftxui::Menu(&sections, &selectedSection, option);
}

ftxui::Component createExit(ftxui::ScreenInteractive& screen) {
    return ftxui::Button("Exit", screen.ExitLoopClosure(),
                         ftxui::ButtonOption::Ascii());
}

}  // namespace ui::events
