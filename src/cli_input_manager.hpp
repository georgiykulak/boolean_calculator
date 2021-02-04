#ifndef CLI_INPUT_MANAGER_HPP
#define CLI_INPUT_MANAGER_HPP

#include "base_input_manager.hpp"

#include <iostream>

namespace bcalc
{

// InputManager for CLI
class InputManagerCLI final : public BaseInputManager
{
public:
    bool input ( Table &, std::size_t & ) override;

    void setVariablesDecimals () override;

    void setVariablesBinaries ( std::size_t const ) override;
};

} // namespace bcalc

#endif // CLI_INPUT_MANAGER_HPP //
