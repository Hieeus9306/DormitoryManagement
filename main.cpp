#include "io/file_io.hpp"
#include "ui/ui.hpp"

int main() {
    loadStudents();
    loadRooms();
    loadContracts();
    loadServiceInvoices();

    return ui::run();
}
