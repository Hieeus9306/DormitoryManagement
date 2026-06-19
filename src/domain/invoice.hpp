#pragma once

#include "core/types.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Invoice management
────────────────────────────────────────────────────────────────────────────────
*/

/// @brief  generate a new unique ID for invoice
/// @return new invoice ID
std::string generateInvoiceId();

/// @brief  calculate electricity fee
/// @param  oldIndex: old electricity index
/// @param  newIndex: new electricity index
/// @return electricity fee
double electricityFee(double oldIndex, double newIndex);

/// @brief  calculate water fee
/// @param  oldIndex: old water index
/// @param  newIndex: new water index
/// @return water fee
double waterFee(double oldIndex, double newIndex);

/// @brief  find invoice by invoiceId
/// @param  invoiceId: id of invoice to find
/// @return lower bound index of invoice in serviceInvoicesList
size_t findInvoice(const std::string& invoiceId);

/// @brief  find invoice of room on (month, year)
/// @param  roomId   : ID of the room
/// @param  month    : month of the invoice
/// @param  year     : year of the invoice
/// @return index of invoice if found, serviceInvoicesList.size() otherwise
size_t findInvoice(const std::string& roomId, size_t month, size_t year);

/// @brief  find the latest invoice for a specific room
/// @param  roomId: ID of the room
/// @return index of the latest invoice if found, serviceInvoicesList.size()
/// otherwise
size_t findLastInvoice(const std::string& roomId);

/// @brief  create new invoice and add to serviceInvoicesList
/// @param  roomId              : ID of the room
/// @param  month               : month of the invoice
/// @param  year                : year of the invoice
/// @param  newElectricityIndex : new electricity index
/// @param  newWaterIndex       : new water index
void createInvoice(const std::string& roomId, size_t month, size_t year,
                   double newElectricityIndex, double newWaterIndex);

/// @brief  update payment status of an invoice
/// @param  invoiceId : id of the invoice to update
/// @param  status    : new payment status
void updatePaymentStatus(const std::string& invoiceId, const bool& status);
