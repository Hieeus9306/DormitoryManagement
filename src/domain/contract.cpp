#include "domain/contract.hpp"

#include "domain/room.hpp"
#include "libs/algorithms.hpp"
#include "state/state.hpp"

/*
────────────────────────────────────────────────────────────────────────────────
Generate contract ID
────────────────────────────────────────────────────────────────────────────────
*/
std::string generateContractId() {
    size_t maxIdx = 0;
    if (!contractsList.empty()) {
        auto lastContract = contractsList.back();
        maxIdx            = std::stoull(lastContract.id.substr(2));
    }

    int    digitNumber = 0;
    size_t nextIdx     = maxIdx + 1;
    while (nextIdx > 0) {
        nextIdx /= 10;
        digitNumber++;
    }
    std::string newId = "CT";
    for (int i = 0; i < 8 - digitNumber; i++) {
        newId += "0";
    }
    newId += std::to_string(maxIdx + 1);

    return newId;
}

/*
────────────────────────────────────────────────────────────────────────────────
Find contract
────────────────────────────────────────────────────────────────────────────────
*/
size_t findContract(const std::string& contractId) {
    auto it = binarySearch(contractsList, contractId,
                           [](const Contract& contract, const std::string& id) {
                               return contract.id < id;
                           });

    return it - contractsList.begin();
}

size_t findActiveContractOfStudent(const std::string& studentId) {
    for (auto& contract : contractsList) {
        if (contract.studentId == studentId && contract.isActive == true) {
            return &contract - contractsList.begin();
        }
    }
    return contractsList.size();
}

/*
────────────────────────────────────────────────────────────────────────────────
Check logic of contract mangagement
────────────────────────────────────────────────────────────────────────────────
*/
bool isContractExist(const std::string& contractId) {
    size_t idx = findContract(contractId);
    return (idx != contractsList.size() && contractsList[idx].id == contractId);
}

bool isActiveContractExist(const std::string& studentId) {
    size_t idx = findActiveContractOfStudent(studentId);
    return (idx != contractsList.size());
}

bool isValidContractRange(const base::Date& startDate,
                          const base::Date& endDate) {
    return (startDate < endDate);
}

bool canExtendContract(const std::string& contractId,
                       const base::Date&  newEndDate) {
    if (!isContractExist(contractId)) {
        return false;
    }
    size_t idx = findContract(contractId);
    if (contractsList[idx].isActive == false) {
        return false;
    }
    if (contractsList[idx].endDate >= newEndDate) {
        return false;
    }

    return true;
}

bool canRegisterRoom(const std::string& studentId, const std::string& roomId,
                     const base::Date& startDate, const base::Date& endDate,
                     std::string& message) {
    size_t contractIdx = findActiveContractOfStudent(studentId);
    if (contractIdx != contractsList.size()) {
        message = "Student already has an active contract.";
        return false;
    }

    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        message = "Room not found.";
        return false;
    }
    if (roomsList[roomIdx].hasAvailableSlot() == false) {
        message = "Room has no available slot.";
        return false;
    }

    if (startDate >= endDate) {
        message = "Invalid date range.";
        return false;
    }
    return true;
}

bool canCheckoutRoom(const std::string& studentId, std::string& message) {
    size_t idx = findActiveContractOfStudent(studentId);

    if (idx == contractsList.size()) {
        message = "No active contract found for the student.";
        return false;
    }
    if (contractsList[idx].isActive == false) {
        message = "The contract is already inactive.";
        return false;
    }

    return true;
}

bool canTransferRoom(const std::string& studentId, const std::string& newRoomId,
                     const base::Date& newStartDate,
                     const base::Date& newEndDate, std::string& message) {
    size_t oldContractIdx = findActiveContractOfStudent(studentId);
    if (oldContractIdx == contractsList.size()) {
        message = "No active contract found for the student.";
        return false;
    }
    std::string oldRoomId     = contractsList[oldContractIdx].roomId;
    std::string oldContractId = contractsList[oldContractIdx].id;

    if (oldRoomId == newRoomId) {
        message = "The student is already in the specified room.";
        return false;
    }
    if (contractsList[oldContractIdx].isActive == false) {
        message = "The contract is already inactive.";
        return false;
    }

    size_t newRoomIdx = findRoom(newRoomId);
    if (newRoomIdx == roomsList.size() ||
        roomsList[newRoomIdx].id != newRoomId) {
        message = "New room not found.";
        return false;
    }
    if (roomsList[newRoomIdx].hasAvailableSlot() == false) {
        message = "New room has no available slot.";
        return false;
    }

    if (newStartDate >= newEndDate) {
        message = "Invalid date range.";
        return false;
    }

    return true;
}

/*
────────────────────────────────────────────────────────────────────────────────
Contract management
────────────────────────────────────────────────────────────────────────────────
*/
void addContract(const std::string& studentId, const std::string& roomId,
                 const base::Date& startDate, const base::Date& endDate) {
    size_t contractIdx = findActiveContractOfStudent(studentId);

    Contract newContract;

    newContract.id        = generateContractId();
    newContract.studentId = studentId;
    newContract.roomId    = roomId;
    newContract.startDate = startDate;
    newContract.endDate   = endDate;
    newContract.isActive  = true;

    contractsList.push_back(newContract);
}

void extendContract(const std::string& contractId,
                    const base::Date&  newEndDate) {
    size_t idx = findContract(contractId);

    contractsList[idx].endDate = newEndDate;
}

void terminateContract(const std::string& contractId) {
    size_t idx = findContract(contractId);

    contractsList[idx].isActive = false;
}

/*
────────────────────────────────────────────────────────────────────────────────
Room assignment management
────────────────────────────────────────────────────────────────────────────────
*/
void registerRoom(const std::string& studentId, const std::string& roomId,
                  const base::Date& startDate, const base::Date& endDate) {
    size_t contractIdx = findActiveContractOfStudent(studentId);
    size_t roomIdx     = findRoom(roomId);

    addStudentToRoom(roomId, studentId);
    addContract(studentId, roomId, startDate, endDate);
}

void transferRoom(const std::string& studentId, const std::string& newRoomId,
                  const base::Date& newStartDate,
                  const base::Date& newEndDate) {

    size_t      oldContractIdx = findActiveContractOfStudent(studentId);
    std::string oldRoomId      = contractsList[oldContractIdx].roomId;
    std::string oldContractId  = contractsList[oldContractIdx].id;

    size_t newRoomIdx = findRoom(newRoomId);

    removeStudentFromRoom(oldRoomId, studentId);
    terminateContract(oldContractId);
    registerRoom(studentId, newRoomId, newStartDate, newEndDate);
}

void checkoutRoom(const std::string& studentId) {
    size_t idx = findActiveContractOfStudent(studentId);

    std::string contractId = contractsList[idx].id;
    std::string roomId     = contractsList[idx].roomId;

    removeStudentFromRoom(roomId, studentId);
    terminateContract(contractId);
}