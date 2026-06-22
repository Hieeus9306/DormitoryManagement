#include "io/file_io.hpp"

#include "core/config.hpp"
#include "state/state.hpp"

#include "libs/string_utils.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

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

/*
────────────────────────────────────────────────────────────────────────────────
Students I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadStudents() {
    std::ifstream fin{std::string(STUDENTS_FILES)};

    if(!fin.is_open()) {
        throw std::runtime_error("Fail to open students file");
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (trim(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 6) {
            continue;
        }

        Student student;

        student.id           = trim(record[0]);
        student.name         = trim(record[1]);
        student.studentClass = trim(record[2]);
        student.isPriority   = (trim(record[3]) == "true");
        student.phone        = trim(record[4]);
        student.email        = trim(record[5]);

        studentsList.push_back(student);
    }

    fin.close();
}

void saveStudents() {
    std::ofstream fout{std::string(STUDENTS_FILES)};
    
    if(!fout.is_open()) {
        std::cout << "Fail to save students data\n";
        return;
    }

    bool isFirstline = true;
    for (const auto& student : studentsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << student.id << ","
             << student.name << ","
             << student.studentClass << "," 
             << (student.isPriority ? "true" : "false") << ","
             << student.phone << "," 
             << student.email;
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

    if(!fin.is_open()) {
        throw std::runtime_error("Fail to open rooms file");
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (trim(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 4) {
            continue;
        }

        size_t roomType  = 0;
        double roomPrice = 0;
        if (!parseSize(record[1], roomType) ||
            !parseDouble(record[2], roomPrice)) {
            continue;
        }

        record[3] = trim(record[3]);
        if (record[3] == "empty") {
            record[3].clear();
        }

        Room room;

        room.id    = trim(record[0]);
        room.type  = roomType;
        room.price = roomPrice;

        std::stringstream ss(record[3]);
        std::string       token;
        while (std::getline(ss, token, '|')) {
            token = trim(token);
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

    if(!fout.is_open()) {
        std::cout << "Fail to save rooms data\n";
        return;
    }

    bool isFirstline = true;
    for (const auto& room : roomsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << room.id << "," 
             << room.type << "," 
             << room.price << ",";
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

    if(!fin.is_open()) {
        throw std::runtime_error("Fail to open contracts file");
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (trim(line).empty()) {
            continue;
        }

        base::Vector<std::string> record = parseRecord(line);
        if (record.size() < 6) {
            continue;
        }

        Contract contract;

        try {
            contract.id        = trim(record[0]);
            contract.studentId = trim(record[1]);
            contract.roomId    = trim(record[2]);
            contract.startDate = base::Date(trim(record[3]));
            contract.endDate   = base::Date(trim(record[4]));
            contract.isActive  = (trim(record[5]) == "true");
        } catch (const std::exception&) {
            continue;
        }

        contractsList.push_back(contract);
    }

    fin.close();
}

void saveContracts() {
    std::ofstream fout{std::string(CONTRACTS_FILE)};

    if(!fout.is_open()) {
        std::cout << "Fail to save contracts data\n";
        return;
    }

    bool isFirstline = true;
    for (const auto& contract : contractsList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << contract.id << "," 
             << contract.studentId << ","
             << contract.roomId << "," 
             << contract.startDate << ","
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

    if(!fin.is_open()) {
        throw std::runtime_error("Fail to open service invoices file");
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (trim(line).empty()) {
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
        if (!parseSize(record[2], month) ||
            !parseSize(record[3], year) ||
            !parseDouble(record[4], oldElectricityIndex) ||
            !parseDouble(record[5], newElectricityIndex) ||
            !parseDouble(record[6], oldWaterIndex) ||
            !parseDouble(record[7], newWaterIndex) ||
            !parseDouble(record[8], totalAmount)) {
            continue;
        }

        ServiceInvoice invoice;

        invoice.id                  = trim(record[0]);
        invoice.roomId              = trim(record[1]);
        invoice.month               = month;
        invoice.year                = year;
        invoice.oldElectricityIndex = oldElectricityIndex;
        invoice.newElectricityIndex = newElectricityIndex;
        invoice.oldWaterIndex       = oldWaterIndex;
        invoice.newWaterIndex       = newWaterIndex;
        invoice.totalAmount         = totalAmount;
        invoice.isPaid              = (trim(record[9]) == "true");

        serviceInvoicesList.push_back(invoice);
    }

    fin.close();
}

void saveServiceInvoices() {
    std::ofstream fout{std::string(BILLS_FILE)};

    if(!fout.is_open()) {
        std::cout << "Fail to save service invoices data\n";
        return;
    }
    
    bool isFirstline = true;
    for (const auto& invoice : serviceInvoicesList) {
        if (isFirstline) {
            isFirstline = false;
        } else {
            fout << "\n";
        }

        fout << invoice.id << "," 
             << invoice.roomId << "," 
             << invoice.month << "," 
             << invoice.year << "," 
             << invoice.oldElectricityIndex << ","
             << invoice.newElectricityIndex << "," 
             << invoice.oldWaterIndex << "," 
             << invoice.newWaterIndex << "," 
             << invoice.totalAmount << "," 
             << (invoice.isPaid ? "true" : "false");
    }

    fout.flush();
    fout.close();
}
