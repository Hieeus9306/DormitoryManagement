#include "domain/report.hpp"

#include "state/state.hpp"

/*
────────────────────────────────────────────────────────────────────────────────
Date utilities
────────────────────────────────────────────────────────────────────────────────
*/
std::time_t dateToTime(base::Date date) {
    std::tm value{};
    value.tm_mday  = static_cast<int>(date.day());
    value.tm_mon   = static_cast<int>(date.month()) - 1;
    value.tm_year  = static_cast<int>(date.year()) - 1900;
    value.tm_isdst = -1;
    return std::mktime(&value);
}

int daysBetween(base::Date startDate, base::Date endDate) {
    constexpr int secondsPerDay = 60 * 60 * 24;
    const double  seconds =
        std::difftime(dateToTime(endDate), dateToTime(startDate));
    return static_cast<int>(seconds / secondsPerDay);
}

/*
────────────────────────────────────────────────────────────────────────────────
Reports
────────────────────────────────────────────────────────────────────────────────
*/
base::Vector<ServiceInvoice*> findOverdueInvoices() {
    base::Date currentDate = base::Date::today();

    base::Vector<ServiceInvoice*> reportList;

    for (auto& invoice : serviceInvoicesList) {
        if (!invoice.isPaid) {
            int monthsDiff = (currentDate.year() - invoice.year) * 12 +
                             (currentDate.month() - invoice.month);

            if (monthsDiff >= 1) {
                reportList.push_back(&invoice);
            }
        }
    }
    return reportList;
}

OccupancyStatus occupancyReport() {
    OccupancyStatus report     = {0, 0, 0.0};
    size_t          totalRooms = roomsList.size();

    for (const auto& room : roomsList) {
        if (room.currentStudents() > 0) {
            report.rentedRooms++;
        } else {
            report.emptyRooms++;
        }
    }

    if (totalRooms > 0) {
        report.occupancyRate =
            (static_cast<double>(report.rentedRooms) / totalRooms) * 100.0;
    }

    return report;
}

base::Vector<Contract*> findExpiringContracts() {
    constexpr int expiringSoonDays = 30;
    base::Date    currentDate      = base::Date::today();

    base::Vector<Contract*> reportList;

    for (auto& contract : contractsList) {
        if (!contract.isActive)
            continue;

        int daysLeft = daysBetween(currentDate, contract.endDate);

        if (daysLeft >= 0 && daysLeft < expiringSoonDays) {
            reportList.push_back(&contract);
        }
    }
    return reportList;
}
