cmake_minimum_required( VERSION 3.19 )

project( boolean_calculator_cli )

set( CMAKE_CXX_STANDARD 20 )

# TODO: Separate library from executable to custom functions
# (no need to build CLI for Frontend)

add_library( classificator
    src/classificator.cpp src/classificator.hpp
)

add_library( base_input_manager
    src/base_input_manager.cpp src/base_input_manager.hpp
)

add_library( bool_rpn
    src/rpe.cpp src/rpe.hpp src/base_input_manager.hpp src/base_output_manager.hpp
)

target_link_libraries( bool_rpn
    base_input_manager
)

add_library( cli_input_manager
    src/cli_input_manager.cpp src/cli_input_manager.hpp src/base_input_manager.hpp
)

target_link_libraries( cli_input_manager
    base_input_manager
)

add_library( cli_output_manager
    src/cli_output_manager.cpp src/cli_output_manager.hpp src/base_output_manager.hpp
)

target_link_libraries( cli_output_manager
    classificator
)

add_library( cli_lib
    src/cli.cpp src/cli.hpp
)

target_link_libraries( cli_lib
    bool_rpn cli_input_manager cli_output_manager
)

add_executable( bool_calc
    src/main.cpp src/cli.hpp
)

target_link_libraries( bool_calc
    cli_lib
)
