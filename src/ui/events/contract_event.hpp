#pragma once

#include "action_event.hpp"
#include "search_event.hpp"

#include <memory>

#include <ftxui/component/component.hpp>

namespace ui::events {

ftxui::Component createContractDashboard(SearchState& searchState);
ftxui::Component createContractActionMenu(std::shared_ptr<ActionState> state,
                                          int& contentMode);
ftxui::Component createContractActionForms(std::shared_ptr<ActionState> state);

} // namespace ui::events
