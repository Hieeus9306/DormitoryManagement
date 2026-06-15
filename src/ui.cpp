#include "ui.hpp"

#include "fileIO.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {
namespace {

using namespace ftxui;

const Color kSidebar = Color::RGB(24, 31, 42);
const Color kPanel = Color::RGB(34, 43, 56);
const Color kPanelLight = Color::RGB(45, 56, 72);
const Color kAccent = Color::RGB(62, 207, 142);
const Color kMutedText = Color::RGB(151, 163, 182);

struct TableData {
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
};

enum class Section {
    Students,
    Rooms,
    Contracts,
    Invoices,
};

// Chuyển chuỗi thành chữ thường để tìm kiếm kh phân biệt hoa thường
std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

// Định dạng số với hai chữ số thập phân.
std::string formatNumber(double value) {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << value;
    return output.str();
}

// Định dạng tháng và năm của hóa đơn theo dạng MM/YYYY.
std::string formatPeriod(size_t month, size_t year) {
    std::ostringstream output;
    output << std::setfill('0') << std::setw(2) << month << "/" << year;
    return output.str();
}

// Load data
void loadApplicationData() {
    loadStudents();
    loadRooms();
    loadContracts();
    loadServiceInvoices();
}

// Hiển thị một hàng trong bảng dữ liệu.
Element renderTableRow(const std::vector<std::string>& cells,
                       bool isHeader = false) {
    Elements columns;
    for (const auto& cell : cells) {
        Element content = text(cell);
        if (isHeader) {
            content = content | bold | color(kAccent);
        }
        columns.push_back(content | flex);
        columns.push_back(separator());
    }
    columns.pop_back();
    return hbox(std::move(columns)) | size(HEIGHT, EQUAL, 1);
}

// Kiểm tra một hàng dữ liệu có khớp từ khóa tìm kiếm hay không.
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

// Giao diện khi chọn module
TableData createTableData(Section section) {
    TableData data;

    switch (section) {
        case Section::Students:
            data.headers = {"ID", "Full name", "Class", "Priority", "Phone",
                            "Email"};
            for (const auto& student : studentsList) {
                data.rows.push_back(
                    {student.id, student.name, student.studentClass,
                     student.isPriority ? "Yes" : "No", student.phone,
                     student.email});
            }
            break;
        case Section::Rooms:
            data.headers = {"Room", "Capacity", "Occupancy", "Price",
                            "Availability"};
            for (const auto& room : roomsList) {
                data.rows.push_back(
                    {room.id, std::to_string(room.type),
                     std::to_string(room.currentStudents()) + " / " +
                         std::to_string(room.type),
                     formatNumber(room.price),
                     room.hasAvailableSlot() ? "Available" : "Full"});
            }
            break;
        case Section::Contracts:
            data.headers = {"Contract", "Student", "Room", "Start date",
                            "End date", "Status"};
            for (const auto& contract : contractsList) {
                data.rows.push_back(
                    {contract.id, contract.studentId, contract.roomId,
                     contract.startDate.format(), contract.endDate.format(),
                     contract.isActive ? "Active" : "Inactive"});
            }
            break;
        case Section::Invoices:
            data.headers = {"Invoice", "Room", "Period", "Electricity",
                            "Water", "Total", "Status"};
            for (const auto& invoice : serviceInvoicesList) {
                data.rows.push_back(
                    {invoice.id, invoice.roomId,
                     formatPeriod(invoice.month, invoice.year),
                     formatNumber(invoice.newElectricityIndex -
                                  invoice.oldElectricityIndex),
                     formatNumber(invoice.newWaterIndex -
                                  invoice.oldWaterIndex),
                     formatNumber(invoice.totalAmount),
                     invoice.isPaid ? "Paid" : "Unpaid"});
            }
            break;
    }

    return data;
}

// Hiển thị bảng dữ liệu dùng search
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
        rows.push_back(text("No matching records") | color(kMutedText) |
                       center);
    }

    return vbox(std::move(rows)) | border | bgcolor(kPanelLight) | flex;
}

}  

// Chạy giao diện
int run() {
    loadApplicationData();

    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> sections = {
        "Students", "Rooms", "Contracts", "Invoices",
    };
    int selectedSection = 0;
    std::string searchText;

    auto menuOption = MenuOption::VerticalAnimated();
    menuOption.entries_option.transform = [](const EntryState& state) {
        auto item = hbox({
            text(state.active ? "  >  " : "     "),
            text(state.label) | flex,
        });

        if (state.active) {
            item = item | bold | color(kAccent) | bgcolor(kPanelLight);
        } else if (state.focused) {
            item = item | color(Color::White) | bgcolor(kPanel);
        } else {
            item = item | color(kMutedText);
        }
        return item | size(HEIGHT, EQUAL, 3);
    };

    auto sidebarMenu = Menu(&sections, &selectedSection, menuOption);

    InputOption searchOption = InputOption::Spacious();
    searchOption.multiline = false;
    auto searchInput = Input(&searchText, "Search records...", searchOption);
    auto exitButton =
        Button("Exit", screen.ExitLoopClosure(), ButtonOption::Ascii());

    auto controls = Container::Vertical({searchInput});
    auto sidebar = Container::Vertical({sidebarMenu, exitButton});
    auto layout = Container::Horizontal({sidebar, controls});

    auto application = Renderer(layout, [&] {
        const std::string& section = sections[selectedSection];
        const auto selected = static_cast<Section>(selectedSection);
        const TableData data = createTableData(selected);
        auto list = renderTable(data, searchText);

        auto sidebarElement =
            vbox({
                vbox({
                    text("DORMITORY") | bold | color(kAccent),
                    text("MANAGEMENT") | bold,
                    text("Student housing system") | color(kMutedText),
                }) | center |
                    size(HEIGHT, EQUAL, 6),
                separator(),
                text("  NAVIGATION") | color(kMutedText),
                sidebarMenu->Render() | flex,
                separator(),
                exitButton->Render() | center,
                text("  Arrow keys: Move  Enter: Select") | color(kMutedText),
            }) |
            bgcolor(kSidebar) | size(WIDTH, EQUAL, 30);

        auto mainElement =
            vbox({
                hbox({
                    vbox({
                        text(section) | bold | size(HEIGHT, EQUAL, 1),
                        text("View and manage " + toLower(section) +
                             " information") |
                            color(kMutedText),
                    }) | flex,
                    text("Dormitory Management System") | color(kMutedText),
                }),
                separator(),
                hbox({
                    text(" Search  ") | bold,
                    searchInput->Render() | flex,
                }) | border |
                    bgcolor(kPanelLight),
                separatorEmpty(),
                list | flex,
            }) |
            border | bgcolor(kPanel) | flex;

        return hbox({sidebarElement, mainElement}) | bgcolor(kPanel) |
               color(Color::White);
    });

    screen.Loop(application);
    return 0;
}

}
