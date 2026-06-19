#include "ui.hpp"

#include "Theme.hpp"
#include "events/action_event.hpp"
#include "events/common_events.hpp"
#include "events/search_event.hpp"

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {

int run() {
    using namespace ftxui;

    auto screen          = ScreenInteractive::Fullscreen();
    int  selectedSection = 0;
    int  contentMode     = 0;
    auto searchState     = events::createSearchState();

    std::vector<std::string> sections = {
        "Students", "Rooms", "Contracts", "Invoices", "Report",
    };
    const std::vector<std::string> sectionDescriptions = {
        "students", "rooms", "contracts", "invoices", "reports",
    };

    auto search = events::createSearch(searchState, selectedSection);
    auto exit   = events::createExit(screen);
    auto businessActions =
        events::createBusinessActions(selectedSection, contentMode);
    auto navigation = events::createNavigation(
        sections, selectedSection, contentMode, businessActions.sectionMenus);
    auto dashboard = events::createDashboard(selectedSection, searchState);

    auto content =
        Container::Tab({dashboard, businessActions.form}, &contentMode);
    auto controls = Container::Vertical({search, content});
    auto sidebar  = Container::Vertical({navigation, exit});
    auto layout   = Container::Horizontal({sidebar, controls});

    auto application = Renderer(layout, [&] {
        const std::string& sectionName = sections[selectedSection];
        const std::string& desc        = sectionDescriptions[selectedSection];

        // ── Sidebar ──
        auto sidebarElement =
            vbox(Elements{
                // Brand header
                vbox(Elements{
                    text("  \u25CB") | color(theme::accent) |
                        size(HEIGHT, EQUAL, 1),
                    text("DORMITORY") | bold | color(theme::accent),
                    text("MANAGEMENT") | bold | color(Color::White),
                    text("Student housing") | color(theme::mutedText) | dim,
                }) | center |
                    size(HEIGHT, EQUAL, 7),
                theme::divider(),
                // Navigation
                text("  NAVIGATION") | color(theme::mutedText) | bold | dim,
                theme::spacer(1),
                navigation->Render() | flex,
                theme::divider(),
                // Exit
                exit->Render() | center,
                theme::helpText("Arrow: Move | Enter: Select") | center,
                theme::spacer(1),
            }) |
            bgcolor(theme::sidebar) | size(WIDTH, EQUAL, theme::sidebarWidth);

        // ── Main content ──
        auto mainElement =
            vbox(Elements{
                // Top bar: section title + system name
                hbox(Elements{
                    text("  ") | size(WIDTH, EQUAL, 2),
                    vbox(Elements{
                        text(sectionName) | bold | color(Color::White) |
                            size(HEIGHT, EQUAL, 1),
                        text("Manage " + desc) | color(theme::mutedText) | dim,
                    }) | flex,
                    theme::pillLabel("v1.0") | size(WIDTH, EQUAL, 6),
                    text("  ") | size(WIDTH, EQUAL, 2),
                }) | bgcolor(theme::panelDark) |
                    size(HEIGHT, EQUAL, 3),
                // Search bar
                hbox(Elements{
                    text(" ") | size(WIDTH, EQUAL, 1),
                    search->Render() | flex,
                    text(" ") | size(WIDTH, EQUAL, 1),
                }) | bgcolor(theme::panelDark),
                // Main dashboard / form area
                content->Render() | flex,
            }) |
            bgcolor(theme::panel) | flex;

        return hbox(Elements{sidebarElement, mainElement}) |
               bgcolor(theme::panel);
    });

    screen.Loop(application);
    return 0;
}

} // namespace ui
