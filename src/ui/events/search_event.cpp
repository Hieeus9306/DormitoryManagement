#include "search_event.hpp"

#include "../Theme.hpp"

#include <algorithm>
#include <cctype>

#include <ftxui/component/component_options.hpp>

namespace ui::events {
namespace {

// Chuyen chuoi ve chu thuong de so khop tim kiem khong phan biet hoa thuong.
std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

} // namespace

// Tao state mac dinh cho tung navigation gom field search va keyword rieng.
SearchState createSearchState() {
    return {
        {
            {"Student ID", "Name"},
            {"Room ID", "Student ID"},
            {"Contract ID", "Student ID"},
            {"Invoice ID", "Room ID"},
            {"Report"},
        },
        {0, 0, 0, 0, 0},
        {"", "", "", "", ""},
    };
}

// Lay field search dang duoc chon cua navigation hien tai.
int selectedSearchField(const SearchState& state, int section) {
    if (section < 0 ||
        static_cast<size_t>(section) >= state.selectedFields.size()) {
        return 0;
    }
    return state.selectedFields[section];
}

// Lay keyword rieng cua navigation hien tai.
std::string searchKeyword(const SearchState& state, int section) {
    if (section < 0 || static_cast<size_t>(section) >= state.keywords.size()) {
        return "";
    }
    return state.keywords[section];
}

// So khop mot gia tri voi keyword, tai su dung cho tat ca dashboard.
bool searchMatches(const std::string& value, const std::string& keyword) {
    const std::string query = toLower(keyword);
    if (query.empty()) {
        return true;
    }
    return toLower(value).find(query) != std::string::npos;
}

// ── Search bar ──
//
// Each navigation section gets its own search bar with a field dropdown and
// keyword input.  Reports are excluded since they have no searchable records.

ftxui::Component createSearch(SearchState& state, int& selectedSection) {
    using namespace ftxui;

    auto option      = InputOption::Spacious();
    option.multiline = false;

    std::vector<Component> searchRows;
    for (size_t index = 0; index < state.fields.size(); ++index) {
        if (state.fields[index].size() == 1 &&
            state.fields[index][0] == "Report") {
            searchRows.push_back(Renderer([] {
                return text(" Report summary does not use search.") |
                       color(theme::mutedText);
            }));
            continue;
        }

        auto dropdown =
            Dropdown(&state.fields[index], &state.selectedFields[index]);
        auto input = Input(&state.keywords[index], "Search records...", option);
        auto row   = Container::Horizontal({dropdown, input});

        searchRows.push_back(Renderer(row, [dropdown, input] {
            return hbox(Elements{
                text(" By ") | color(theme::mutedText),
                dropdown->Render() | size(WIDTH, EQUAL, 24),
                text("  Value ") | color(theme::mutedText),
                input->Render() | flex,
            });
        }));
    }

    return Container::Tab(std::move(searchRows), &selectedSection);
}

} // namespace ui::events
