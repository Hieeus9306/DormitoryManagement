#include "io/file_io.hpp"
#include "ui/ui.hpp"
#include <iostream>

int main() {
    loadStudents();
    loadRooms();
    loadContracts();
    loadServiceInvoices();

    int result = ui::run();

    saveStudents();
    saveRooms();
    saveContracts();
    saveServiceInvoices();

    std::cout << "Program closed successfully.";

    return result;
}
