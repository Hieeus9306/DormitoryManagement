#pragma once

#include "search_event.hpp"

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace ui::events {

ftxui::Component
createNavigation(std::vector<std::string>& sections, int& selectedSection,
                 int&                                 contentMode,
                 const std::vector<ftxui::Component>& sectionMenus);
ftxui::Component createExit(ftxui::ScreenInteractive& screen);
ftxui::Component createDashboard(int&         selectedSection,
                                 SearchState& searchState);

} // namespace ui::events
