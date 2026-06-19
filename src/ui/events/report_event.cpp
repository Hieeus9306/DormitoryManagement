#include "report_event.hpp"

#include "../Theme.hpp"
#include "domain/report.hpp"
#include "state/state.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>

namespace ui::events {
namespace {

using namespace ftxui;

std::string fmtNum(double value, int precision = 2) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

// ── Overdue invoices ──

Element overdueInvoiceReport() {
    const auto overdue = findOverdueInvoices();
    Elements   rows    = {
        theme::headerRow({"Invoice ID", "Room ID", "Period", "Total"}),
        theme::divider(),
    };
    for (size_t i = 0; i < overdue.size(); ++i) {
        const auto* inv = overdue[i];
        rows.push_back(theme::zebraRow({inv->id, inv->roomId,
                                        std::to_string(inv->month) + "/" +
                                            std::to_string(inv->year),
                                        fmtNum(inv->totalAmount)},
                                       i) |
                       bgcolor(i % 2 == 1 ? theme::stripe : theme::panelLight));
        rows.push_back(theme::divider());
    }
    if (overdue.empty()) {
        rows.push_back(theme::emptyState("No overdue invoices"));
    }
    rows.push_back(
        hbox({
            text(" Total overdue: ") | color(theme::mutedText),
            text(std::to_string(overdue.size())) | bold | color(theme::danger),
        }) |
        bold);
    return theme::reportBlock("Overdue invoices", std::move(rows));
}

// ── Expiring contracts ──

Element expiringContractReport() {
    const base::Date today    = base::Date::today();
    const auto       expiring = findExpiringContracts();
    Elements         rows     = {
        theme::headerRow(
            {"Contract ID", "Student ID", "Room ID", "End date", "Days left"}),
        theme::divider(),
    };
    for (size_t i = 0; i < expiring.size(); ++i) {
        const auto* c        = expiring[i];
        const int   daysLeft = daysBetween(today, c->endDate);
        auto        daysEl   = text(std::to_string(daysLeft));
        if (daysLeft <= 7) {
            daysEl = daysEl | color(theme::danger) | bold;
        } else {
            daysEl = daysEl | color(theme::warning);
        }
        rows.push_back(
            hbox({
                theme::dataRow({c->id, c->studentId, c->roomId,
                                c->endDate.format(), std::to_string(daysLeft)}),
            }) |
            bgcolor(i % 2 == 1 ? theme::stripe : theme::panelLight));
        rows.push_back(theme::divider());
    }
    if (expiring.empty()) {
        rows.push_back(
            theme::emptyState("No contracts expiring within one month"));
    }
    rows.push_back(hbox({
        text(" Total expiring: ") | color(theme::mutedText),
        text(std::to_string(expiring.size())) | bold | color(theme::warning),
    }));
    return theme::reportBlock("Contracts expiring soon", std::move(rows));
}

// ── Room usage ──

Element roomUsageReport() {
    size_t totalStudents = 0, totalCapacity = 0, fullRooms = 0;
    for (const auto& room : roomsList) {
        totalStudents += room.currentStudents();
        totalCapacity += room.type;
        if (!room.hasAvailableSlot())
            ++fullRooms;
    }
    const double bedUsage =
        totalCapacity == 0
            ? 0.0
            : (static_cast<double>(totalStudents) / totalCapacity);
    const double occRate =
        roomsList.empty() ? 0.0 : occupancyReport().occupancyRate / 100.0;

    const int barWidth = 20;

    Elements rows = {
        theme::headerRow({"Metric", "Value", ""}),
        theme::divider(),

        hbox({
            text(" Current students   ") | color(theme::mutedText),
            text(std::to_string(totalStudents)) | flex | color(Color::White),
        }),
        theme::divider(),

        hbox({
            text(" Total capacity     ") | color(theme::mutedText),
            text(std::to_string(totalCapacity)) | flex | color(Color::White),
        }),
        theme::divider(),

        hbox({
            text(" Bed usage          ") | color(theme::mutedText),
            theme::progressBar(bedUsage, barWidth) | flex,
            text(" " + fmtNum(bedUsage * 100.0) + "%") |
                color(bedUsage > 0.8   ? theme::danger
                      : bedUsage > 0.5 ? theme::warning
                                       : theme::success) |
                bold | size(WIDTH, EQUAL, 8),
        }),
        theme::divider(),

        hbox({
            text(" Room occupancy     ") | color(theme::mutedText),
            theme::progressBar(occRate, barWidth) | flex,
            text(" " + fmtNum(occRate * 100.0) + "%") |
                color(occRate > 0.8   ? theme::danger
                      : occRate > 0.5 ? theme::warning
                                      : theme::success) |
                bold | size(WIDTH, EQUAL, 8),
        }),
        theme::divider(),

        hbox({
            text(" Full rooms         ") | color(theme::mutedText),
            text(std::to_string(fullRooms)) | flex |
                color(fullRooms > 0 ? theme::warning : theme::success),
        }),
        theme::divider(),

        hbox({
            text(" Rented rooms       ") | color(theme::mutedText),
            text(std::to_string(occupancyReport().rentedRooms)) | flex |
                color(Color::White),
        }),
        theme::divider(),

        hbox({
            text(" Empty rooms        ") | color(theme::mutedText),
            text(std::to_string(occupancyReport().emptyRooms)) | flex |
                color(occupancyReport().emptyRooms > 0 ? theme::mutedText
                                                       : theme::success),
        }),
    };

    return theme::reportBlock("Room usage rate", std::move(rows));
}

} // namespace

Component createReportDashboard() {
    return Renderer([] {
        return vbox({
                   hbox({
                       overdueInvoiceReport() | flex,
                       expiringContractReport() | flex,
                   }) | flex,
                   roomUsageReport(),
               }) |
               flex;
    });
}

Component createReportActionMenu() { return Container::Vertical({}); }

Component createReportActionForms() {
    return Renderer([] {
        return text("Report does not have action forms.") |
               color(theme::mutedText) | center | border |
               bgcolor(theme::panelLight);
    });
}

} // namespace ui::events
