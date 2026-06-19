#pragma once

#include "core/types.hpp"
#include <string>
/*
────────────────────────────────────────────────────────────────────────────────
Find room
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  find room by roomId
 * @param  roomId: id of room to find
 * @return lower bound index of room in roomsList
 */
size_t findRoom(const std::string& roomId);

/**
 * @brief  find student in a specific room
 * @param  room       : the room to search in
 * @param  studentId  : id of student to find
 * @return lower bound index of studentId in room.students
 */
size_t findStudentInRoom(const Room& room, const std::string& studentId);

/**
 * @brief  find room that has a specific student
 * @param  studentId: id of student to find
 * @return index of room if found, roomsList.size() otherwise
 */
size_t findRoomHasStudent(const std::string& studentId);

/*
────────────────────────────────────────────────────────────────────────────────
Check logic for room management
────────────────────────────────────────────────────────────────────────────────
*/
/// @brief  check if a room exists
bool isRoomExist(const std::string& roomId);

/// @brief  check if a room type is valid
bool isValidRoomType(size_t roomType);

/// @brief  check if a room price is valid
bool isValidRoomPrice(double roomPrice);

/// @brief  check if a room can be added
bool canAddRoom(const std::string& roomId, size_t roomType, double roomPrice,
                std::string& message);

/// @brief  check if a room can be removed
bool canRemoveRoom(const std::string& roomId, std::string& message);

/// @brief  check if a room can be updated
bool canUpdateRoom(const std::string& roomId, size_t newType, double newPrice,

                   std::string& message);
/// @brief  check if a student can be added to a room
bool canAddStudentToRoom(const std::string& roomId,
                         const std::string& studentId, std::string& message);

/// @brief  check if a student can be removed from a room
bool canRemoveStudentFromRoom(const std::string& roomId,
                              const std::string& studentId,
                              std::string&       message);

/*
────────────────────────────────────────────────────────────────────────────────
Room management
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  add room to the roomsList
 * @param  roomId   : id of the room to add
 * @param  roomType : type of the room to add
 * @param  roomPrice: price of the room to add
 */
void addRoom(const std::string& roomId, size_t roomType, double roomPrice);

/**
 * @brief  remove room from the roomsList
 * @param  roomId: id of the room to remove
 */
void removeRoom(const std::string& roomId);

/**
 * @brief  update room information
 * @param  roomId  : id of the room to update
 * @param  newType : new type of the room
 * @param  newPrice: new price of the room
 */
void updateRoom(const std::string& roomId, size_t newType, double newPrice);

/*
────────────────────────────────────────────────────────────────────────────────
Student management in room
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  add student to a specific room
 * @param  roomId   : id of the room to add student to
 * @param  studentId: id of the student to add
 */
void addStudentToRoom(const std::string& roomId, const std::string& studentId);

/**
 * @brief  remove student from a specific room
 * @param  roomId   : id of the room to remove student from
 * @param  studentId: id of the student to remove
 */
void removeStudentFromRoom(const std::string& roomId,
                           const std::string& studentId);