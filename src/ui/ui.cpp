#include "ui.hpp"

#include "events/common_events.hpp"
#include "events/contract_event.hpp"
#include "events/invoice_event.hpp"
#include "events/room_event.hpp"
#include "events/search_event.hpp"
#include "events/student_event.hpp"

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {
namespace {

ftxui::Element buildSectionView(Section section,
                                const std::string& searchText) {
    switch (section) {
        case Section::Students:
            return events::buildStudentView(searchText);
        case Section::Rooms:
            return events::buildRoomView(searchText);
        case Section::Contracts:
            return events::buildContractView(searchText);
        case Section::Invoices:
            return events::buildInvoiceView(searchText);
    }

    return ftxui::text("Unknown section");
}

}  // namespace

int run() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();
    int selectedSection = 0;
    std::string searchText;
    std::vector<std::string> sections = {
        "Students",
        "Rooms",
        "Contracts",
        "Invoices",
    };
    const std::vector<std::string> sectionDescriptions = {
        "students",
        "rooms",
        "contracts",
        "invoices",
    };

    auto navigation = events::createNavigation(sections, selectedSection);
    auto search = events::createSearch(searchText);
    auto exit = events::createExit(screen);

    auto controls = Container::Vertical({search});
    auto sidebar = Container::Vertical({navigation, exit});
    auto layout = Container::Horizontal({sidebar, controls});

    auto application = Renderer(layout, [&] {
        const std::string& sectionName = sections[selectedSection];
        const auto section = static_cast<Section>(selectedSection);

        auto sidebarElement =
            vbox({
                vbox({
                    text("DORMITORY") | bold | color(theme::accent),
                    text("MANAGEMENT") | bold,
                    text("Student housing system") | color(theme::mutedText),
                }) | center |
                    size(HEIGHT, EQUAL, 6),
                separator(),
                text("  NAVIGATION") | color(theme::mutedText),
                navigation->Render() | flex,
                separator(),
                exit->Render() | center,
                text("  Arrow keys: Move  Enter: Select") |
                    color(theme::mutedText),
            }) |
            bgcolor(theme::sidebar) | size(WIDTH, EQUAL, 30);

        auto mainElement =
            vbox({
                hbox({
                    vbox({
                        text(sectionName) | bold | size(HEIGHT, EQUAL, 1),
                        text("View and manage " +
                             sectionDescriptions[selectedSection] +
                             " information") |
                            color(theme::mutedText),
                    }) | flex,
                    text("Dormitory Management System") |
                        color(theme::mutedText),
                }),
                separator(),
                hbox({
                    text(" Search  ") | bold,
                    search->Render() | flex,
                }) | border |
                    bgcolor(theme::panelLight),
                separatorEmpty(),
                buildSectionView(section, searchText) | flex,
            }) |
            border | bgcolor(theme::panel) | flex;

        return hbox({sidebarElement, mainElement}) | bgcolor(theme::panel) |
               color(Color::White);
    });

    screen.Loop(application);
    return 0;
}

}  // namespace ui
