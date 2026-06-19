#include "room_event.hpp"

#include "../Dashboard.hpp"
#include "../Theme.hpp"
#include "../table.hpp"
#include "core/config.hpp"
#include "domain/room.hpp"
#include "io/file_io.hpp"
#include "state/state.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>

namespace ui::events {
namespace {

using namespace ftxui;

constexpr int kRoomSection = 1;

// ── Validation helpers ──

bool validRoomType(size_t roomType) {
    return roomType == 4 || roomType == 6 || roomType == 8;
}

size_t existingRoomIndex(const std::string& roomId) {
    const size_t roomIndex = findRoom(roomId);
    if (roomIndex == roomsList.size() || roomsList[roomIndex].id != roomId) {
        return roomsList.size();
    }
    return roomIndex;
}

bool parseRoomForm(const std::string& roomId, const std::string& typeText,
                   const std::string& priceText, size_t& roomType,
                   double& price, std::string& message) {
    if (roomId.empty() || !parseSize(typeText, roomType) ||
        !parseDouble(priceText, price)) {
        message = "Room ID, type, and price are required.";
        return false;
    }
    if (!validRoomType(roomType)) {
        message = "Room type must be 4, 6, or 8.";
        return false;
    }
    if (price <= 0) {
        message = "Room price must be greater than 0.";
        return false;
    }
    return true;
}

bool canUpdateRoom(const std::string& roomId, size_t roomType,
                   size_t& roomIndex, std::string& message) {
    roomIndex = existingRoomIndex(roomId);
    if (roomIndex == roomsList.size()) {
        message = "Room ID does not exist.";
        return false;
    }
    if (roomsList[roomIndex].currentStudents() > roomType) {
        message = "New capacity is smaller than current student count.";
        return false;
    }
    return true;
}

bool canAddRoom(const std::string& roomId, std::string& message) {
    if (existingRoomIndex(roomId) != roomsList.size()) {
        message = "Room ID already exists.";
        return false;
    }
    return true;
}

bool canDeleteRoom(const std::string& roomId, size_t& roomIndex,
                   std::string& message) {
    if (roomId.empty()) {
        message = "Room ID is required.";
        return false;
    }
    roomIndex = existingRoomIndex(roomId);
    if (roomIndex == roomsList.size()) {
        message = "Room ID does not exist.";
        return false;
    }
    if (!roomsList[roomIndex].students.empty()) {
        message = "Room still has students. Transfer or checkout them first.";
        return false;
    }
    return true;
}

// ── Search ──

bool matchesRoom(const Room& room, const SearchState& searchState) {
    const std::string keyword = searchKeyword(searchState, kRoomSection);
    if (selectedSearchField(searchState, kRoomSection) == 0) {
        return searchMatches(room.id, keyword);
    }
    for (const auto& studentId : room.students) {
        if (searchMatches(studentId, keyword)) {
            return true;
        }
    }
    return keyword.empty();
}

// ── Detail view ──

Element renderRoomDetail(size_t index) {
    const auto& room = roomsList[index];

    Elements studentElements = {
        text("Student IDs") | color(theme::mutedText),
    };
    if (room.students.empty()) {
        studentElements.push_back(text("empty") | color(theme::mutedText));
    } else {
        for (const auto& sid : room.students) {
            studentElements.push_back(text("  - " + sid));
        }
    }

    return vbox({
        theme::sectionTitle("Room information"),
        separator(),
        theme::detailLine("ID", room.id),
        theme::detailLine("Capacity", std::to_string(room.type)),
        theme::detailLine("Current students",
                          std::to_string(room.currentStudents())),
        theme::detailLine("Price", std::to_string(room.price)),
        vbox(std::move(studentElements)),
        theme::detailLine("Available",
                          room.hasAvailableSlot() ? "true" : "false"),
    });
}

// ── Capacity display with colour ──

Element capacityElement(const Room& room) {
    auto cap = text(std::to_string(room.currentStudents()) + " / " +
                    std::to_string(room.type));
    if (room.hasAvailableSlot()) {
        cap = cap | color(Color::Green);
    }
    return cap;
}

// ── Form: Update ──

Component roomUpdateForm(std::shared_ptr<ActionState> state) {
    auto id     = createTextInput(state->rooms.updateId, "Room ID");
    auto type   = createTextInput(state->rooms.updateType, "4, 6, or 8");
    auto price  = createTextInput(state->rooms.updatePrice, "New price");
    auto button = Button(
        "updateRoom",
        [state] {
            size_t     roomType  = 0;
            size_t     roomIndex = roomsList.size();
            double     price     = 0;
            const auto roomId    = nonEmpty(state->rooms.updateId);

            std::string message;
            if (!parseRoomForm(roomId, state->rooms.updateType,
                               state->rooms.updatePrice, roomType, price,
                               message) ||
                !canUpdateRoom(roomId, roomType, roomIndex, message)) {
                state->rooms.message = message;
                return;
            }

            updateRoom(roomId, roomType, price);
            saveRooms();
            state->rooms.message = "updateRoom completed for " + roomId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({id, type, price, button});

    return Renderer(container, [=] {
        return titledForm("Rooms / Update",
                          {
                              field("Room ID", id),
                              field("Type", type),
                              field("Price", price),
                              button->Render() | center,
                          },
                          state->rooms.message);
    });
}

// ── Form: Add ──

Component roomAddForm(std::shared_ptr<ActionState> state) {
    auto id     = createTextInput(state->rooms.addId, "Room ID");
    auto type   = createTextInput(state->rooms.addType, "4, 6, or 8");
    auto price  = createTextInput(state->rooms.addPrice, "Price");
    auto button = Button(
        "addRoom",
        [state] {
            size_t     roomType = 0;
            double     price    = 0;
            const auto roomId   = nonEmpty(state->rooms.addId);

            std::string message;
            if (!parseRoomForm(roomId, state->rooms.addType,
                               state->rooms.addPrice, roomType, price,
                               message) ||
                !canAddRoom(roomId, message)) {
                state->rooms.message = message;
                return;
            }

            addRoom(roomId, roomType, price);
            saveRooms();
            state->rooms.message = "addRoom completed for " + roomId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({id, type, price, button});

    return Renderer(container, [=] {
        return titledForm("Rooms / Add",
                          {
                              field("Room ID", id),
                              field("Type", type),
                              field("Price", price),
                              button->Render() | center,
                          },
                          state->rooms.message);
    });
}

// ── Form: Delete ──

Component roomRemoveForm(std::shared_ptr<ActionState> state) {
    auto id     = createTextInput(state->rooms.removeId, "Room ID");
    auto button = Button(
        "deleteRoom",
        [state] {
            const auto  roomId    = nonEmpty(state->rooms.removeId);
            size_t      roomIndex = roomsList.size();
            std::string message;
            if (!canDeleteRoom(roomId, roomIndex, message)) {
                state->rooms.message = message;
                return;
            }

            removeRoom(roomId);
            saveRooms();
            state->rooms.message = "Room deleted successfully: " + roomId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({id, button});

    return Renderer(container, [=] {
        return titledForm("Rooms / Delete",
                          {
                              field("Room ID", id),
                              button->Render() | center,
                          },
                          state->rooms.message);
    });
}

} // namespace

// ── Dashboard ──

Component createRoomDashboard(SearchState& searchState) {
    return createDashboard({
        .headers = {"Id", "Current / Capacity"},
        // .pageSize = 0;  // auto-calculate from terminal height
        .hasDetailView = true,
        .totalRecords  = roomsList.size(),
        .filterFn =
            [&searchState](size_t index) {
                return index < roomsList.size() &&
                       matchesRoom(roomsList[index], searchState);
            },
        .renderRowFn = [](size_t index) -> std::vector<Element> {
            const auto& room = roomsList[index];
            return {
                text(room.id) | color(Color::White),
                capacityElement(room),
            };
        },
        .renderDetailFn = &renderRoomDetail,
        .emptyMessage   = "No matching rooms",
    });
}

// ── Action menu & forms ──

Component createRoomActionMenu(std::shared_ptr<ActionState> state,
                               int&                         contentMode) {
    return createActionMenu(state->roomActions, state->rooms.action,
                            contentMode);
}

Component createRoomActionForms(std::shared_ptr<ActionState> state) {
    return Container::Tab(
        {roomUpdateForm(state), roomAddForm(state), roomRemoveForm(state)},
        &state->rooms.action);
}

} // namespace ui::events
