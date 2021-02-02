#ifndef CLI_HPP
#define CLI_HPP

#include "rpe.hpp"
#include <cctype>

namespace bcalc
{

class BooleanCalculatorCli
{
public:
    ~BooleanCalculatorCli () = default;

    BooleanCalculatorCli () = default;

    BooleanCalculatorCli ( int argc, char ** argv );

    int run ();

private:
    void construct ();

    void help ();
};

} // namespace bcalc

#endif // CLI_HPP