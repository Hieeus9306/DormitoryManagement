#include "Dashboard.hpp"

#include "Theme.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/terminal.hpp>

namespace ui {
namespace {

using namespace ftxui;

struct DashboardState {
    size_t page           = 0;
    size_t selectedRecord = 0;
    size_t selectedSlot   = 0;
    bool   showDetail     = false;
};

/// Builds a list of visible (filtered) record indices.
std::vector<size_t> buildVisibleIndices(const DashboardConfig& config) {
    std::vector<size_t> visible;
    if (!config.filterFn) {
        visible.reserve(config.totalRecords);
        for (size_t i = 0; i < config.totalRecords; ++i) {
            visible.push_back(i);
        }
        return visible;
    }
    for (size_t i = 0; i < config.totalRecords; ++i) {
        if (config.filterFn(i)) {
            visible.push_back(i);
        }
    }
    return visible;
}

/// Nicely styled nav button.
Component styledButton(const std::string&    label,
                       std::function<void()> onPress) {
    auto opt      = ButtonOption::Ascii();
    opt.transform = [label](const EntryState& s) {
        auto el = ftxui::text(label) | ftxui::bold;
        if (s.focused) {
            el = el | ftxui::bgcolor(theme::accentDim) |
                 ftxui::color(theme::accent);
        } else {
            el = el | ftxui::bgcolor(theme::panelLight) |
                 ftxui::color(theme::mutedText);
        }
        return el | ftxui::size(WIDTH, ftxui::GREATER_THAN, 8) | center;
    };
    return Button(label, std::move(onPress), opt);
}

} // namespace

Component createDashboard(DashboardConfig config) {
    auto state           = std::make_shared<DashboardState>();
    auto visiblePageRows = std::make_shared<std::vector<size_t>>();
    auto totalVisible    = std::make_shared<size_t>(0);

    // Calculate page size dynamically to fill terminal height.
    // Overhead: topBar(3) + searchBar(1) + tableFrame(4) + pageCtrl(1) +
    // help(1) = 10
    const size_t pageSize =
        config.pageSize > 0 ? config.pageSize
                            : std::max<size_t>(3, Terminal::Size().dimy - 10);

    // ── Info buttons (one per page slot) ──
    std::vector<Component> infoButtons;
    if (config.hasDetailView && config.renderDetailFn) {
        for (size_t slot = 0; slot < pageSize; ++slot) {
            auto opt      = ButtonOption::Ascii();
            opt.transform = [](const EntryState& s) {
                auto el = ftxui::text("[i]") | ftxui::bold;
                if (s.focused) {
                    el = el | ftxui::color(theme::accent) |
                         ftxui::bgcolor(theme::accentDim);
                } else {
                    el = el | ftxui::color(theme::mutedText) |
                         ftxui::bgcolor(theme::panelLight);
                }
                return el | ftxui::size(WIDTH, ftxui::EQUAL, 4) | center;
            };
            infoButtons.push_back(Button(
                "i",
                [state, visiblePageRows, slot] {
                    if (slot < visiblePageRows->size()) {
                        state->selectedRecord = (*visiblePageRows)[slot];
                        state->showDetail     = true;
                    }
                },
                opt));
        }
    }

    // ── Navigation buttons ──
    auto previous = styledButton(" \u25C0 Prev ", [state] {
        if (state->page > 0)
            --state->page;
        state->selectedSlot = 0;
    });
    auto next     = styledButton(" Next \u25B6 ", [state, &pageSize] {
        ++state->page;
        state->selectedSlot = 0;
    });
    auto back =
        styledButton(" \u25C0 Return ", [state] { state->showDetail = false; });

    // ── Collect focusable children ──
    std::vector<Component> children;
    children.insert(children.end(), infoButtons.begin(), infoButtons.end());
    children.push_back(previous);
    children.push_back(next);
    children.push_back(back);
    auto container = Container::Vertical(std::move(children));

    // ── Main component ──
    auto component = Renderer(container, [=, config = std::move(config)] {
        // ── Detail view ──
        if (state->showDetail && config.renderDetailFn &&
            state->selectedRecord < config.totalRecords) {
            return vbox(Elements{
                       config.renderDetailFn(state->selectedRecord),
                       theme::spacer(1),
                       hbox(Elements{
                           back->Render() | center,
                           text("  ") | size(WIDTH, EQUAL, 2),
                           text("Esc to return") | color(theme::mutedText) |
                               dim,
                       }) | center,
                   }) |
                   theme::accentCard | flex;
        }

        // ── Filtered rows ──
        const auto allVisible = buildVisibleIndices(config);
        *totalVisible         = allVisible.size();
        const size_t totalPages =
            std::max<size_t>(1, (allVisible.size() + pageSize - 1) / pageSize);
        if (state->page >= totalPages)
            state->page = totalPages - 1;

        if (state->selectedSlot > pageSize)
            state->selectedSlot = 0;

        // ── Page slice ──
        const size_t begin = state->page * pageSize;
        const size_t end   = std::min(begin + pageSize, allVisible.size());

        visiblePageRows->clear();
        for (size_t off = begin; off < end; ++off) {
            visiblePageRows->push_back(allVisible[off]);
        }

        // ── Build Table grid ──
        // Grid layout:
        //   Col 0          : marker (▶ or empty)
        //   Col 1..H       : data columns (from renderRowFn)
        //   Col H+1 (opt)  : info button
        const bool   hasInfo   = config.hasDetailView && !infoButtons.empty();
        const size_t nDataCols = config.headers.size();
        const size_t nCols     = 1 + nDataCols + (hasInfo ? 1 : 0);

        std::vector<std::vector<Element>> grid;

        // ── Header row ──
        {
            std::vector<Element> headerRow;
            headerRow.reserve(nCols);
            headerRow.push_back(text("   ")); // marker placeholder
            for (const auto& h : config.headers) {
                headerRow.push_back(text(h));
            }
            if (hasInfo) {
                headerRow.push_back(text("Info"));
            }
            grid.push_back(std::move(headerRow));
        }

        // ── Data rows ──
        for (size_t slot = 0; slot < visiblePageRows->size(); ++slot) {
            const size_t idx        = (*visiblePageRows)[slot];
            const bool   isSelected = (slot == state->selectedSlot);

            auto cells = config.renderRowFn(idx);
            // Ensure we have the right number of data cells
            cells.resize(nDataCols, text(""));

            std::vector<Element> row;
            row.reserve(nCols);

            // Marker
            row.push_back(isSelected ? text(" \u25B6 ") | color(theme::accent)
                                     : text("   "));

            // Data cells
            for (auto& cell : cells) {
                row.push_back(std::move(cell));
            }

            // Info button
            if (hasInfo && slot < infoButtons.size()) {
                row.push_back(infoButtons[slot]->Render());
            }

            grid.push_back(std::move(row));
        }

        // ── Assemble and style Table ──
        Elements output;

        if (grid.size() <= 1) {
            // No data rows — show empty state
            output.push_back(theme::emptyState(config.emptyMessage));
        } else {
            ftxui::Table table(std::move(grid));

            // Outer border
            table.SelectAll().Border(LIGHT);

            // Vertical separators between data columns (skip marker col 0)
            if (nCols > 2) {
                table.SelectColumns(1, nCols - 1).SeparatorVertical(LIGHT);
            }

            // Horizontal separator below header
            table.SelectRow(0).SeparatorHorizontal(LIGHT);

            // Header style
            table.SelectRow(0).Decorate(bold | color(theme::accent) |
                                        bgcolor(theme::panelDark));

            // Marker column: fixed width, centered
            table.SelectColumn(0).Decorate(size(WIDTH, EQUAL, 3) | center);

            // Data columns: expand to fill remaining horizontal space
            if (nCols > 1) {
                table.SelectColumns(1, nCols - 1).Decorate(flex);
            }

            // Zebra striping on body
            if (grid.size() > 1) {
                table.SelectRows(1, grid.size() - 1)
                    .DecorateCellsAlternateRow(bgcolor(theme::stripe), 2, 0);
            }

            // Highlight selected row
            if (state->selectedSlot + 1 < grid.size()) {
                table.SelectRow(state->selectedSlot + 1)
                    .Decorate(bgcolor(theme::accentDim));
            }

            output.push_back(table.Render() | flex);
        }

        // ── Page controls ──
        output.push_back(hbox(Elements{
                             previous->Render(),
                             theme::pageIndicator(state->page, totalPages),
                             next->Render(),
                         }) |
                         bgcolor(theme::stripe));

        // ── Keyboard hints ──
        if (config.hasDetailView) {
            output.push_back(
                theme::helpText(
                    "\u2191\u2195 Select  |  Enter Open  |  Esc Back  |  "
                    "Pg\u2191\u2193 Page  |  Home End") |
                center | bgcolor(theme::panelDark));
        } else {
            output.push_back(
                theme::helpText("\u2191\u2195 Navigate  |  "
                                "Pg\u2191\u2193 Page  |  Home End") |
                center | bgcolor(theme::panelDark));
        }

        return vbox(std::move(output)) | flex;
    });

    // ── Keyboard event handler ──
    return CatchEvent(std::move(component), [state, visiblePageRows, pageSize,
                                             totalVisible](const Event& event) {
        if (event == Event::Escape) {
            if (state->showDetail) {
                state->showDetail = false;
                return true;
            }
            return false;
        }

        if (event == Event::Return && !state->showDetail &&
            !visiblePageRows->empty()) {
            if (state->selectedSlot < visiblePageRows->size()) {
                state->selectedRecord = (*visiblePageRows)[state->selectedSlot];
                state->showDetail     = true;
                return true;
            }
            return false;
        }

        if (event == Event::ArrowDown && !state->showDetail) {
            if (!visiblePageRows->empty()) {
                state->selectedSlot =
                    (state->selectedSlot + 1) % visiblePageRows->size();
                return true;
            }
            return false;
        }

        if (event == Event::ArrowUp && !state->showDetail) {
            if (!visiblePageRows->empty()) {
                if (state->selectedSlot == 0) {
                    state->selectedSlot = visiblePageRows->size() - 1;
                } else {
                    --state->selectedSlot;
                }
                return true;
            }
            return false;
        }

        // ── Page navigation ──
        if (event == Event::PageDown && !state->showDetail) {
            const size_t totalPages =
                std::max<size_t>(1, (*totalVisible + pageSize - 1) / pageSize);
            if (state->page + 1 < totalPages) {
                ++state->page;
                state->selectedSlot = 0;
            }
            return true;
        }

        if (event == Event::PageUp && !state->showDetail) {
            if (state->page > 0) {
                --state->page;
                state->selectedSlot = 0;
            }
            return true;
        }

        if (event == Event::Home && !state->showDetail) {
            state->page         = 0;
            state->selectedSlot = 0;
            return true;
        }

        if (event == Event::End && !state->showDetail) {
            const size_t totalPages =
                std::max<size_t>(1, (*totalVisible + pageSize - 1) / pageSize);
            state->page         = totalPages - 1;
            state->selectedSlot = 0;
            return true;
        }

        return false;
    });
}

} // namespace ui
