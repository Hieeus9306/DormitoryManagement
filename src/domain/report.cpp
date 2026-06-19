#include "report.hpp"

#include "config.hpp"
#include "libs/date.hpp"
#include "libs/vector.hpp"


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
    base::Date currentDate = base::Date::today();

    base::Vector<Contract*> reportList;

    for (auto& contract : contractsList) {
        if (!contract.isActive)
            continue;

        int monthsDiff = (contract.endDate.year() - currentDate.year()) * 12 +
                         (contract.endDate.month() - currentDate.month());

        if (monthsDiff <= 1) {
            reportList.push_back(&contract);
        }
    }
    return reportList;
}

