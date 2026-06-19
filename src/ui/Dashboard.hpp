#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {

/// Configuration for a reusable paginated dashboard.
///
/// Encapsulates the common pattern: filtered list with optional detail view,
/// page navigation, and per-row info buttons.
struct DashboardConfig {
    /// Column headers for the summary table
    std::vector<std::string> headers;

    /// Number of rows per page. 0 = auto-calculate from terminal height.
    size_t pageSize = 0;

    /// Whether this dashboard shows a detail view (Room, Contract, Invoice)
    bool hasDetailView = false;

    /// Total number of records in the underlying data set.
    /// The dashboard iterates indices [0, totalRecords).
    size_t totalRecords = 0;

    /// Returns true if the record at @p index passes the current search filter.
    /// Called for indices in [0, totalRecords).
    std::function<bool(size_t index)> filterFn;

    /// Renders a single row of the summary table for record @p index.
    /// Returns a vector of cell elements (one per column, no separators
    /// needed).
    std::function<std::vector<ftxui::Element>(size_t index)> renderRowFn;

    /// Renders the full detail view for record @p index (only if
    /// hasDetailView).
    std::function<ftxui::Element(size_t index)> renderDetailFn;

    /// Optional empty-state message override (default: "No matching records")
    std::string emptyMessage = "No matching records";
};

/// Creates a fully wired paginated dashboard component.
///
/// Handles: search filtering, pagination with Prev/Next, optional detail view
/// with Info buttons and a Back button, empty-state messaging.
ftxui::Component createDashboard(DashboardConfig config);

} // namespace ui
