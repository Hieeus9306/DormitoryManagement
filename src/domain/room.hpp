#pragma once

#include "core/types.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Room management
────────────────────────────────────────────────────────────────────────────────
*/

/// @brief  find room by roomId
/// @param  roomId: id of room to find
/// @return lower bound index of room in roomsList
size_t findRoom(const std::string& roomId);

/// @brief  find student in a specific room
/// @param  room       : the room to search in
/// @param  studentId  : id of student to find
/// @return lower bound index of studentId in room.students
size_t findStudentInRoom(const Room& room, const std::string& studentId);

/// @brief  add room to the roomsList
/// @param  roomId   : id of the room to add
/// @param  roomType : type of the room to add
/// @param  roomPrice: price of the room to add
void addRoom(const std::string& roomId, size_t roomType, double roomPrice);

/// @brief  remove room from the roomsList
/// @param  roomId: id of the room to remove
void removeRoom(const std::string& roomId);

/// @brief  update room information
/// @param  roomId  : id of the room to update
/// @param  newType : new type of the room
/// @param  newPrice: new price of the room
void updateRoom(const std::string& roomId, size_t newType, double newPrice);

/// @brief  add student to a specific room
/// @param  roomId   : id of the room to add student to
/// @param  studentId: id of the student to add
void addStudentToRoom(const std::string& roomId, const std::string& studentId);

/// @brief  remove student from a specific room
/// @param  roomId   : id of the room to remove student from
/// @param  studentId: id of the student to remove
void removeStudentFromRoom(const std::string& roomId,
                           const std::string& studentId);
