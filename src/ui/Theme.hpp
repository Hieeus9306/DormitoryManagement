#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

namespace ui::theme {

// ═══════════════════════════════════════════════════════════════
//  Color palette — modern dark dashboard
// ═══════════════════════════════════════════════════════════════

/// Deep navy sidebar
inline const ftxui::Color sidebar = ftxui::Color::RGB(24, 31, 42);
/// Main content background
inline const ftxui::Color panel = ftxui::Color::RGB(34, 43, 56);
/// Top bar / header background
inline const ftxui::Color panelDark = ftxui::Color::RGB(28, 35, 48);
/// Card / form background
inline const ftxui::Color panelLight = ftxui::Color::RGB(45, 56, 72);
/// Zebra stripe highlight
inline const ftxui::Color stripe = ftxui::Color::RGB(39, 50, 64);
/// Accent teal
inline const ftxui::Color accent = ftxui::Color::RGB(62, 207, 142);
/// Accent dim (for hover bg)
inline const ftxui::Color accentDim = ftxui::Color::RGB(30, 80, 60);
/// Dim text
inline const ftxui::Color mutedText = ftxui::Color::RGB(151, 163, 182);
/// Border lines
inline const ftxui::Color borderColor = ftxui::Color::RGB(55, 68, 84);

/// Semantic
inline const ftxui::Color success = ftxui::Color::RGB(62, 207, 142);
inline const ftxui::Color danger  = ftxui::Color::RGB(255, 85, 85);
inline const ftxui::Color warning = ftxui::Color::RGB(255, 200, 50);
inline const ftxui::Color info    = ftxui::Color::RGB(80, 160, 255);

// ═══════════════════════════════════════════════════════════════
//  Spacing
// ═══════════════════════════════════════════════════════════════

inline constexpr int sidebarWidth = 30;
inline constexpr int labelWidth   = 20;
inline constexpr int rowHeight    = 1;

// ═══════════════════════════════════════════════════════════════
//  Element helpers
// ═══════════════════════════════════════════════════════════════

// ── Box / border ──

/// Rounded-looking card with background
inline ftxui::Element card(ftxui::Element content) {
    return content | ftxui::border | ftxui::bgcolor(panelLight) |
           ftxui::color(ftxui::Color::White);
}

/// Card with accent top border accent line
inline ftxui::Element accentCard(ftxui::Element content) {
    return ftxui::vbox({
               ftxui::separator() | ftxui::color(accent),
               content,
           }) |
           ftxui::border | ftxui::bgcolor(panelLight);
}

// ── Typography ──

/// Section title with accent color
inline ftxui::Element sectionTitle(const std::string& text) {
    return ftxui::text(text) | ftxui::bold | ftxui::color(accent);
}

/// Muted label (for form fields)
inline ftxui::Element label(const std::string& text) {
    return ftxui::text(text) |
           ftxui::size(ftxui::WIDTH, ftxui::EQUAL, labelWidth) |
           ftxui::color(mutedText);
}

/// Small pill label for form fields
inline ftxui::Element pillLabel(const std::string& label) {
    return ftxui::text(" " + label + " ") | ftxui::bold | ftxui::color(accent);
}

/// Label + value detail row
inline ftxui::Element detailLine(const std::string& field,
                                 const std::string& value) {
    return ftxui::hbox({
               label(field),
               ftxui::text(value) | ftxui::flex |
                   ftxui::color(ftxui::Color::White),
           }) |
           ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

// ── Status badges ──

/// Colored pill-shaped status badge
inline ftxui::Element pillBadge(const std::string& text, ftxui::Color bg,
                                ftxui::Color fg = ftxui::Color::White) {
    return ftxui::text(" " + text + " ") | ftxui::bgcolor(bg) |
           ftxui::color(fg) | ftxui::bold;
}

/// Boolean status: active/success vs inactive/danger
inline ftxui::Element statusBadge(const std::string& text, bool active) {
    if (active) {
        return pillBadge(text, accentDim, accent);
    }
    return pillBadge(text, ftxui::Color::RGB(80, 30, 30), danger);
}

/// Paid / Unpaid badge
inline ftxui::Element paidBadge(bool isPaid) {
    if (isPaid) {
        return pillBadge(" PAID ", ftxui::Color::RGB(20, 70, 40), success);
    }
    return pillBadge(" UNPAID ", ftxui::Color::RGB(70, 20, 20), danger);
}

/// Available / Full badge for rooms
inline ftxui::Element availabilityBadge(bool available) {
    if (available) {
        return pillBadge(" Available ", accentDim, accent);
    }
    return pillBadge(" Full ", ftxui::Color::RGB(70, 30, 30), warning);
}

// ── Tables ──

/// Header row — accent bold columns with bottom line
inline ftxui::Element headerRow(const std::vector<std::string>& cells) {
    ftxui::Elements columns;
    for (const auto& cell : cells) {
        columns.push_back(ftxui::text(cell) | ftxui::bold |
                          ftxui::color(accent) | ftxui::flex);
        columns.push_back(ftxui::separator() | ftxui::color(borderColor));
    }
    if (!columns.empty())
        columns.pop_back();
    return ftxui::hbox(std::move(columns)) |
           ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

/// Single data cell (white text, expands to fill)
inline ftxui::Element dataCell(const std::string& text) {
    return ftxui::text(text) | ftxui::color(ftxui::Color::White) | ftxui::flex;
}

/// Data row (white text)
inline ftxui::Element dataRow(const std::vector<std::string>& cells) {
    ftxui::Elements columns;
    for (const auto& cell : cells) {
        columns.push_back(ftxui::text(cell) | ftxui::flex |
                          ftxui::color(ftxui::Color::White));
        columns.push_back(ftxui::separator() | ftxui::color(borderColor));
    }
    if (!columns.empty())
        columns.pop_back();
    return ftxui::hbox(std::move(columns)) |
           ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

/// Zebra-striped data row (alternating background)
inline ftxui::Element zebraRow(const std::vector<std::string>& cells,
                               size_t                          index) {
    auto row = dataRow(cells);
    if (index % 2 == 1) {
        row = row | ftxui::bgcolor(stripe);
    }
    return row;
}

// ── Visual indicators ──

/// Horizontal progress bar (e.g. for occupancy)
/// @param fraction 0.0 – 1.0
inline ftxui::Element progressBar(double fraction, int width = 15) {
    const int filled = static_cast<int>(fraction * width);
    const int empty  = width - filled;
    auto bar = ftxui::text(std::string(filled, '|') + std::string(empty, '.'));
    ftxui::Color barColor;
    if (fraction > 0.8)
        barColor = danger;
    else if (fraction > 0.5)
        barColor = warning;
    else
        barColor = success;
    return bar | ftxui::color(barColor);
}

// ── Empty / subtle states ──

/// Empty state message with dimmed styling
inline ftxui::Element emptyState(const std::string& msg) {
    return ftxui::text(" " + msg + " ") | ftxui::color(mutedText) |
           ftxui::center | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3);
}

/// Help text at the bottom of panels
inline ftxui::Element helpText(const std::string& text) {
    return ftxui::text("  " + text) | ftxui::color(mutedText) |
           ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

// ── Page navigation ──

/// Page X / Y indicator centered
inline ftxui::Element pageIndicator(size_t current, size_t total) {
    auto text = ftxui::text(" Page " + std::to_string(current + 1) + " / " +
                            std::to_string(std::max<size_t>(total, 1)) + " ");
    return text | ftxui::color(mutedText) | ftxui::center | ftxui::flex;
}

// ── Report block ──

/// A report section with title and content
inline ftxui::Element reportBlock(const std::string& title,
                                  ftxui::Elements    rows) {
    ftxui::Elements content = {
        sectionTitle(title),
        ftxui::separator() | ftxui::color(borderColor),
    };
    for (auto& row : rows) {
        content.push_back(std::move(row));
    }
    return ftxui::vbox(std::move(content)) | ftxui::border |
           ftxui::bgcolor(panelLight);
}

// ── Dividers ──

/// Thin horizontal divider
inline ftxui::Element divider() {
    return ftxui::separator() | ftxui::color(borderColor);
}

/// Empty vertical space
inline ftxui::Element spacer(int height = 1) {
    return ftxui::emptyElement() |
           ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height);
}

} // namespace ui::theme
