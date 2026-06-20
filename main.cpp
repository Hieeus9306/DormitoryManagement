#include "io/file_io.hpp"
#include "ui/ui.hpp"

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

    return result;
}
