#include "action_event.hpp"

#include "../Theme.hpp"
#include "contract_event.hpp"
#include "invoice_event.hpp"
#include "io/file_io.hpp"
#include "report_event.hpp"
#include "room_event.hpp"
#include "student_event.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>

namespace ui::events {

using namespace ftxui;

// Xóa khoảng trắng thừa ở đầu và cuối chuỗi nhập từ form.
std::string trim(std::string value) {
    auto isSpace = [](unsigned char ch) { return std::isspace(ch) != 0; };
    value.erase(value.begin(),
                std::find_if(value.begin(), value.end(),
                             [&](unsigned char ch) { return !isSpace(ch); }));
    value.erase(std::find_if(value.rbegin(), value.rend(),
                             [&](unsigned char ch) { return !isSpace(ch); })
                    .base(),
                value.end());
    return value;
}

// Chuyển chuỗi sang số nguyên không dấu, trả về false nếu dữ liệu không hợp lệ.
bool parseSize(const std::string& text, size_t& value) {
    try {
        std::string input = trim(text);
        if (input.empty()) {
            return false;
        }
        size_t pos = 0;
        value      = static_cast<size_t>(std::stoull(input, &pos));
        return pos == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

// Chuyển chuỗi sang số thực, trả về false nếu dữ liệu không hợp lệ.
bool parseDouble(const std::string& text, double& value) {
    try {
        std::string input = trim(text);
        if (input.empty()) {
            return false;
        }
        size_t pos = 0;
        value      = std::stod(input, &pos);
        return pos == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

// Chuyển chuỗi ngày dạng DD/MM/YYYY thành base::Date và kiểm tra ngày hợp lệ.
bool parseDate(const std::string& text, base::Date& date) {
    try {
        std::string input = trim(text);
        if (input.size() != 10) {
            return false;
        }
        date = base::Date(input);
        return date.isValid();
    } catch (const std::exception&) {
        return false;
    }
}

// Chuẩn hóa input bắt buộc bằng cách trim trước khi kiểm tra hoặc lưu.
std::string nonEmpty(const std::string& value) { return trim(value); }

// Lưu lại toàn bộ dữ liệu sau các action ảnh hưởng nhiều nhóm dữ liệu.
void saveAll() {
    saveStudents();
    saveRooms();
    saveContracts();
    saveServiceInvoices();
}

// Tạo input một dòng dùng chung cho các form nghiệp vụ.
Component createTextInput(std::string& value, const std::string& placeholder) {
    auto option      = InputOption::Spacious();
    option.multiline = false;
    return Input(&value, placeholder, option);
}

// Tạo menu chọn action con của từng module như Update, Add hoặc Delete.
Component createActionMenu(std::vector<std::string>& actions, int& selected,
                           int& contentMode) {
    std::vector<Component> buttons;
    for (size_t index = 0; index < actions.size(); ++index) {
        auto option      = ButtonOption::Ascii();
        option.transform = [&selected, index](const EntryState& entry) {
            const bool isActive = selected == static_cast<int>(index);
            auto       line     = hbox({
                text(isActive ? "  \u25B6 " : "    "),
                text(entry.label) | flex,
            });

            if (isActive) {
                line = line | bold | color(theme::accent) |
                       bgcolor(theme::panelLight);
            } else if (entry.focused) {
                line = line | color(Color::White) | bgcolor(theme::panel);
            } else {
                line = line | color(theme::mutedText);
            }

            return line | size(HEIGHT, EQUAL, 1);
        };

        buttons.push_back(Button(
            actions[index],
            [&selected, &contentMode, index] {
                selected    = static_cast<int>(index);
                contentMode = 1;
            },
            option));
    }

    return Container::Vertical(std::move(buttons));
}

// Render một dòng label + input trong form với độ rộng label cố định.
Element field(const std::string& label, const Component& input) {
    return hbox({
               theme::pillLabel(label) | size(WIDTH, EQUAL, 20),
               text(" ") | size(WIDTH, EQUAL, 1),
               input->Render() | flex | bgcolor(theme::panel),
           }) |
           size(HEIGHT, EQUAL, 3);
}

namespace {

// Render dòng thông báo trạng thái sau khi người dùng thực hiện action.
Element messageLine(const std::string& message) {
    return text(message) | color(theme::mutedText);
}

} // namespace

// Bọc các dòng input thành một form có tiêu đề, border và thông báo trạng thái.
Element titledForm(const std::string& title, Elements rows,
                   const std::string& message) {
    Elements content = {
        text(title) | bold | color(theme::accent),
        separator(),
    };
    for (auto& row : rows) {
        content.push_back(std::move(row));
    }
    content.push_back(separator());
    content.push_back(messageLine(message));

    return vbox(std::move(content)) | border | bgcolor(theme::panelLight);
}

// Tạo toàn bộ menu action và form action cho bốn module nghiệp vụ.
BusinessActionComponents createBusinessActions(int& selectedSection,
                                               int& contentMode) {
    auto state = std::make_shared<ActionState>();

    auto studentMenu  = createStudentActionMenu(state, contentMode);
    auto roomMenu     = createRoomActionMenu(state, contentMode);
    auto contractMenu = createContractActionMenu(state, contentMode);
    auto invoiceMenu  = createInvoiceActionMenu(state, contentMode);
    auto reportMenu   = createReportActionMenu();

    auto studentForms  = createStudentActionForms(state);
    auto roomForms     = createRoomActionForms(state);
    auto contractForms = createContractActionForms(state);
    auto invoiceForms  = createInvoiceActionForms(state);
    auto reportForms   = createReportActionForms();

    auto formTabs = Container::Tab(
        {studentForms, roomForms, contractForms, invoiceForms, reportForms},
        &selectedSection);

    auto form = Renderer(formTabs, [=] { return formTabs->Render(); });

    return {{studentMenu, roomMenu, contractMenu, invoiceMenu, reportMenu},
            form};
}

} // namespace ui::events
