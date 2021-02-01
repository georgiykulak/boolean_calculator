cmake_minimum_required( VERSION 3.19 )

project( boolean_calculator_cli )

set( CMAKE_CXX_STANDARD 20 )

add_executable( bool_calc
    src/main.cpp src/analysis.hpp src/rpe.hpp src/stack.hpp 
)