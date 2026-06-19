#pragma once

#include "core/types.hpp"
#include "libs/date.hpp"
#include "libs/vector.hpp"

#include <ctime>

/// @brief  Convert a Date object to time_t for day distance calculation.
/// @param  date: date value to convert
/// @return time_t value at local midnight
std::time_t dateToTime(base::Date date);

/// @brief  Calculates the number of days from startDate to endDate.
/// @param  startDate: the starting date
/// @param  endDate  : the ending date
/// @return number of days between the two dates
int daysBetween(base::Date startDate, base::Date endDate);

/// @brief  Finds service invoices that are overdue by at least one month.
/// @return A vector of pointers to overdue ServiceInvoice objects.
base::Vector<ServiceInvoice*> findOverdueInvoices();

/// @struct OccupancyStatus
/// @brief  Represents the occupancy status of the dormitory.
struct OccupancyStatus {
    size_t rentedRooms;
    size_t emptyRooms;
    double occupancyRate;
};

/// @brief  Calculates the occupancy status of the dormitory.
/// @return An OccupancyStatus struct containing rented rooms, empty rooms,
/// and occupancy rate.
OccupancyStatus occupancyReport();

/// @brief  Finds active contracts that will expire in less than one month.
/// @return A vector of pointers to expiring Contract objects.
base::Vector<Contract*> findExpiringContracts();