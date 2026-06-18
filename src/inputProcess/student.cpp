#include "student.hpp"

#include "config.hpp"
#include "libs/algorithms.hpp"

#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Find student
────────────────────────────────────────────────────────────────────────────────
*/
size_t findStudent(const std::string& studentId) {
    auto it = binarySearch(studentsList, studentId,
                           [](const Student& student, const std::string& id) {
                               return student.id < id;
                           });

    return it - studentsList.begin();
}

/*
────────────────────────────────────────────────────────────────────────────────
Check logic for student management
────────────────────────────────────────────────────────────────────────────────
*/
bool isStudentExist(const std::string& studentId) {
    size_t idx = findStudent(studentId);
    return (idx != studentsList.size()) && (studentsList[idx].id == studentId);
}

bool canAddStudent(const Student& newStudent, std::string& message) {
    if (isStudentExist(newStudent.id)) {
        message = "Student already exists";
        return false;
    }
    return true;
}

bool canUpdateStudent(const std::string& studentId, std::string& message) {
    if (!isStudentExist(studentId)) {
        message = "Student does not exist";
        return false;
    }
    return true;
}

bool canRemoveStudent(const std::string& studentId, std::string& message) {
    if (!isStudentExist(studentId)) {
        message = "Student does not exist";
        return false;
    }
    return true;
}

/*
────────────────────────────────────────────────────────────────────────────────
Student management
────────────────────────────────────────────────────────────────────────────────
*/
void addStudent(const Student& newStudent) {
    auto idx = findStudent(newStudent.id);

    studentsList.insert(idx, newStudent);
}

void removeStudent(const std::string& studentId) {
    size_t idx = findStudent(studentId);

    studentsList.erase_at(idx);
}

void updateStudent(const std::string& studentId, std::string newClass,
                   bool newIsPriority, std::string newPhone,
                   std::string newEmail) {
    size_t idx = findStudent(studentId);

    Student& student     = studentsList[idx];
    student.studentClass = newClass;
    student.isPriority   = newIsPriority;
    student.phone        = newPhone;
    student.email        = newEmail;
}
