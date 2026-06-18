#pragma once

#include "config.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Find student
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  find student by studentId
 * @param  studentId: id of student to find
 * @return lower bound index of student in studentsList
 */
size_t findStudent(const std::string& studentId);

/*
────────────────────────────────────────────────────────────────────────────────
Check logic for student management
────────────────────────────────────────────────────────────────────────────────
*/
/// @brief  check if a student exists in the studentsList
bool isStudentExist(const std::string& studentId);

/// @brief  check if a student can be added
bool canAddStudent(const Student& newStudent, std::string& message);

/// @brief  check if student can be updated
bool canUpdateStudent(const std::string& studentId, std::string& message);

/// @brief  check if student can be removed
bool canRemoveStudent(const std::string& studentId, std::string& message);

/*
────────────────────────────────────────────────────────────────────────────────
Student management
────────────────────────────────────────────────────────────────────────────────
*/
/**
 * @brief  add student to the studentsList
 * @param  newStudent: student to add
 */
void addStudent(const Student& newStudent);

/**
 * @brief  remove student from the studentsList
 * @param  studentId: id of student to remove
 */
void removeStudent(const std::string& studentId);

/**
 * @brief  update student information
 * @param  studentId    : id of student to update
 * @param  newClass     : new class of student
 * @param  newIsPriority: new priority status of student
 * @param  newPhone     : new phone number of student
 * @param  newEmail     : new email of student
 */
void updateStudent(const std::string& studentId, std::string newClass,
                   bool newIsPriority, std::string newPhone,
                   std::string newEmail);