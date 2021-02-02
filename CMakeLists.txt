cmake_minimum_required( VERSION 3.19 )

project( boolean_calculator_cli )

set( CMAKE_CXX_STANDARD 20 )

add_library( bool_rpe_lib
    src/rpe.cpp src/rpe.hpp src/stack.hpp 
)

add_library( cli_lib
    src/cli.cpp src/cli.hpp
)

target_link_libraries( cli_lib
    bool_rpe_lib
)

add_executable( bool_calc
    src/main.cpp
)

target_link_libraries( bool_calc
    cli_lib
)