#include "contract_event.hpp"

#include "../table.hpp"
#include "config.hpp"

namespace ui::events {

ftxui::Element buildContractView(const std::string& searchText) {
    TableData data;
    data.headers = {"Contract", "Student", "Room", "Start date", "End date",
                    "Status"};

    for (const auto& contract : contractsList) {
        data.rows.push_back(
            {contract.id, contract.studentId, contract.roomId,
             contract.startDate.format(), contract.endDate.format(),
             contract.isActive ? "Active" : "Inactive"});
    }

    return renderTable(data, searchText);
}

}  // namespace ui::events
