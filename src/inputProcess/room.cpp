#include "room.hpp"
#include "config.hpp"
#include "libs/algorithms.hpp"

/*
────────────────────────────────────────────────────────────────────────────────
Find room
────────────────────────────────────────────────────────────────────────────────
*/
size_t findRoom(const std::string& roomId) {
    auto it = binarySearch(
        roomsList, roomId,
        [](const Room& room, const std::string& id) { return room.id < id; });

    return it - roomsList.begin();
}

size_t findStudentInRoom(const Room& room, const std::string& studentId) {
    auto it = binarySearch(room.students, studentId);

    return it - room.students.begin();
}

size_t findRoomHasStudent(const std::string& studentId) {
    for (auto& room : roomsList) {
        size_t studentIdx = findStudentInRoom(room, studentId);
        if (studentIdx != room.students.size() &&
            room.students[studentIdx] == studentId) {
            return &room - roomsList.begin();
        }
    }
    return roomsList.size();
}

/*
────────────────────────────────────────────────────────────────────────────────
Check logic for room management
────────────────────────────────────────────────────────────────────────────────
*/
bool isRoomExist(const std::string& roomId) {
    size_t idx = findRoom(roomId);
    return (idx != roomsList.size() && roomsList[idx].id == roomId);
}

bool isValidRoomType(size_t roomType) {
    return (roomType == 4 || roomType == 6 || roomType == 8);
}

bool isValidRoomPrice(double roomPrice) { return (roomPrice > 0); }

bool canAddRoom(const std::string& roomId, size_t roomType, double roomPrice,
                std::string& message) {
    if (isRoomExist(roomId)) {
        message = "Room already exists";
        return false;
    }
    if (!isValidRoomType(roomType)) {
        message = "Invalid room type";
        return false;
    }
    if (!isValidRoomPrice(roomPrice)) {
        message = "Invalid room price";
        return false;
    }
    return true;
}

bool canRemoveRoom(const std::string& roomId, std::string& message) {
    size_t idx = findRoom(roomId);
    if (idx == roomsList.size() || roomsList[idx].id != roomId) {
        message = "Room does not exist";
        return false;
    }
    if (!roomsList[idx].students.empty()) {
        message = "Room is not empty";
        return false;
    }
    return true;
}

bool canUpdateRoom(const std::string& roomId, size_t newType, double newPrice,
                   std::string& message) {
    size_t idx = findRoom(roomId);
    if (idx == roomsList.size() || roomsList[idx].id != roomId) {
        message = "Room does not exist";
        return false;
    }
    if (!isValidRoomType(newType)) {
        message = "Invalid room type";
        return false;
    }
    if (roomsList[idx].currentStudents() > newType) {
        message = "New room type is less than current students";
        return false;
    }
    if (!isValidRoomPrice(newPrice)) {
        message = "Invalid room price";
        return false;
    }
    return true;
}

bool canAddStudentToRoom(const std::string& roomId,
                         const std::string& studentId, std::string& message) {
    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        message = "Room does not exist";
        return false;
    }
    if (!roomsList[roomIdx].hasAvailableSlot()) {
        message = "Room is full";
        return false;
    }
    auto& room = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);
    if (studentIdx != room.students.size() &&
        room.students[studentIdx] == studentId) {
        message = "Student already in room";
        return false;
    }
    return true;
}

bool canRemoveStudentFromRoom(const std::string& roomId,
                              const std::string& studentId,
                              std::string&       message) {
    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        message = "Room does not exist";
        return false;
    }
    auto& room = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);
    if (studentIdx == room.students.size() ||
        room.students[studentIdx] != studentId) {
        message = "Student is not in room";
        return false;
    }
    return true;
}
/*
────────────────────────────────────────────────────────────────────────────────
Room management
────────────────────────────────────────────────────────────────────────────────
*/
void addRoom(const std::string& roomId, size_t roomType, double roomPrice) {
    size_t idx = findRoom(roomId);

    Room newRoom;
    newRoom.id    = roomId;
    newRoom.type  = roomType;
    newRoom.price = roomPrice;

    roomsList.insert(idx, newRoom);
}

void removeRoom(const std::string& roomId) {
    size_t idx = findRoom(roomId);

    roomsList.erase_at(idx);
}

void updateRoom(const std::string& roomId, size_t newType, double newPrice) {
    size_t idx = findRoom(roomId);

    roomsList[idx].type  = newType;
    roomsList[idx].price = newPrice;
}

/*
────────────────────────────────────────────────────────────────────────────────
Student management in room
────────────────────────────────────────────────────────────────────────────────
*/
void addStudentToRoom(const std::string& roomId, const std::string& studentId) {
    size_t roomIdx = findRoom(roomId);
    auto&  room    = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);

    room.students.insert(studentIdx, studentId);
}

void removeStudentFromRoom(const std::string& roomId,
                           const std::string& studentId) {
    size_t roomIdx = findRoom(roomId);
    auto&  room    = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);

    room.students.erase_at(studentIdx);
}
