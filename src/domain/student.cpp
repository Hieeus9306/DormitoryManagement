#include "domain/student.hpp"

#include "libs/algorithms.hpp"
#include "state/state.hpp"

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
Student management
────────────────────────────────────────────────────────────────────────────────
*/
void addStudent(const Student& newStudent) {
    auto idx = findStudent(newStudent.id);

    if (idx != studentsList.size() && studentsList[idx].id == newStudent.id) {
        return;
    }

    studentsList.insert(idx, newStudent);
}

void removeStudent(const std::string& studentId) {
    size_t idx = findStudent(studentId);

    if (idx == studentsList.size() || studentsList[idx].id != studentId) {
        return;
    }

    studentsList.erase_at(idx);
}

void updateStudent(const std::string& studentId, std::string newName,
                   std::string newClass, bool newIsPriority,
                   std::string newPhone, std::string newEmail) {
    size_t idx = findStudent(studentId);

    if (idx == studentsList.size() || studentsList[idx].id != studentId) {
        return;
    }

    Student& student     = studentsList[idx];
    student.name         = newName;
    student.studentClass = newClass;
    student.isPriority   = newIsPriority;
    student.phone        = newPhone;
    student.email        = newEmail;
}
