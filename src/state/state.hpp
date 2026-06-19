#pragma once

#include "core/types.hpp"

/*
────────────────────────────────────────────────────────────────────────────────
Global application state — single definition in state.cpp
────────────────────────────────────────────────────────────────────────────────
*/
extern base::Vector<Student>        studentsList;
extern base::Vector<Room>           roomsList;
extern base::Vector<Contract>       contractsList;
extern base::Vector<ServiceInvoice> serviceInvoicesList;
