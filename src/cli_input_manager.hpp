#ifndef CLI_INPUT_MANAGER_HPP
#define CLI_INPUT_MANAGER_HPP

#include "rpe.hpp"

#include <iostream>

namespace bcalc
{

// InputManager for CLI
class InputManagerCLI final : public BaseInputManager
{
public:
    void input ( BoolRPN::Table & expressions, std::size_t & size ) override;

    void setVariablesDecimals () override;

    void setVariablesBinaries ( std::size_t const size ) override;
};

} // namespace bcalc

#endif // CLI_INPUT_MANAGER_HPP //
