#pragma once

#include <libs/date.hpp>
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Generate contract ID
────────────────────────────────────────────────────────────────────────────────
*/
std::string generateContractId();

/*
────────────────────────────────────────────────────────────────────────────────
Find contract
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  find contract by contractId
 * @param  contractId: id of contract to find
 * @return lower bound of contractId in contractsList
 */
size_t findContract(const std::string& contractId);

/**
 * @brief  find active contract of a student
 * @param  studentId: id of student to find
 * @return index of contract if found, contractsList.size() otherwise
 */
size_t findActiveContractOfStudent(const std::string& studentId);

/*
────────────────────────────────────────────────────────────────────────────────
Check logic of contract mangagement
────────────────────────────────────────────────────────────────────────────────
*/
/// @brief  check if contract exists
bool isContractExist(const std::string& contractId);
/// @brief  check if active contract of student exists
bool isActiveContractExist(const std::string& studentId);
/// @brief  check if contract range is valid
bool isValidContractRange(const base::Date& startDate,
                          const base::Date& endDate);

/// @brief check if contract can be extended
bool canExtendContract(const std::string& contractId,
                       const base::Date&  newEndDate);

/// @brief check if student can be assigned to room
bool canRegisterRoom(const std::string& studentId, const std::string& roomId,
                     const base::Date& startDate, const base::Date& endDate,
                     std::string& message);
///@brief check if student can be transferred to different room
bool canTransferRoom(const std::string& studentId, const std::string& newRoomId,
                     const base::Date& newStartDate,
                     const base::Date& newEndDate, std::string& message);
/// @brief check if student can checkout from room
bool canCheckoutRoom(const std::string& studentId, std::string& message);

/*
────────────────────────────────────────────────────────────────────────────────
Contract management
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  add new contract to contractsList
 * @param  studentId    : id of student associated with the contract
 * @param  roomId       : id of room associated with the contract
 * @param  startDate    : start date of the contract
 * @param  endDate      : end date of the contract
 */
void addContract(const std::string& studentId, const std::string& roomId,
                 const base::Date& startDate, const base::Date& endDate);

/**
 * @brief  extend end date of a contract
 * @param  contractId: id of contract to extend
 * @param  newEndDate: new end date of the contract
 */
void extendContract(const std::string& contractId,
                    const base::Date&  newEndDate);

/**
 * @brief  terminate a contract
 * @param  contractId: id of contract to terminate
 */
void terminateContract(const std::string& contractId);

/*
────────────────────────────────────────────────────────────────────────────────
Room assignment management
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  assign student to a room
 * @param  studentId    : id of student to assign
 * @param  roomId       : id of room to assign student to
 * @param  startDate    : start date of the assignment
 * @param  endDate      : end date of the assignment
 */
void addStudentToRoom(const std::string& roomId, const std::string& studentId);

/**
 * @brief  transfer student to a different room
 * @param  studentId    : id of student to transfer
 * @param  newRoomId    : id of new room to transfer student to
 * @param  newStartDate : start date of the new contract
 * @param  newEndDate   : end date of the new contract
 */
void removeStudentFromRoom(const std::string& roomId,
                           const std::string& studentId);

/**
 * @brief  checkout student from a room
 * @param  studentId: id of student to checkout
 */
void transferRoom(const std::string& studentId, const std::string& newRoomId,
                  const base::Date& newStartDate, const base::Date& newEndDate);
