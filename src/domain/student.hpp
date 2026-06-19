#pragma once

#include "core/types.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Student management
────────────────────────────────────────────────────────────────────────────────
*/

/// @brief  find student by studentId
/// @param  studentId: id of student to find
/// @return lower bound index of student in studentsList
size_t findStudent(const std::string& studentId);

/// @brief  add student to the studentsList
/// @param  newStudent: student to add
void addStudent(const Student& newStudent);

/// @brief  remove student from the studentsList
/// @param  studentId: id of student to remove
void removeStudent(const std::string& studentId);

/// @brief  update student information
/// @param  studentId    : id of student to update
/// @param  newName      : new full name of student
/// @param  newClass     : new class of student
/// @param  newIsPriority: new priority status of student
/// @param  newPhone     : new phone number of student
/// @param  newEmail     : new email of student
void updateStudent(const std::string& studentId, std::string newName,
                   std::string newClass, bool newIsPriority,
                   std::string newPhone, std::string newEmail);
