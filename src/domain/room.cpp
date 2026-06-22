#include "domain/room.hpp"

#include "io/file_io.hpp"
#include "libs/algorithms.hpp"
#include "state/state.hpp"

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

/*
────────────────────────────────────────────────────────────────────────────────
Room management
────────────────────────────────────────────────────────────────────────────────
*/
void addRoom(const std::string& roomId, size_t roomType, double roomPrice) {
    size_t idx = findRoom(roomId);
    if (idx != roomsList.size() && roomsList[idx].id == roomId) {
        return;
    }
    if (roomType != 4 && roomType != 6 && roomType != 8) {
        return;
    }
    if (roomPrice <= 0) {
        return;
    }

    Room newRoom;
    newRoom.id    = roomId;
    newRoom.type  = roomType;
    newRoom.price = roomPrice;

    roomsList.insert(idx, newRoom);
    saveRooms();
}

void removeRoom(const std::string& roomId) {
    size_t idx;
    for (idx = 0; idx < roomsList.size(); idx++) {
        if (roomsList[idx].id == roomId)
            break;
    }
    if (idx == roomsList.size() || roomsList[idx].id != roomId) {
        return;
    }
    if (!roomsList[idx].students.empty()) {
        return;
    }

    roomsList.erase_at(idx);
    saveRooms();
}

void updateRoom(const std::string& roomId, size_t newType, double newPrice) {
    size_t idx = findRoom(roomId);

    if (idx == roomsList.size() || roomsList[idx].id != roomId) {
        return;
    }
    if (newType != 4 && newType != 6 && newType != 8) {
        return;
    }
    if (roomsList[idx].currentStudents() > newType) {
        return;
    }
    if (newPrice <= 0) {
        return;
    }

    roomsList[idx].type  = newType;
    roomsList[idx].price = newPrice;
    saveRooms();
}

void addStudentToRoom(const std::string& roomId, const std::string& studentId) {
    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        return;
    }
    if (!roomsList[roomIdx].hasAvailableSlot()) {
        return;
    }
    auto& room = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);
    if (studentIdx != room.students.size() &&
        room.students[studentIdx] == studentId) {
        return;
    }

    room.students.insert(studentIdx, studentId);
    saveRooms();
}

void removeStudentFromRoom(const std::string& roomId,
                           const std::string& studentId) {
    size_t roomIdx = findRoom(roomId);
    if (roomIdx == roomsList.size() || roomsList[roomIdx].id != roomId) {
        return;
    }
    auto& room = roomsList[roomIdx];

    size_t studentIdx = findStudentInRoom(room, studentId);
    if (studentIdx == room.students.size() ||
        room.students[studentIdx] != studentId) {
        return;
    }

    room.students.erase_at(studentIdx);
    saveRooms();
}
