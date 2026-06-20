#pragma once

#include "libs/vector.hpp"
#include <string>

/*
────────────────────────────────────────────────────────────────────────────────
Parsing helpers
────────────────────────────────────────────────────────────────────────────────
*/
base::Vector<std::string> parseRecord(std::string& line);
std::string               trimField(std::string value);
bool parseSizeField(const std::string& text, size_t& value);
bool parseDoubleField(const std::string& text, double& value);

/*
────────────────────────────────────────────────────────────────────────────────
Students I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadStudents();
void saveStudents();

/*
────────────────────────────────────────────────────────────────────────────────
Rooms I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadRooms();
void saveRooms();

/*
────────────────────────────────────────────────────────────────────────────────
Contracts I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadContracts();
void saveContracts();

/*
────────────────────────────────────────────────────────────────────────────────
Service Invoices I/O
────────────────────────────────────────────────────────────────────────────────
*/
void loadServiceInvoices();
void saveServiceInvoices();
