#pragma once

#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>

namespace ui {

struct TableData {
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
};

ftxui::Element renderTable(const TableData& data,
                           const std::string& searchText);

}  // namespace ui
