#pragma once

#include <ftxui/screen/color.hpp>

namespace ui {

enum class Section {
    Students,
    Rooms,
    Contracts,
    Invoices,
};

namespace theme {

inline const ftxui::Color sidebar = ftxui::Color::RGB(24, 31, 42);
inline const ftxui::Color panel = ftxui::Color::RGB(34, 43, 56);
inline const ftxui::Color panelLight = ftxui::Color::RGB(45, 56, 72);
inline const ftxui::Color accent = ftxui::Color::RGB(62, 207, 142);
inline const ftxui::Color mutedText = ftxui::Color::RGB(151, 163, 182);

}  // namespace theme

int run();

}  // namespace ui
