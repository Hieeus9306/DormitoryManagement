#pragma once

#include "action_event.hpp"
#include "search_event.hpp"

#include <memory>

#include <ftxui/component/component.hpp>

namespace ui::events {

ftxui::Component createStudentDashboard(SearchState& searchState);
ftxui::Component createStudentActionMenu(std::shared_ptr<ActionState> state,
                                         int& contentMode);
ftxui::Component createStudentActionForms(std::shared_ptr<ActionState> state);

} // namespace ui::events
