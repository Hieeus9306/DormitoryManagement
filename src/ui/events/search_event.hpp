#pragma once

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>

namespace ui::events {

struct SearchState {
    std::vector<std::vector<std::string>> fields;
    std::vector<int>                      selectedFields;
    std::vector<std::string>              keywords;
};

SearchState createSearchState();
int         selectedSearchField(const SearchState& state, int section);
std::string searchKeyword(const SearchState& state, int section);
bool        searchMatches(const std::string& value, const std::string& keyword);
ftxui::Component createSearch(SearchState& state, int& selectedSection);

} // namespace ui::events
