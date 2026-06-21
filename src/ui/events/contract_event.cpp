#include "contract_event.hpp"

#include "../Dashboard.hpp"
#include "../Theme.hpp"
#include "domain/contract.hpp"
#include "domain/room.hpp"
#include "domain/student.hpp"
#include "state/state.hpp"

#include "libs/string_utils.hpp"
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>

namespace ui::events {
namespace {

using namespace ftxui;

constexpr int kContractSection = 2;

// ── Validation helpers ──

size_t existingRoomIndex(const std::string& roomId) {
    const size_t roomIndex = findRoom(roomId);
    if (roomIndex == roomsList.size() || roomsList[roomIndex].id != roomId) {
        return roomsList.size();
    }
    return roomIndex;
}

bool validContractRange(const base::Date& startDate, const base::Date& endDate,
                        std::string& message) {
    if (startDate >= endDate) {
        message = "Contract start date must be before end date.";
        return false;
    }
    return true;
}

bool roomCanReceiveStudent(const std::string& roomId, size_t& roomIndex,
                           std::string& message) {
    roomIndex = existingRoomIndex(roomId);
    if (roomIndex == roomsList.size()) {
        message = "Room ID does not exist.";
        return false;
    }
    if (!roomsList[roomIndex].hasAvailableSlot()) {
        message = "Selected room is full.";
        return false;
    }
    return true;
}

bool canRegisterContract(const std::string& studentId,
                         const std::string& roomId, const base::Date& startDate,
                         const base::Date& endDate, std::string& message) {
    if (!validContractRange(startDate, endDate, message)) {
        return false;
    }
    if (findActiveContractOfStudent(studentId) != contractsList.size()) {
        message = "This student already has an active contract.";
        return false;
    }
    size_t roomIndex = roomsList.size();
    return roomCanReceiveStudent(roomId, roomIndex, message);
}

bool canTransferContract(const std::string& studentId,
                         const std::string& newRoomId,
                         const base::Date& startDate, const base::Date& endDate,
                         size_t& activeContractIndex, std::string& message) {
    if (!validContractRange(startDate, endDate, message)) {
        return false;
    }
    activeContractIndex = findActiveContractOfStudent(studentId);
    if (activeContractIndex == contractsList.size()) {
        message = "Student does not have an active contract.";
        return false;
    }
    if (contractsList[activeContractIndex].roomId == newRoomId) {
        message = "New room must be different from current room.";
        return false;
    }
    size_t roomIndex = roomsList.size();
    return roomCanReceiveStudent(newRoomId, roomIndex, message);
}

bool canCheckoutContract(const std::string& studentId, std::string& message) {
    if (studentId.empty()) {
        message = "Student ID is required.";
        return false;
    }
    if (findActiveContractOfStudent(studentId) == contractsList.size()) {
        message = "Student does not have an active contract.";
        return false;
    }
    return true;
}

// ── Search ──

bool matchesContract(const Contract& contract, const SearchState& searchState) {
    const std::string keyword = searchKeyword(searchState, kContractSection);
    switch (selectedSearchField(searchState, kContractSection)) {
    case 0:
        return searchMatches(contract.id, keyword);
    case 1:
        return searchMatches(contract.studentId, keyword);
    default:
        return true;
    }
}

// ── Detail view ──

Element renderContractDetail(size_t index) {
    const auto& contract = contractsList[index];
    return vbox({
        theme::sectionTitle("Contract information"),
        separator(),
        theme::detailLine("ID", contract.id),
        theme::detailLine("Student ID", contract.studentId),
        theme::detailLine("Room ID", contract.roomId),
        theme::detailLine("Start date", contract.startDate.format()),
        theme::detailLine("End date", contract.endDate.format()),
        theme::detailLine("Active", contract.isActive ? "true" : "false"),
    });
}

// ── Form: Register ──

Component contractRegisterForm(std::shared_ptr<ActionState> state) {
    auto studentId =
        createTextInput(state->contracts.registerStudentId, "Student ID");
    auto name = createTextInput(state->contracts.registerName, "Full name");
    auto cls  = createTextInput(state->contracts.registerClass, "Major/Class");
    auto priority =
        Checkbox("Priority (true/false)", &state->contracts.registerPriority);
    auto phone  = createTextInput(state->contracts.registerPhone, "Phone");
    auto email  = createTextInput(state->contracts.registerEmail, "Email");
    auto roomId = createTextInput(state->contracts.registerRoomId, "Room ID");
    auto start =
        createTextInput(state->contracts.registerStartDate, "DD/MM/YYYY");
    auto end = createTextInput(state->contracts.registerEndDate, "DD/MM/YYYY");
    auto button = Button(
        "registerRoom + addStudent",
        [state] {
            base::Date startDate;
            base::Date endDate;
            const auto studentId = nonEmpty(state->contracts.registerStudentId);
            const auto roomId    = nonEmpty(state->contracts.registerRoomId);
            if (studentId.empty() ||
                nonEmpty(state->contracts.registerName).empty() ||
                nonEmpty(state->contracts.registerClass).empty() ||
                roomId.empty() ||
                !parseDate(state->contracts.registerStartDate, startDate) ||
                !parseDate(state->contracts.registerEndDate, endDate)) {
                state->contracts.message =
                    "Student info, room ID, start date, and end date "
                    "are required.";
                return;
            }

            std::string message;
            if (!canRegisterContract(studentId, roomId, startDate, endDate,
                                     message)) {
                state->contracts.message = message;
                return;
            }

            Student student;
            student.id           = studentId;
            student.name         = nonEmpty(state->contracts.registerName);
            student.studentClass = nonEmpty(state->contracts.registerClass);
            student.isPriority   = state->contracts.registerPriority;
            student.phone        = nonEmpty(state->contracts.registerPhone);
            student.email        = nonEmpty(state->contracts.registerEmail);

            addStudent(student);
            registerRoom(studentId, roomId, startDate, endDate);
            state->contracts.message =
                "addStudent and registerRoom completed for " + studentId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({studentId, name, cls, priority, phone,
                                          email, roomId, start, end, button});

    return Renderer(container, [=] {
        return titledForm("Contracts / Register Room",
                          {
                              text("Student information") | bold,
                              field("Student ID", studentId),
                              field("Full name", name),
                              field("Major/Class", cls),
                              priority->Render(),
                              field("Phone", phone),
                              field("Email", email),
                              separator(),
                              text("Contract information") | bold,
                              field("Room ID", roomId),
                              field("Start date", start),
                              field("End date", end),
                              button->Render() | center,
                          },
                          state->contracts.message);
    });
}

// ── Form: Transfer ──

Component contractTransferForm(std::shared_ptr<ActionState> state) {
    auto studentId =
        createTextInput(state->contracts.transferStudentId, "Student ID");
    auto roomId =
        createTextInput(state->contracts.transferRoomId, "New room ID");
    auto start =
        createTextInput(state->contracts.transferStartDate, "DD/MM/YYYY");
    auto end = createTextInput(state->contracts.transferEndDate, "DD/MM/YYYY");
    auto button = Button(
        "transferRoom",
        [state] {
            base::Date startDate;
            base::Date endDate;
            const auto studentId = nonEmpty(state->contracts.transferStudentId);
            const auto roomId    = nonEmpty(state->contracts.transferRoomId);
            if (studentId.empty() || roomId.empty() ||
                !parseDate(state->contracts.transferStartDate, startDate) ||
                !parseDate(state->contracts.transferEndDate, endDate)) {
                state->contracts.message =
                    "Student ID, new room ID, start date, and end date "
                    "are required.";
                return;
            }

            size_t      activeContractIndex = contractsList.size();
            std::string message;
            if (!canTransferContract(studentId, roomId, startDate, endDate,
                                     activeContractIndex, message)) {
                state->contracts.message = message;
                return;
            }

            transferRoom(studentId, roomId, startDate, endDate);
            state->contracts.message =
                "transferRoom completed for " + studentId + ".";
        },
        ButtonOption::Ascii());
    auto container =
        Container::Vertical({studentId, roomId, start, end, button});

    return Renderer(container, [=] {
        return titledForm("Contracts / Transfer Room",
                          {
                              field("Student ID", studentId),
                              field("New room ID", roomId),
                              field("Start date", start),
                              field("End date", end),
                              button->Render() | center,
                          },
                          state->contracts.message);
    });
}

// ── Form: Check Out ──

Component contractCheckoutForm(std::shared_ptr<ActionState> state) {
    auto studentId =
        createTextInput(state->contracts.checkoutStudentId, "Student ID");
    auto button = Button(
        "checkoutRoom + removeStudent",
        [state] {
            const auto studentId = nonEmpty(state->contracts.checkoutStudentId);

            std::string message;
            if (!canCheckoutContract(studentId, message)) {
                state->contracts.message = message;
                return;
            }

            checkoutRoom(studentId);
            removeStudent(studentId);
            state->contracts.message =
                "checkoutRoom and removeStudent completed for " + studentId +
                ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({studentId, button});

    return Renderer(container, [=] {
        return titledForm("Contracts / Check Out Room",
                          {
                              field("Student ID", studentId),
                              button->Render() | center,
                          },
                          state->contracts.message);
    });
}

} // namespace

// ── Dashboard ──

Component createContractDashboard(SearchState& searchState) {
    return createDashboard({
        .headers = {"Id", "Status"},
        // .pageSize = 0;  // auto-calculate from terminal height
        .hasDetailView = true,
        .totalFn       = [&]() { return contractsList.size(); },
        .filterFn =
            [&searchState](size_t index) {
                const size_t r = contractsList.size() - 1 - index;
                return r < contractsList.size() &&
                       matchesContract(contractsList[r], searchState);
            },
        .renderRowFn = [](size_t index) -> std::vector<Element> {
            const size_t r        = contractsList.size() - 1 - index;
            const auto&  contract = contractsList[r];
            return {
                text(contract.id) | color(Color::White),
                theme::statusBadge(contract.isActive ? "true" : "false",
                                   contract.isActive),
            };
        },
        .renderDetailFn =
            [](size_t index) {
                const size_t r = contractsList.size() - 1 - index;
                return renderContractDetail(r);
            },
        .emptyMessage = "No matching contracts",
    });
}

// ── Action menu & forms ──

Component createContractActionMenu(std::shared_ptr<ActionState> state,
                                   int&                         contentMode) {
    return createActionMenu(state->contractActions, state->contracts.action,
                            contentMode);
}

Component createContractActionForms(std::shared_ptr<ActionState> state) {
    return Container::Tab({contractRegisterForm(state),
                           contractTransferForm(state),
                           contractCheckoutForm(state)},
                          &state->contracts.action);
}

} // namespace ui::events
