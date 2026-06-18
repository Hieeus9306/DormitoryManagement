#include "invoice.hpp"
#include "config.hpp"
#include "libs/algorithms.hpp"
#include "room.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Generate id
────────────────────────────────────────────────────────────────────────────────
*/
std::string generateInvoiceId() {
    size_t maxIdx = 0;
    if (!serviceInvoicesList.empty()) {
        auto lastInvoice = serviceInvoicesList.back();
        maxIdx           = std::stoull(lastInvoice.id.substr(2));
    }

    int    digitNumber = 0;
    size_t nextIdx     = maxIdx + 1;
    while (nextIdx > 0) {
        nextIdx /= 10;
        digitNumber++;
    }
    std::string newId = "SI";
    for (int i = 0; i < 8 - digitNumber; i++) {
        newId += "0";
    }
    newId += std::to_string(maxIdx + 1);

    return newId;
}

/*
────────────────────────────────────────────────────────────────────────────────
Calculate service fee
────────────────────────────────────────────────────────────────────────────────
*/
double electricityFee(double oldIndex, double newIndex) {
    double used = newIndex - oldIndex;
    if (used <= 0)
        return 0;

    double total = 0;
    if (used <= ELECTRICITY_TIER[0]) {
        total = used * ELECTRICITY_RATE[0];
    } else if (used <= ELECTRICITY_TIER[1]) {
        total = ELECTRICITY_TIER[0] * ELECTRICITY_RATE[0] +
                (used - ELECTRICITY_TIER[0]) * ELECTRICITY_RATE[1];
    } else if (used <= ELECTRICITY_TIER[2]) {
        total =
            ELECTRICITY_TIER[0] * ELECTRICITY_RATE[0] +
            (ELECTRICITY_TIER[1] - ELECTRICITY_TIER[0]) * ELECTRICITY_RATE[1] +
            (used - ELECTRICITY_TIER[1]) * ELECTRICITY_RATE[2];
    } else {
        total =
            ELECTRICITY_TIER[0] * ELECTRICITY_RATE[0] +
            (ELECTRICITY_TIER[1] - ELECTRICITY_TIER[0]) * ELECTRICITY_RATE[1] +
            (ELECTRICITY_TIER[2] - ELECTRICITY_TIER[1]) * ELECTRICITY_RATE[2] +
            (used - ELECTRICITY_TIER[2]) * ELECTRICITY_RATE[3];
    }
    return total;
}

double waterFee(double oldIndex, double newIndex) {
    if (newIndex < oldIndex)
        return 0;
    return (newIndex - oldIndex) * WATER_RATE;
}

/*
────────────────────────────────────────────────────────────────────────────────
Find service invoice
────────────────────────────────────────────────────────────────────────────────
 */
size_t findInvoice(const std::string& invoiceId) {
    auto it =
        binarySearch(serviceInvoicesList, invoiceId,
                     [](const ServiceInvoice& invoice, const std::string& id) {
                         return invoice.id < id;
                     });

    return it - serviceInvoicesList.begin();
}

size_t findInvoice(const std::string& roomId, size_t month, size_t year) {
    for (auto& invoice : serviceInvoicesList) {
        if (invoice.roomId == roomId && invoice.month == month &&
            invoice.year == year) {
            return &invoice - serviceInvoicesList.begin();
        }
    }
    return serviceInvoicesList.size();
}

size_t findLastInvoice(const std::string& roomId) {
    for (auto it = serviceInvoicesList.end(); it != serviceInvoicesList.begin();
         it--) {
        if ((it - 1)->roomId == roomId) {
            return (it - 1) - serviceInvoicesList.begin();
        }
    }
    return serviceInvoicesList.size();
}

/*
────────────────────────────────────────────────────────────────────────────────
Check logic of invoice management
────────────────────────────────────────────────────────────────────────────────
 */
bool isInvoiceExist(const std::string& roomId, size_t month, size_t year) {
    return findInvoice(roomId, month, year) != serviceInvoicesList.size();
}
bool isInvoiceExist(const std::string& invoiceId) {
    size_t idx = findInvoice(invoiceId);
    return (idx != serviceInvoicesList.size()) &&
           (serviceInvoicesList[idx].id == invoiceId);
}

bool canCreateInvoice(const std::string& roomId, size_t month, size_t year,
                      double electricity, double water, std::string& message) {

    if (isInvoiceExist(roomId, month, year)) {
        message = "Invoice already exists for this room and period.";
        return false;
    }

    if (electricity <= 0 || water <= 0) {
        message = "Invalid servise indices";
        return false;
    }
    if (!isRoomExist(roomId)) {
        message = "Room ID does not exist.";
        return false;
    }

    const size_t lastInvoiceIdx = findLastInvoice(roomId);
    if (lastInvoiceIdx == serviceInvoicesList.size()) {
        return true;
    }
    const auto& lastInvoice = serviceInvoicesList[lastInvoiceIdx];
    if (electricity < lastInvoice.newElectricityIndex ||
        water < lastInvoice.newWaterIndex) {
        message = "New indexes must be greater than or equal to the last "
                  "invoice indexes.";
        return false;
    }

    return true;
}

bool canUpdatePaymentStatus(const std::string& invoiceId,
                            std::string&       message) {
    if (invoiceId == "") {
        message = "Invoice ID is required.";
        return false;
    }
    if (isInvoiceExist(invoiceId)) {
        message = "Invoice ID does not exist.";
        return false;
    }
    return true;
}

/*
────────────────────────────────────────────────────────────────────────────────
Service invoice management
────────────────────────────────────────────────────────────────────────────────
 */
void createInvoice(const std::string& roomId, size_t month, size_t year,
                   double newElectricityIndex, double newWaterIndex) {
    if (findInvoice(roomId, month, year) != serviceInvoicesList.size()) {
        return;
    }

    ServiceInvoice newInvoice;

    newInvoice.id     = generateInvoiceId();
    newInvoice.roomId = roomId;
    newInvoice.month  = month;
    newInvoice.year   = year;

    auto lastIdx = findLastInvoice(roomId);
    if (lastIdx != serviceInvoicesList.size()) {
        auto& lastInvoice = serviceInvoicesList[lastIdx];

        newInvoice.oldElectricityIndex = lastInvoice.newElectricityIndex;
        newInvoice.oldWaterIndex       = lastInvoice.newWaterIndex;
    } else {
        newInvoice.oldElectricityIndex = 0;
        newInvoice.oldWaterIndex       = 0;
    }
    newInvoice.newElectricityIndex = newElectricityIndex;
    newInvoice.newWaterIndex       = newWaterIndex;
    newInvoice.totalAmount =
        electricityFee(newInvoice.oldElectricityIndex, newElectricityIndex) +
        waterFee(newInvoice.oldWaterIndex, newWaterIndex);

    newInvoice.isPaid = false;

    serviceInvoicesList.push_back(newInvoice);
}

void updatePaymentStatus(const std::string& invoiceId, const bool& status) {
    size_t idx = findInvoice(invoiceId);
    if (idx == serviceInvoicesList.size() ||
        serviceInvoicesList[idx].id != invoiceId) {
        return;
    }
    serviceInvoicesList[idx].isPaid = status;
}