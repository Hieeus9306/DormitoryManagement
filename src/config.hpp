#pragma once

#include "libs/date.hpp"
#include "libs/vector.hpp"
#include <string>
#include <string_view>

/*
────────────────────────────────────────────────────────────────────────────────
Data objects
────────────────────────────────────────────────────────────────────────────────
*/
struct Student {
    std::string id;
    std::string name;
    std::string studentClass;
    bool        isPriority;
    std::string phone;
    std::string email;
};

struct Room {
    std::string               id;
    size_t                    type;
    double                    price;
    base::Vector<std::string> students;

    constexpr bool hasAvailableSlot() const { return students.size() < type; }

    constexpr size_t currentStudents() const { return students.size(); }
};

struct Contract {
    std::string id;
    std::string studentId;
    std::string roomId;
    base::Date  startDate;
    base::Date  endDate;
    bool        isActive;
};

struct ServiceInvoice {
    std::string id;
    std::string roomId;
    size_t      month;
    size_t      year;
    double      oldElectricityIndex;
    double      newElectricityIndex;
    double      oldWaterIndex;
    double      newWaterIndex;
    double      totalAmount;
    bool        isPaid;
};

/*
────────────────────────────────────────────────────────────────────────────────
Data lists
────────────────────────────────────────────────────────────────────────────────
*/
base::Vector<Student>        studentsList;
base::Vector<Room>           roomsList;
base::Vector<Contract>       contractsList;
base::Vector<ServiceInvoice> serviceInvoicesList;

/*
────────────────────────────────────────────────────────────────────────────────
Data files
────────────────────────────────────────────────────────────────────────────────
*/
#ifndef DATA_DIR_PATH
#define DATA_DIR_PATH "../data"
#endif

constexpr std::string_view STUDENTS_FILES =
    DATA_DIR_PATH "/Students.txt";
constexpr std::string_view ROOMS_FILE =
    DATA_DIR_PATH "/Rooms.txt";
constexpr std::string_view CONTRACTS_FILE =
    DATA_DIR_PATH "/Contracts.txt";
constexpr std::string_view BILLS_FILE =
    DATA_DIR_PATH "/ServiceInvoices.txt";

/*
────────────────────────────────────────────────────────────────────────────────
Service rates
────────────────────────────────────────────────────────────────────────────────
*/
constexpr size_t ELECTRICITY_TIER[4] = {50, 100, 150, 200};
constexpr size_t ELECTRICITY_RATE[4] = {2500, 3000, 3600, 4000};
constexpr size_t WATER_RATE          = 10000;
