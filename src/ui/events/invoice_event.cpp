#include "invoice_event.hpp"

#include "../Dashboard.hpp"
#include "../Theme.hpp"
#include "domain/invoice.hpp"
#include "domain/room.hpp"
#include "state/state.hpp"

#include "libs/string_utils.hpp"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>

namespace ui::events {
namespace {

using namespace ftxui;

constexpr int kInvoiceSection = 3;

// ── Validation helpers ──

size_t existingRoomIndex(const std::string& roomId) {
    const size_t roomIndex = findRoom(roomId);
    if (roomIndex == roomsList.size() || roomsList[roomIndex].id != roomId) {
        return roomsList.size();
    }
    return roomIndex;
}

size_t existingInvoiceIndex(const std::string& invoiceId) {
    const size_t invoiceIndex = findInvoice(invoiceId);
    if (invoiceIndex == serviceInvoicesList.size() ||
        serviceInvoicesList[invoiceIndex].id != invoiceId) {
        return serviceInvoicesList.size();
    }
    return invoiceIndex;
}

bool validInvoicePeriod(size_t month, size_t year, std::string& message) {
    if (month < 1 || month > 12 || year == 0) {
        message = "Invoice period must use month 1-12 and a valid year.";
        return false;
    }
    return true;
}

bool validInvoiceIndexes(const std::string& roomId, double electricity,
                         double water, std::string& message) {
    if (electricity < 0 || water < 0) {
        message = "Electricity and water indexes cannot be negative.";
        return false;
    }
    const size_t lastInvoiceIndex = findLastInvoice(roomId);
    if (lastInvoiceIndex == serviceInvoicesList.size()) {
        return true;
    }
    const auto& lastInvoice = serviceInvoicesList[lastInvoiceIndex];
    if (electricity < lastInvoice.newElectricityIndex ||
        water < lastInvoice.newWaterIndex) {
        message = "New indexes must be >= last invoice indexes.";
        return false;
    }
    return true;
}

bool canCreateInvoice(const std::string& roomId, size_t month, size_t year,
                      double electricity, double water, std::string& message) {
    if (existingRoomIndex(roomId) == roomsList.size()) {
        message = "Room ID does not exist.";
        return false;
    }
    if (!validInvoicePeriod(month, year, message)) {
        return false;
    }
    if (findInvoice(roomId, month, year) != serviceInvoicesList.size()) {
        message = "Invoice already exists for this room and period.";
        return false;
    }
    return validInvoiceIndexes(roomId, electricity, water, message);
}

bool canUpdatePaymentStatus(const std::string& invoiceId,
                            std::string&       message) {
    if (invoiceId.empty()) {
        message = "Invoice ID is required.";
        return false;
    }
    if (existingInvoiceIndex(invoiceId) == serviceInvoicesList.size()) {
        message = "Invoice ID does not exist.";
        return false;
    }
    return true;
}

// ── Formatting ──

std::string formatPeriod(size_t month, size_t year) {
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << month << "/" << year;
    return out.str();
}

// ── Search ──

bool matchesInvoice(const ServiceInvoice& invoice,
                    const SearchState&    searchState) {
    const std::string keyword = searchKeyword(searchState, kInvoiceSection);
    switch (selectedSearchField(searchState, kInvoiceSection)) {
    case 0:
        return searchMatches(invoice.id, keyword);
    case 1:
        return searchMatches(invoice.roomId, keyword);
    default:
        return true;
    }
}

// ── Detail view ──

Element renderInvoiceDetail(size_t index) {
    const auto& inv       = serviceInvoicesList[index];
    auto        formatVal = [](double v) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << v;
        return out.str();
    };

    return vbox({
        theme::sectionTitle("Invoice information"),
        separator(),
        theme::detailLine("ID", inv.id),
        theme::detailLine("Room ID", inv.roomId),
        theme::detailLine("Month", std::to_string(inv.month)),
        theme::detailLine("Year", std::to_string(inv.year)),
        theme::detailLine("Old electricity",
                          formatVal(inv.oldElectricityIndex)),
        theme::detailLine("New electricity",
                          formatVal(inv.newElectricityIndex)),
        theme::detailLine("Old water", formatVal(inv.oldWaterIndex)),
        theme::detailLine("New water", formatVal(inv.newWaterIndex)),
        theme::detailLine("Total amount", formatVal(inv.totalAmount)),
        theme::detailLine("Paid", inv.isPaid ? "true" : "false"),
    });
}

// ── Form: Create ──

Component invoiceCreateForm(std::shared_ptr<ActionState> state) {
    auto roomId      = createTextInput(state->invoices.roomId, "Room ID");
    auto month       = createTextInput(state->invoices.month, "Month");
    auto year        = createTextInput(state->invoices.year, "Year");
    auto electricity = createTextInput(state->invoices.electricityIndex,
                                       "New electricity index");
    auto water = createTextInput(state->invoices.waterIndex, "New water index");
    auto button = Button(
        "createInvoice",
        [state] {
            size_t     month       = 0;
            size_t     year        = 0;
            double     electricity = 0;
            double     water       = 0;
            const auto roomId      = nonEmpty(state->invoices.roomId);
            if (roomId.empty() || !parseSize(state->invoices.month, month) ||
                !parseSize(state->invoices.year, year) ||
                !parseDouble(state->invoices.electricityIndex, electricity) ||
                !parseDouble(state->invoices.waterIndex, water)) {
                state->invoices.message =
                    "Room ID, month, year, electricity, and water "
                    "are required.";
                return;
            }

            std::string message;
            if (!canCreateInvoice(roomId, month, year, electricity, water,
                                  message)) {
                state->invoices.message = message;
                return;
            }

            createInvoice(roomId, month, year, electricity, water);
            state->invoices.message =
                "createInvoice completed for " + roomId + ".";
        },
        ButtonOption::Ascii());
    auto container =
        Container::Vertical({roomId, month, year, electricity, water, button});

    return Renderer(container, [=] {
        return titledForm("Invoices / Create Invoice",
                          {
                              field("Room ID", roomId),
                              field("Month", month),
                              field("Year", year),
                              field("Electricity", electricity),
                              field("Water", water),
                              button->Render() | center,
                          },
                          state->invoices.message);
    });
}

// ── Form: Payment Status ──

Component invoicePaymentForm(std::shared_ptr<ActionState> state) {
    auto invoiceId = createTextInput(state->invoices.invoiceId, "Invoice ID");
    auto paid      = Checkbox("Paid", &state->invoices.isPaid);
    auto button    = Button(
        "updatePaymentStatus",
        [state] {
            const auto invoiceId = nonEmpty(state->invoices.invoiceId);

            std::string message;
            if (!canUpdatePaymentStatus(invoiceId, message)) {
                state->invoices.message = message;
                return;
            }

            updatePaymentStatus(invoiceId, state->invoices.isPaid);
            state->invoices.message =
                "updatePaymentStatus completed for " + invoiceId + ".";
        },
        ButtonOption::Ascii());
    auto container = Container::Vertical({invoiceId, paid, button});

    return Renderer(container, [=] {
        return titledForm("Invoices / Payment Status",
                          {
                              field("Invoice ID", invoiceId),
                              paid->Render(),
                              button->Render() | center,
                          },
                          state->invoices.message);
    });
}

} // namespace

// ── Dashboard ──

Component createInvoiceDashboard(SearchState& searchState) {
    return createDashboard({
        .headers = {"Id", "Status"},
        // .pageSize = 0;  // auto-calculate from terminal height
        .hasDetailView = true,
        .totalRecords  = serviceInvoicesList.size(),
        .filterFn =
            [&searchState](size_t index) {
                const size_t r = serviceInvoicesList.size() - 1 - index;
                return r < serviceInvoicesList.size() &&
                       matchesInvoice(serviceInvoicesList[r], searchState);
            },
        .renderRowFn = [](size_t index) -> std::vector<Element> {
            const size_t r   = serviceInvoicesList.size() - 1 - index;
            const auto&  inv = serviceInvoicesList[r];
            return {
                text(inv.id) | color(Color::White),
                theme::statusBadge(inv.isPaid ? "true" : "false", inv.isPaid),
            };
        },
        .renderDetailFn =
            [](size_t index) {
                const size_t r = serviceInvoicesList.size() - 1 - index;
                return renderInvoiceDetail(r);
            },
        .emptyMessage = "No matching invoices",
    });
}

// ── Action menu & forms ──

Component createInvoiceActionMenu(std::shared_ptr<ActionState> state,
                                  int&                         contentMode) {
    return createActionMenu(state->invoiceActions, state->invoices.action,
                            contentMode);
}

Component createInvoiceActionForms(std::shared_ptr<ActionState> state) {
    return Container::Tab({invoiceCreateForm(state), invoicePaymentForm(state)},
                          &state->invoices.action);
}

} // namespace ui::events
