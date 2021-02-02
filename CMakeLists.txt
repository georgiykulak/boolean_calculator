cmake_minimum_required( VERSION 3.19 )

project( boolean_calculator_cli )

set( CMAKE_CXX_STANDARD 20 )

add_library( bool_rpn_lib
    src/rpe.cpp src/rpe.hpp
)

add_library( cli_lib
    src/cli.cpp src/cli.hpp
)

target_link_libraries( cli_lib
    bool_rpn_lib
)

add_executable( bool_calc
    src/main.cpp
)

target_link_libraries( bool_calc
    cli_lib
)