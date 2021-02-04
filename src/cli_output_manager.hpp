#ifndef CLI_OUTPUT_MANAGER_HPP
#define CLI_OUTPUT_MANAGER_HPP

#include "base_output_manager.hpp"

#include <iostream>

namespace bcalc
{

// OutputManager for CLI
class OutputManagerCLI final : public BaseOutputManager
{
public:
    void output ( Table const &, LineOfTable const & ) const noexcept override;
    void getVariables ( Table const & ) const noexcept override;
    void getAnswer ( LineOfTable const & ) const noexcept override;
};

} // namespace bcalc

#endif // CLI_OUTPUT_MANAGER_HPP //