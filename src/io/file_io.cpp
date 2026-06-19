#include "io/file_io.hpp"

#include "core/config.hpp"
#include "state/state.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Convert file line into record
────────────────────────────────────────────────────────────────────────────────
*/
base::Vector<std::string> parseRecord(std::string& line) {
    std::stringstream ss(line);
    std::string       token;

    base::Vector<std::string> record;

    while (std::getline(ss, token, ',')) {
        record.push_back(token);
    }

    return record;
}

std::string trimField(std::string value) {
    auto isSpace = [](unsigned char character) {
        return std::isspace(character) != 0;
    };

    value.erase(value.begin(), std::find_if(value.begin(), value.end(),
                                            [&](unsigned char character) {
                                                return !isSpace(character);
                                            }));
    value.erase(std::find_if(value.rbegin(), value.rend(),
                             [&](unsigned char character) {
                                 return !isSpace(character);
                             })
                    .base(),
                value.end());

    return value;
}

bool parseSizeField(const std::string& text, size_t& value) {
    try {
        std::string input = trimField(text);
        if (input.empty()) {
            return false;
        }

        size_t parsed = 0;
        value         = static_cast<size_t>(std::stoull(input, &parsed));
        return parsed == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

bool parseDoubleField(const std::string& text, double& value) {
    try {
        std::string input = trimField(text);
        if (input.empty()) {
            return false;
        }

        size_t parsed = 0;
        value         = std::stod(input, &parsed);
        return parsed == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

/*
────────────────────────────────────────────────────────────────────────────────
Students I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadStudents() {
    std::ifstream fin{std::string(STUDENTS_FILES)};

    std::string line;
    while (std::getline(fin, line)) {
        if (trimField(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 6) {
            continue;
        }

        Student student;

        student.id           = trimField(record[0]);
        student.name         = trimField(record[1]);
        student.studentClass = trimField(record[2]);
        student.isPriority   = (trimField(record[3]) == "true");
        student.phone        = trimField(record[4]);
        student.email        = trimField(record[5]);

        studentsList.push_back(student);
    }

    fin.close();
}

void saveStudents() {
    std::ofstream fout{std::string(STUDENTS_FILES)};

    bool isFirstline = true;
    for (const auto& student : studentsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << student.id << "," << student.name << "," << student.studentClass
             << "," << (student.isPriority ? "true" : "false") << ","
             << student.phone << "," << student.email;
    }

    fout.flush();
    fout.close();
}

/*
────────────────────────────────────────────────────────────────────────────────
Rooms I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadRooms() {
    std::ifstream fin{std::string(ROOMS_FILE)};

    std::string line;
    while (std::getline(fin, line)) {
        if (trimField(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 4) {
            continue;
        }

        size_t roomType  = 0;
        double roomPrice = 0;
        if (!parseSizeField(record[1], roomType) ||
            !parseDoubleField(record[2], roomPrice)) {
            continue;
        }

        record[3] = trimField(record[3]);
        if (record[3] == "empty") {
            record[3].clear();
        }

        Room room;

        room.id    = trimField(record[0]);
        room.type  = roomType;
        room.price = roomPrice;

        std::stringstream ss(record[3]);
        std::string       token;
        while (std::getline(ss, token, '|')) {
            token = trimField(token);
            if (!token.empty()) {
                room.students.push_back(token);
            }
        }

        roomsList.push_back(room);
    }

    fin.close();
}

void saveRooms() {
    std::ofstream fout{std::string(ROOMS_FILE)};

    bool isFirstline = true;
    for (const auto& room : roomsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << room.id << "," << room.type << "," << room.price << ",";
        if (room.students.empty()) {
            fout << "empty";
        } else {
            for (size_t i = 0; i < room.students.size(); ++i) {
                fout << room.students[i];
                if (i < room.students.size() - 1) {
                    fout << "|";
                }
            }
        }
    }

    fout.flush();
    fout.close();
}

/*
────────────────────────────────────────────────────────────────────────────────
Contracts I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadContracts() {
    std::ifstream fin{std::string(CONTRACTS_FILE)};

    std::string line;
    while (std::getline(fin, line)) {
        if (trimField(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 6) {
            continue;
        }

        Contract contract;

        try {
            contract.id        = trimField(record[0]);
            contract.studentId = trimField(record[1]);
            contract.roomId    = trimField(record[2]);
            contract.startDate = base::Date(trimField(record[3]));
            contract.endDate   = base::Date(trimField(record[4]));
            contract.isActive  = (trimField(record[5]) == "true");
        } catch (const std::exception&) {
            continue;
        }

        contractsList.push_back(contract);
    }

    fin.close();
}

void saveContracts() {
    std::ofstream fout{std::string(CONTRACTS_FILE)};

    bool isFirstline = true;
    for (const auto& contract : contractsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << contract.id << "," << contract.studentId << ","
             << contract.roomId << "," << contract.startDate << ","
             << contract.endDate << ","
             << (contract.isActive ? "true" : "false");
    }

    fout.flush();
    fout.close();
}

/*
────────────────────────────────────────────────────────────────────────────────
Service Invoices I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadServiceInvoices() {
    std::ifstream fin{std::string(BILLS_FILE)};

    std::string line;
    while (std::getline(fin, line)) {
        if (trimField(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 10) {
            continue;
        }

        size_t month               = 0;
        size_t year                = 0;
        double oldElectricityIndex = 0;
        double newElectricityIndex = 0;
        double oldWaterIndex       = 0;
        double newWaterIndex       = 0;
        double totalAmount         = 0;
        if (!parseSizeField(record[2], month) ||
            !parseSizeField(record[3], year) ||
            !parseDoubleField(record[4], oldElectricityIndex) ||
            !parseDoubleField(record[5], newElectricityIndex) ||
            !parseDoubleField(record[6], oldWaterIndex) ||
            !parseDoubleField(record[7], newWaterIndex) ||
            !parseDoubleField(record[8], totalAmount)) {
            continue;
        }

        ServiceInvoice invoice;

        invoice.id                  = trimField(record[0]);
        invoice.roomId              = trimField(record[1]);
        invoice.month               = month;
        invoice.year                = year;
        invoice.oldElectricityIndex = oldElectricityIndex;
        invoice.newElectricityIndex = newElectricityIndex;
        invoice.oldWaterIndex       = oldWaterIndex;
        invoice.newWaterIndex       = newWaterIndex;
        invoice.totalAmount         = totalAmount;
        invoice.isPaid              = (trimField(record[9]) == "true");

        serviceInvoicesList.push_back(invoice);
    }

    fin.close();
}

void saveServiceInvoices() {
    std::ofstream fout{std::string(BILLS_FILE)};

    bool isFirstline = true;
    for (const auto& invoice : serviceInvoicesList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << invoice.id << "," << invoice.roomId << "," << invoice.month
             << "," << invoice.year << "," << invoice.oldElectricityIndex << ","
             << invoice.newElectricityIndex << "," << invoice.oldWaterIndex
             << "," << invoice.newWaterIndex << "," << invoice.totalAmount
             << "," << (invoice.isPaid ? "true" : "false");
    }

    fout.flush();
    fout.close();
}
