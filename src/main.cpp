#include "cli.hpp"

int main ( int argc, char * argv[] )
{
    bcalc::BooleanCalculatorCli cli( argc, argv );

    return cli.run();
}
