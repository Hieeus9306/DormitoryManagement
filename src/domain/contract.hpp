#pragma once

#include "libs/date.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Contract management
────────────────────────────────────────────────────────────────────────────────
*/

/// @brief  generate a new unique ID for contract
/// @return new contract ID
std::string generateContractId();

/// @brief  find contract by contractId
/// @param  contractId: id of contract to find
/// @return lower bound of contractId in contractsList
size_t findContract(const std::string& contractId);

/// @brief  find active contract of a student
/// @param  studentId: id of student to find
/// @return index of contract if found, contractsList.size() otherwise
size_t findActiveContractOfStudent(const std::string& studentId);

/// @brief  add new contract to contractsList
/// @param  studentId    : id of student associated with the contract
/// @param  roomId       : id of room associated with the contract
/// @param  startDate    : start date of the contract
/// @param  endDate      : end date of the contract
void addContract(const std::string& studentId, const std::string& roomId,
                 const base::Date& startDate, const base::Date& endDate);

/// @brief  remove contract from contractsList
/// @param  contractId: id of contract to remove
void removeContract(const std::string& contractId);

/// @brief  extend end date of a contract
/// @param  contractId: id of contract to extend
/// @param  newEndDate: new end date of the contract
void extendContract(const std::string& contractId,
                    const base::Date&  newEndDate);

/// @brief  terminate a contract
/// @param  contractId: id of contract to terminate
void terminateContract(const std::string& contractId);

/*
────────────────────────────────────────────────────────────────────────────────
Room assignment management
────────────────────────────────────────────────────────────────────────────────
*/

/// @brief  assign student to a room
/// @param  studentId    : id of student to assign
/// @param  roomId       : id of room to assign student to
/// @param  startDate    : start date of the assignment
/// @param  endDate      : end date of the assignment
void registerRoom(const std::string& studentId, const std::string& roomId,
                  const base::Date& startDate, const base::Date& endDate);

/// @brief  transfer student to a different room
/// @param  studentId    : id of student to transfer
/// @param  newRoomId    : id of new room to transfer student to
/// @param  newStartDate : start date of the new contract
/// @param  newEndDate   : end date of the new contract
void transferRoom(const std::string& studentId, const std::string& newRoomId,
                  const base::Date& newStartDate, const base::Date& newEndDate);

/// @brief  checkout student from a room
/// @param  studentId: id of student to checkout
void checkoutRoom(const std::string& studentId);
