#pragma once

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "libs/date.hpp"

namespace ui::events {

struct StudentActionState {
    int         action = 0;
    std::string updateId;
    std::string updateName;
    std::string updateClass;
    bool        updatePriority = false;
    std::string updatePhone;
    std::string updateEmail;
    std::string message = "Choose an action and fill the required fields.";
};

struct RoomActionState {
    int         action = 0;
    std::string updateId;
    std::string updateType;
    std::string updatePrice;
    std::string addId;
    std::string addType;
    std::string addPrice;
    std::string removeId;
    std::string message = "Choose an action and fill the required fields.";
};

struct ContractActionState {
    int         action = 0;
    std::string registerStudentId;
    std::string registerName;
    std::string registerClass;
    bool        registerPriority = false;
    std::string registerPhone;
    std::string registerEmail;
    std::string registerRoomId;
    std::string registerStartDate;
    std::string registerEndDate;
    std::string transferStudentId;
    std::string transferRoomId;
    std::string transferStartDate;
    std::string transferEndDate;
    std::string checkoutStudentId;
    std::string message = "Choose an action and fill the required fields.";
};

struct InvoiceActionState {
    int         action = 0;
    std::string roomId;
    std::string month;
    std::string year;
    std::string electricityIndex;
    std::string waterIndex;
    std::string invoiceId;
    bool        isPaid  = true;
    std::string message = "Choose an action and fill the required fields.";
};

struct ActionState {
    std::vector<std::string> studentActions  = {"Update"};
    std::vector<std::string> roomActions     = {"Update", "Add", "Delete"};
    std::vector<std::string> contractActions = {
        "Register Room", "Transfer Room", "Check Out Room"};
    std::vector<std::string> invoiceActions = {"Create Invoice",
                                               "Update Payment Status"};
    StudentActionState       students;
    RoomActionState          rooms;
    ContractActionState      contracts;
    InvoiceActionState       invoices;
};

struct BusinessActionComponents {
    std::vector<ftxui::Component> sectionMenus;
    ftxui::Component              form;
};

ftxui::Component createTextInput(std::string&       value,
                                 const std::string& placeholder);
ftxui::Component createActionMenu(std::vector<std::string>& actions,
                                  int& selected, int& contentMode);
ftxui::Element   field(const std::string& label, const ftxui::Component& input);
ftxui::Element   titledForm(const std::string& title, ftxui::Elements rows,
                            const std::string& message);

BusinessActionComponents createBusinessActions(int& selectedSection,
                                               int& contentMode);

} // namespace ui::events
