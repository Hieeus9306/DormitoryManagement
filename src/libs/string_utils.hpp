#pragma once

#include "libs/date.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

// convert string into lower case
inline std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

// delete blank space 
inline std::string trim(std::string value) {
    auto isSpace = [](unsigned char ch) { return std::isspace(ch) != 0; };
    value.erase(value.begin(),
                std::find_if(value.begin(), value.end(),
                             [&](unsigned char ch) { return !isSpace(ch); }));
    value.erase(std::find_if(value.rbegin(), value.rend(),
                             [&](unsigned char ch) { return !isSpace(ch); })
                    .base(),
                value.end());
    return value;
}

// convert string into unsigned long long, return false if invalid
inline bool parseSize(const std::string& text, size_t& value) {
    try {
        std::string input = trim(text);
        if (input.empty()) {
            return false;
        }
        size_t pos = 0;
        value      = static_cast<size_t>(std::stoull(input, &pos));
        return pos == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

// convert string into double, return false if invalid
inline bool parseDouble(const std::string& text, double& value) {
    try {
        std::string input = trim(text);
        if (input.empty()) {
            return false;
        }
        size_t pos = 0;
        value      = std::stod(input, &pos);
        return pos == input.size();
    } catch (const std::exception&) {
        return false;
    }
}

// convert string format DD/MM/YYYY into Date, return false if invalid 
inline bool parseDate(const std::string& text, base::Date& date) {
    try {
        std::string input = trim(text);
        if (input.size() != 10) {
            return false;
        }
        date = base::Date(input);
        return date.isValid();
    } catch (const std::exception&) {
        return false;
    }
}

// Normalize string input obligately before checking and processing
inline std::string nonEmpty(const std::string& value) { return trim(value); }

// string format of number
inline std::string fmtNum(double value, int precision = 2) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

