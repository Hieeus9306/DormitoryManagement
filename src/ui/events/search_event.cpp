#include "search_event.hpp"

#include <ftxui/component/component_options.hpp>

namespace ui::events {

ftxui::Component createSearch(std::string& searchText) {
    auto option = ftxui::InputOption::Spacious();
    option.multiline = false;
    return ftxui::Input(&searchText, "Search records...", option);
}

}  // namespace ui::events
