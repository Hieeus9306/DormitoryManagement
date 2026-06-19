#pragma once

#include "action_event.hpp"
#include "search_event.hpp"

#include <memory>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui::events {

ftxui::Component createInvoiceDashboard(SearchState& searchState);
ftxui::Component createInvoiceActionMenu(std::shared_ptr<ActionState> state,
                                         int& contentMode);
ftxui::Component createInvoiceActionForms(std::shared_ptr<ActionState> state);

} // namespace ui::events
