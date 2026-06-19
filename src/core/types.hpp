#pragma once

#include "libs/date.hpp"
#include "libs/vector.hpp"
#include <string>

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

    [[nodiscard]] constexpr bool hasAvailableSlot() const {
        return students.size() < type;
    }

    [[nodiscard]] constexpr size_t currentStudents() const {
        return students.size();
    }
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
