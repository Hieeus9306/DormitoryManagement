#pragma once

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace ui::events {

ftxui::Component createNavigation(std::vector<std::string>& sections,
                                  int& selectedSection);
ftxui::Component createExit(ftxui::ScreenInteractive& screen);

}  // namespace ui::events
