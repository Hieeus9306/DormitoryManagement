#include "invoice_event.hpp"

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

std::string formatPeriod(size_t month, size_t year) {
    std::ostringstream output;
    output << std::setfill('0') << std::setw(2) << month << "/" << year;
    return output.str();
}

}  // namespace

ftxui::Element buildInvoiceView(const std::string& searchText) {
    TableData data;
    data.headers = {"Invoice", "Room", "Period", "Electricity", "Water",
                    "Total", "Status"};

    for (const auto& invoice : serviceInvoicesList) {
        data.rows.push_back(
            {invoice.id, invoice.roomId,
             formatPeriod(invoice.month, invoice.year),
             formatNumber(invoice.newElectricityIndex -
                          invoice.oldElectricityIndex),
             formatNumber(invoice.newWaterIndex - invoice.oldWaterIndex),
             formatNumber(invoice.totalAmount),
             invoice.isPaid ? "Paid" : "Unpaid"});
    }

    return renderTable(data, searchText);
}

}  // namespace ui::events
