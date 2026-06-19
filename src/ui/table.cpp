#include "table.hpp"

#include "Theme.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>

namespace ui {
namespace {

using namespace ftxui;

std::string toLower(std::string value) {
    std::transform(
        value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

Element renderTableRow(const std::vector<std::string>& cells,
                       bool isHeader = false, size_t rowIndex = 0) {
    Elements columns;
    for (const auto& cell : cells) {
        Element content = text(cell);
        if (isHeader) {
            content = content | bold | color(theme::accent);
        } else {
            content = content | color(Color::White);
        }
        columns.push_back(content | flex);
        columns.push_back(separator() | color(theme::borderColor));
    }
    columns.pop_back();
    auto row = hbox(std::move(columns)) | size(HEIGHT, EQUAL, 1);
    if (!isHeader && rowIndex % 2 == 1) {
        row = row | bgcolor(theme::stripe);
    }
    return row;
}

bool matchesSearch(const std::vector<std::string>& row,
                   const std::string&              query) {
    if (query.empty())
        return true;
    for (const auto& value : row) {
        if (toLower(value).find(query) != std::string::npos)
            return true;
    }
    return false;
}

} // namespace

Element renderTable(const TableData& data, const std::string& searchText) {
    Elements rows = {
        renderTableRow(data.headers, true),
        theme::divider(),
    };

    const std::string query      = toLower(searchText);
    bool              hasMatches = false;
    size_t            rowIndex   = 0;

    for (const auto& record : data.rows) {
        if (!matchesSearch(record, query))
            continue;
        rows.push_back(renderTableRow(record, false, rowIndex));
        rows.push_back(theme::divider());
        hasMatches = true;
        ++rowIndex;
    }

    if (!hasMatches) {
        rows.push_back(theme::emptyState("No matching records"));
    }

    return vbox(std::move(rows)) | theme::card | flex;
}

} // namespace ui
