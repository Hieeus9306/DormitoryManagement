add_library(dormitory_lib STATIC
    # Core types and configuration
    core/types.hpp
    core/config.hpp

    # Application state
    state/state.cpp
    state/state.hpp

    # Domain logic
    domain/student.cpp
    domain/student.hpp
    domain/room.cpp
    domain/room.hpp
    domain/contract.cpp
    domain/contract.hpp
    domain/invoice.cpp
    domain/invoice.hpp
    domain/report.cpp
    domain/report.hpp

    # File I/O
    io/file_io.cpp
    io/file_io.hpp

    # Custom data structures
    libs/algorithms.hpp
    libs/date.hpp
    libs/vector.hpp
)

target_include_directories(dormitory_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The library reads data files via core/config.hpp which uses DATA_DIR_PATH.
# PUBLIC so both the library and any target linking it get the definition.
target_compile_definitions(dormitory_lib PUBLIC
    "DATA_DIR_PATH=\"${CMAKE_SOURCE_DIR}/data\""
)
