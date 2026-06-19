#include "student_event.hpp"

#include "../Dashboard.hpp"
#include "../Theme.hpp"
#include "../table.hpp"
#include "core/config.hpp"
#include "domain/contract.hpp"
#include "domain/student.hpp"
#include "io/file_io.hpp"
#include "state/state.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>

namespace ui::events {
namespace {

using namespace ftxui;

constexpr int kStudentSection = 0;

// ── Validation helpers ──

bool studentExists(const std::string& studentId) {
    const size_t studentIndex = findStudent(studentId);
    return studentIndex != studentsList.size() &&
           studentsList[studentIndex].id == studentId;
}

bool studentIsStoredInRoom(const std::string& studentId) {
    for (const auto& room : roomsList) {
        if (std::find(room.students.begin(), room.students.end(), studentId) !=
            room.students.end()) {
            return true;
        }
    }
    return false;
}

bool canUpdateStudent(const std::string& studentId, const std::string& name,
                      const std::string& studentClass, std::string& message) {
    if (studentId.empty() || name.empty() || studentClass.empty()) {
        message = "Student ID, full name, and major/class are required.";
        return false;
    }
    if (!studentExists(studentId)) {
        message = "Student ID does not exist.";
        return false;
    }
    return true;
}

bool canDeleteStudent(const std::string& studentId, std::string& message) {
    if (studentId.empty()) {
        message = "Student ID is required.";
        return false;
    }
    if (!studentExists(studentId)) {
        message = "Student ID does not exist.";
        return false;
    }
    if (findActiveContractOfStudent(studentId) != contractsList.size() ||
        studentIsStoredInRoom(studentId)) {
        message = "Student is still linked to a room or active contract. "
                  "Use Contracts / Check Out Room first.";
        return false;
    }
    return true;
}

// ── Search ──

bool matchesStudent(const Student& student, const SearchState& searchState) {
    const std::string keyword = searchKeyword(searchState, kStudentSection);
    switch (selectedSearchField(searchState, kStudentSection)) {
    case 0:
        return searchMatches(student.id, keyword);
    case 1:
        return searchMatches(student.name, keyword);
    default:
        return true;
    }
}

// ── Form: Update ──

Component studentUpdateForm(std::shared_ptr<ActionState> state) {
    auto id   = createTextInput(state->students.updateId, "Student ID");
    auto name = createTextInput(state->students.updateName, "Full name");
    auto cls  = createTextInput(state->students.updateClass, "Major/Class");
    auto priority =
        Checkbox("Priority (true/false)", &state->students.updatePriority);
    auto phone  = createTextInput(state->students.updatePhone, "Phone number");
    auto email  = createTextInput(state->students.updateEmail, "Email");
    auto button = Button(
        "updateStudent",
        [state] {
            const auto studentId    = nonEmpty(state->students.updateId);
            const auto name         = nonEmpty(state->students.updateName);
            const auto studentClass = nonEmpty(state->students.updateClass);
            const auto phone        = nonEmpty(state->students.updatePhone);
            const auto email        = nonEmpty(state->students.updateEmail);

            std::string message;
            if (!canUpdateStudent(studentId, name, studentClass, message)) {
                state->students.message = message;
                return;
            }

            updateStudent(studentId, name, studentClass,
                          state->students.updatePriority, phone, email);
            saveStudents();
            state->students.message =
                "updateStudent completed for " + studentId + ".";
        },
        ButtonOption::Ascii());
    auto container =
        Container::Vertical({id, name, cls, priority, phone, email, button});

    return Renderer(container, [=] {
        return titledForm("Students / Update",
                          {
                              field("Student ID", id),
                              field("Full name", name),
                              field("Major/Class", cls),
                              priority->Render(),
                              field("Phone", phone),
                              field("Email", email),
                              button->Render() | center,
                          },
                          state->students.message);
    });
}

// ── Form: Delete ──

Component studentRemoveForm(std::shared_ptr<ActionState> state) {
    auto id     = createTextInput(state->students.removeId, "Student ID");
    auto button = Button(
        "Delete Student",
        [state] {
            const auto  studentId = nonEmpty(state->students.removeId);
            std::string message;
            if (!canDeleteStudent(studentId, message)) {
                state->students.message = message;
                return;
            }

            removeStudent(studentId);
            saveStudents();
            state->students.message =
                "Student deleted successfully: " + studentId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({id, button});

    return Renderer(container, [=] {
        return titledForm("Students / Delete",
                          {
                              field("Student ID", id),
                              button->Render() | center,
                          },
                          state->students.message);
    });
}

} // namespace

// ── Dashboard ──

Component createStudentDashboard(SearchState& searchState) {
    return createDashboard({
        .headers = {"Student ID", "Full name", "Major/Class", "Priority",
                    "Phone", "Email"},
        // .pageSize = 0;  // auto-calculate from terminal height
        .hasDetailView = false,
        .totalRecords  = studentsList.size(),
        .filterFn =
            [&searchState](size_t index) {
                return index < studentsList.size() &&
                       matchesStudent(studentsList[index], searchState);
            },
        .renderRowFn = [](size_t index) -> std::vector<Element> {
            const auto& s = studentsList[index];
            return {
                theme::dataCell(s.id),
                theme::dataCell(s.name),
                theme::dataCell(s.studentClass),
                theme::statusBadge(s.isPriority ? "Yes" : "No", s.isPriority),
                theme::dataCell(s.phone),
                theme::dataCell(s.email),
            };
        },
        .emptyMessage = "No matching students",
    });
}

// ── Action menu & forms ──

Component createStudentActionMenu(std::shared_ptr<ActionState> state,
                                  int&                         contentMode) {
    return createActionMenu(state->studentActions, state->students.action,
                            contentMode);
}

Component createStudentActionForms(std::shared_ptr<ActionState> state) {
    return Container::Tab({studentUpdateForm(state), studentRemoveForm(state)},
                          &state->students.action);
}

} // namespace ui::events
