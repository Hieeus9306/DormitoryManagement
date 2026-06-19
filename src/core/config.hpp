#pragma once

#include <cstddef>
#include <string_view>

/*
────────────────────────────────────────────────────────────────────────────────
Data files
────────────────────────────────────────────────────────────────────────────────
*/
#ifndef DATA_DIR_PATH
#define DATA_DIR_PATH "../data"
#endif

constexpr std::string_view STUDENTS_FILES = DATA_DIR_PATH "/Students.txt";
constexpr std::string_view ROOMS_FILE     = DATA_DIR_PATH "/Rooms.txt";
constexpr std::string_view CONTRACTS_FILE = DATA_DIR_PATH "/Contracts.txt";
constexpr std::string_view BILLS_FILE = DATA_DIR_PATH "/ServiceInvoices.txt";

/*
────────────────────────────────────────────────────────────────────────────────
Service rates
────────────────────────────────────────────────────────────────────────────────
*/
constexpr size_t ELECTRICITY_TIER[4] = {50, 100, 150, 200};
constexpr size_t ELECTRICITY_RATE[4] = {2500, 3000, 3600, 4000};
constexpr size_t WATER_RATE          = 10000;
