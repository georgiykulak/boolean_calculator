#ifndef CLI_HPP
#define CLI_HPP

#include "rpe.hpp"
#include "classificator.hpp"
#include "cli_input_manager.hpp"
#include "cli_output_manager.hpp"

namespace bcalc
{

class BooleanCalculatorCli
{
public:
    BooleanCalculatorCli ( int argc, char ** argv );

    int run (); // TODO: Provide menu to work with more than one expression

private:
    void help ();
};

} // namespace bcalc

#endif // CLI_HPP //
