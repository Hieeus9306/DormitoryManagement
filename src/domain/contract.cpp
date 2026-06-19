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
Contract management
────────────────────────────────────────────────────────────────────────────────
*/
void addContract(const std::string& studentId, const std::string& roomId,
                 const base::Date& startDate, const base::Date& endDate) {
    size_t contractIdx = findActiveContractOfStudent(studentId);
    if (contractIdx != contractsList.size()) {
        return;
    }

    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        return;
    }
    if (startDate >= endDate) {
        return;
    }

    Contract newContract;

    newContract.id        = generateContractId();
    newContract.studentId = studentId;
    newContract.roomId    = roomId;
    newContract.startDate = startDate;
    newContract.endDate   = endDate;
    newContract.isActive  = true;

    contractsList.push_back(newContract);
}

void removeContract(const std::string& contractId) {
    size_t idx = findContract(contractId);

    if (idx == contractsList.size() || contractsList[idx].id != contractId) {
        return;
    }
    if (contractsList[idx].isActive == true) {
        return;
    }

    contractsList.erase_at(idx);
}

void extendContract(const std::string& contractId,
                    const base::Date&  newEndDate) {
    size_t idx = findContract(contractId);

    if (idx == contractsList.size() || contractsList[idx].id != contractId) {
        return;
    }
    if (contractsList[idx].isActive == false) {
        return;
    }
    if (contractsList[idx].endDate >= newEndDate) {
        return;
    }

    contractsList[idx].endDate = newEndDate;
}

void terminateContract(const std::string& contractId) {
    size_t idx = findContract(contractId);

    if (idx == contractsList.size() || contractsList[idx].id != contractId) {
        return;
    }
    if (contractsList[idx].isActive == false) {
        return;
    }

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
    if (contractIdx != contractsList.size()) {
        return;
    }

    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        return;
    }
    if (!roomsList[roomIdx].hasAvailableSlot()) {
        return;
    }

    if (startDate >= endDate) {
        return;
    }

    addStudentToRoom(roomId, studentId);
    addContract(studentId, roomId, startDate, endDate);
}

void transferRoom(const std::string& studentId, const std::string& newRoomId,
                  const base::Date& newStartDate,
                  const base::Date& newEndDate) {
    size_t oldContractIdx = findActiveContractOfStudent(studentId);
    if (oldContractIdx == contractsList.size()) {
        return;
    }
    std::string oldRoomId     = contractsList[oldContractIdx].roomId;
    std::string oldContractId = contractsList[oldContractIdx].id;

    if (oldRoomId == newRoomId) {
        return;
    }
    if (contractsList[oldContractIdx].isActive == false) {
        return;
    }

    size_t newRoomIdx = findRoom(newRoomId);
    if (newRoomIdx == roomsList.size() ||
        roomsList[newRoomIdx].id != newRoomId) {
        return;
    }
    if (!roomsList[newRoomIdx].hasAvailableSlot()) {
        return;
    }

    if (newStartDate >= newEndDate) {
        return;
    }

    removeStudentFromRoom(oldRoomId, studentId);
    terminateContract(oldContractId);
    registerRoom(studentId, newRoomId, newStartDate, newEndDate);
}

void checkoutRoom(const std::string& studentId) {
    size_t idx = findActiveContractOfStudent(studentId);

    if (idx == contractsList.size()) {
        return;
    }
    if (contractsList[idx].isActive == false) {
        return;
    }

    std::string contractId = contractsList[idx].id;
    std::string roomId     = contractsList[idx].roomId;

    removeStudentFromRoom(roomId, studentId);
    terminateContract(contractId);
}
