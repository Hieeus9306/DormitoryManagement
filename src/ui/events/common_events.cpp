#include "common_events.hpp"

#include "../Theme.hpp"
#include "contract_event.hpp"
#include "invoice_event.hpp"
#include "report_event.hpp"
#include "room_event.hpp"
#include "student_event.hpp"

#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui::events {
namespace {

// Tạo một nút điều hướng cho từng module và cập nhật section đang chọn khi bấm.
ftxui::Component createNavigationButton(std::string label, int index,
                                        int& selectedSection,
                                        int& contentMode) {
    using namespace ftxui;

    auto option      = ButtonOption::Border();
    option.transform = [&selectedSection, index](const EntryState& entry) {
        const bool isActive = selectedSection == index;
        auto       icon     = isActive ? " \u25C9 " : " \u25CB ";
        auto       row      = hbox({
            text(icon),
            text(entry.label) | flex,
        });

        if (isActive) {
            row =
                row | bold | color(theme::accent) | bgcolor(theme::panelLight);
        } else if (entry.focused) {
            row = row | color(Color::White) | bgcolor(theme::panel);
        } else {
            row = row | color(theme::mutedText);
        }

        // Keep the sidebar compact enough to show every action in sections
        // such as Rooms and Contracts.
        return row | size(HEIGHT, EQUAL, 2);
    };

    return Button(
        label,
        [&selectedSection, &contentMode, index] {
            selectedSection = index;
            contentMode     = 0;
        },
        option);
}

} // namespace

// Tạo cụm navigation bên sidebar, gồm nút module và các chức năng con.
ftxui::Component
createNavigation(std::vector<std::string>& sections, int& selectedSection,
                 int&                                 contentMode,
                 const std::vector<ftxui::Component>& sectionMenus) {
    std::vector<ftxui::Component> navigationItems;
    for (size_t index = 0; index < sections.size(); ++index) {
        navigationItems.push_back(
            createNavigationButton(sections[index], static_cast<int>(index),
                                   selectedSection, contentMode));
        if (index < sectionMenus.size() &&
            sectionMenus[index]->ChildCount() > 0) {
            navigationItems.push_back(
                ftxui::Maybe(sectionMenus[index], [&, index] {
                    return selectedSection == static_cast<int>(index);
                }));
        }
    }

    return ftxui::Container::Vertical(std::move(navigationItems));
}

// Tạo nút thoát chương trình bằng closure kết thúc vòng lặp FTXUI.
ftxui::Component createExit(ftxui::ScreenInteractive& screen) {
    return ftxui::Button("Exit", screen.ExitLoopClosure(),
                         ftxui::ButtonOption::Border());
}

// Tạo tab dashboard chính, tự đổi nội dung theo navigation đang chọn.
ftxui::Component createDashboard(int&         selectedSection,
                                 SearchState& searchState) {
    return ftxui::Container::Tab(
        {
            createStudentDashboard(searchState),
            createRoomDashboard(searchState),
            createContractDashboard(searchState),
            createInvoiceDashboard(searchState),
            createReportDashboard(),
        },
        &selectedSection);
}

} // namespace ui::events
