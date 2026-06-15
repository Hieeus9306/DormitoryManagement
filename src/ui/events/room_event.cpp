#include "room_event.hpp"

#include "../table.hpp"
#include "config.hpp"

#include <iomanip>
#include <sstream>

namespace ui::events {
namespace {

std::string formatNumber(double value) {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << value;
    return output.str();
}

}  // namespace

ftxui::Element buildRoomView(const std::string& searchText) {
    TableData data;
    data.headers = {"Room", "Capacity", "Occupancy", "Price", "Availability"};

    for (const auto& room : roomsList) {
        data.rows.push_back(
            {room.id, std::to_string(room.type),
             std::to_string(room.currentStudents()) + " / " +
                 std::to_string(room.type),
             formatNumber(room.price),
             room.hasAvailableSlot() ? "Available" : "Full"});
    }

    return renderTable(data, searchText);
}

}  // namespace ui::events
