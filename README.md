# Dormitory Management System

A terminal-based dormitory management application built with **C++20** and **FTXUI**, designed as a final-term project for a Programming Techniques course.

## Features

| Feature | Description |
|---------|-------------|
| **Student Management** | Add, update, and search students by ID or name. |
| **Room Management** | Add, update, delete rooms with capacity tracking; assign/remove students. |
| **Contract Management** | Register, transfer, check-out rooms with full contract lifecycle. |
| **Invoice Management** | Create monthly service invoices with automatic electricity (tiered) and water fee calculation. |
| **Reports** | Overdue invoices, expiring contracts (30-day window), room occupancy statistics. |
| **Search & Filter** | Per-section search bar with field selector and real-time filtering. |
| **Pagination** | Keyboard and button-driven page navigation (PgUp/PgDn/Home/End). |
| **Detail Views** | Full record details accessible via Enter or Info button. |
| **Data Persistence** | CSV-based file I/O with load-on-startup and save-on-shutdown. |

## Prerequisites

- **CMake** ≥ 3.10
- **C++20** compiler (GCC 11+, Clang 14+, MSVC 2022+)
- **Ninja** build system (recommended) or `make` / MSBuild
- Internet connection at first build (fetches FTXUI v5.0.0)

## Build

```bash
# Configure & Build (Debug)
cmake --preset default
cmake --build --preset default
```

For a release build:

```bash
cmake --preset release
cmake --build --preset release
```

## Run

```bash
./output/dormitory.exe
```

## Usage

### Navigation

| Key | Action |
|-----|--------|
| ← → | Navigate sidebar / form fields |
| ↑ ↓ | Select rows in list |
| Enter | Open detail view / confirm action |
| Esc | Back / cancel |
| PgUp / PgDn | Previous / next page |
| Home / End | First / last page |

### Sections

| Section | Description |
|---------|-------------|
| **Students** | View list, search, update student info. |
| **Rooms** | View list with occupancy, add/update/delete rooms. |
| **Contracts** | Register new students into rooms, transfer, check out. |
| **Invoices** | Create monthly invoices, mark payments. |
| **Report** | View overdue invoices, expiring contracts, room usage. |

## Entity Overview

| Entity | Key Fields | File |
|--------|------------|------|
| **Student** | ID, name, class, priority, phone, email | `Students.txt` |
| **Room** | ID, capacity (4/6/8), price, student list | `Rooms.txt` |
| **Contract** | ID, student ID, room ID, start/end dates, active status | `Contracts.txt` |
| **ServiceInvoice** | ID, room ID, month/year, meter readings, total, paid status | `ServiceInvoices.txt` |

## Data Files

All data is stored as plain CSV in `data/`:

```
data/
├── Students.txt
├── Rooms.txt
├── Contracts.txt
└── ServiceInvoices.txt
```

Data is loaded automatically at startup and saved after every mutation.

## Project Structure

```
├── main.cpp                  # Entry point
├── CMakeLists.txt            # Root build definition
├── CMakePresets.json         # Build presets (Debug / Release)
└── src/
    ├── core/                 # Domain structs, config constants
    ├── domain/               # Business logic per entity
    ├── io/                   # CSV file I/O
    ├── libs/                 # Custom Vector, Date, algorithms, string utils
    ├── state/                # Global application state (4 data lists)
    └── ui/                   # FTXUI terminal interface
        └── events/           # Section-specific event handlers
```

## Tech Stack

| Technology | Role |
|------------|------|
| **C++20** | Language standard |
| **FTXUI** v5.0.0 | Terminal UI framework (DOM, components, event handling) |
| **CMake + Ninja** | Build system |

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Custom `base::Vector<T>` over STL containers** | Pedagogical constraint — demonstrates memory management, allocators, and dynamic arrays from scratch. |
| **Custom `base::Date` over `<chrono>`** | Avoids chrono complexity for the simple DD/MM/YYYY parsing and arithmetic this domain needs. |
| **Global mutable state (`state/state.hpp`)** | Keeps the course project simple — no DI, no state management framework. Four global `Vector`s are the single source of truth. |
| **Sorted-by-ID invariant** | Students, rooms, and invoices are kept sorted by ID via binary search, enabling O(log n) lookups. |
| **CSV without escaping** | Works because sample data contains no commas, quotes, or newlines. Simple and sufficient for the scope. |
| **FTXUI over ncurses** | Modern C++ DOM-style UI composition, built-in table rendering, keyboard/mouse support, and cross-platform. |
| **Tiered electricity pricing** | Mirrors real-world Vietnamese utility billing (4 progressive tiers). |
| **Save after every mutation** | Each business operation persists to disk immediately, preventing data loss on crash. |
| **Reusable `Dashboard` component** | `DashboardConfig` with `filterFn`/`renderRowFn`/`renderDetailFn` callbacks drives all four entity lists with zero duplication. |
| **Separation of library and executable** | `dormitory_lib` (static lib) packages domain logic independently from UI, enabling potential unit tests without FTXUI dependency. |

## Notes

- Electricity fees use progressive tiered pricing (4 tiers); water fees use a flat rate.
- Student lists in rooms are pipe-delimited (`|`).
- Contracts are displayed newest-first.
- This is a course project, not intended for production deployment.
