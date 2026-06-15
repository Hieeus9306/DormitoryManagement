#include "table.hpp"

#include "ui.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>

namespace ui {
namespace {

using namespace ftxui;

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

Element renderTableRow(const std::vector<std::string>& cells,
                       bool isHeader = false) {
    Elements columns;
    for (const auto& cell : cells) {
        Element content = text(cell);
        if (isHeader) {
            content = content | bold | color(theme::accent);
        }
        columns.push_back(content | flex);
        columns.push_back(separator());
    }
    columns.pop_back();
    return hbox(std::move(columns)) | size(HEIGHT, EQUAL, 1);
}

bool matchesSearch(const std::vector<std::string>& row,
                   const std::string& query) {
    if (query.empty()) {
        return true;
    }

    for (const auto& value : row) {
        if (toLower(value).find(query) != std::string::npos) {
            return true;
        }
    }
    return false;
}

}  // namespace

Element renderTable(const TableData& data, const std::string& searchText) {
    Elements rows = {
        renderTableRow(data.headers, true),
        separator(),
    };

    const std::string query = toLower(searchText);
    bool hasMatches = false;
    for (const auto& record : data.rows) {
        if (!matchesSearch(record, query)) {
            continue;
        }

        rows.push_back(renderTableRow(record));
        rows.push_back(separator());
        hasMatches = true;
    }

    if (!hasMatches) {
        rows.push_back(text("No matching records") | color(theme::mutedText) |
                       center);
    }

    return vbox(std::move(rows)) | border | bgcolor(theme::panelLight) | flex;
}

}  // namespace ui
