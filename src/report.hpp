#pragma once

#include "config.hpp"
#include "libs/date.hpp"
#include "libs/vector.hpp"

/**
 * @brief  Finds service invoices that are overdue by at least one month.
 * @return A vector of pointers to overdue ServiceInvoice objects in
 * ServiceInvoicesList.
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

/**
 * @struct OccupancyStatus
 * @brief  Represents the occupancy status of the dormitory.
 */
struct OccupancyStatus {
    size_t rentedRooms;
    size_t emptyRooms;
    double occupancyRate;
};

/**
 * @brief  Calculates the occupancy status of the dormitory.
 * @return An OccupancyStatus struct containing rented rooms, empty rooms, and
 * occupancy rate.
 */
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

/**
 * @brief  Finds contracts that are expiring within the next month.
 * @return A vector of pointers to expiring Contract objects in ContractsList.
 */
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
