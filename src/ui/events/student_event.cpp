#include "student_event.hpp"

#include "../table.hpp"
#include "config.hpp"

namespace ui::events {

ftxui::Element buildStudentView(const std::string& searchText) {
    TableData data;
    data.headers = {"ID", "Full name", "Class", "Priority", "Phone", "Email"};

    for (const auto& student : studentsList) {
        data.rows.push_back(
            {student.id, student.name, student.studentClass,
             student.isPriority ? "Yes" : "No", student.phone, student.email});
    }

    return renderTable(data, searchText);
}

}  // namespace ui::events
