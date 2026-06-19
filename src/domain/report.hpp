#pragma once

#include "config.hpp"
#include "libs/date.hpp"
#include "libs/vector.hpp"

/**
 * @brief  Finds service invoices that are overdue by at least one month.
 * @return A vector of pointers to overdue ServiceInvoice objects in
 * ServiceInvoicesList.
 */
base::Vector<ServiceInvoice*> findOverdueInvoices();

/**
 * @brief  Calculates the occupancy status of the dormitory.
 * @return An OccupancyStatus struct containing rented rooms, empty rooms, and
 * occupancy rate.
 */
OccupancyStatus occupancyReport();

/**
 * @brief  Finds contracts that are expiring within the next month.
 * @return A vector of pointers to expiring Contract objects in ContractsList.
 */
base::Vector<Contract*> findExpiringContracts();
