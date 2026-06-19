#pragma once

#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Generate id
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  generate a new unique ID for invoice
 * @return new invoice ID
 */
std::string generateInvoiceId();

/*
────────────────────────────────────────────────────────────────────────────────
Calculate service fee
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  calculate electricity fee
 * @param  oldIndex: old electricity index
 * @param  newIndex: new electricity index
 * @return electricity fee
 */
double electricityFee(double oldIndex, double newIndex);

/**
 * @brief  calculate water fee
 * @param  oldIndex: old water index
 * @param  newIndex: new water index
 * @return water fee
 */
double waterFee(double oldIndex, double newIndex);

/*
────────────────────────────────────────────────────────────────────────────────
Find service invoice
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  find invoice by invoiceId
 * @param  invoiceId: id of invoice to find
 * @return lower bound index of invoice in serviceInvoicesList
 */
size_t findInvoice(const std::string& invoiceId);

/**
 * @brief  find invoice of room on (month, year)
 * @param  roomId   : ID of the room
 * @param  month    : month of the invoice
 * @param  year     : year of the invoice
 * @return index of latest invoice is found, serviceInvoicesList.size()
 * otherwise
 */
size_t findInvoice(const std::string& roomId, size_t month, size_t year);

/**
 * @brief  find the latest invoice for a specific room
 * @param  roomId: ID of the room
 * @return index of the latest invoice if found, serviceInvoicesList.size()
 * otherwise
 */
size_t findLastInvoice(const std::string& roomId);

/*
────────────────────────────────────────────────────────────────────────────────
Check logic of invoice management
────────────────────────────────────────────────────────────────────────────────
 */
bool isInvoiceExist(const std::string& roomId, size_t month, size_t year);
bool isInvoiceExist(const std::string& invoiceId);

bool canCreateInvoice(const std::string& roomId, size_t month, size_t year,
                      double electricity, double water, std::string& message);
bool canUpdatePaymentStatus(const std::string& invoiceId,
                            std::string&       message);
/*
────────────────────────────────────────────────────────────────────────────────
Service invoice management
────────────────────────────────────────────────────────────────────────────────
 */

/**
 * @brief  create new invoice and add to serviceInvoicesList
 * @param  roomId              : ID of the room
 * @param  month               : month of the invoice
 * @param  year                : year of the invoice
 * @param  newElectricityIndex : new electricity index
 * @param  newWaterIndex       : new water index
 */
void createInvoice(const std::string& roomId, size_t month, size_t year,
                   double newElectricityIndex, double newWaterIndex);

/**
 * @brief  update payment status of an invoice
 * @param  invoiceId : id of the invoice to update
 * @param  status    : new payment status
 */
void updateInvoicePaymentStatus(const std::string& invoiceId, bool status);