#include "cli_output_manager.hpp"

namespace bcalc
{

void OutputManagerCLI::output (
        Table const & expressions
    ,   LineOfTable const & answers
) const noexcept
{
    getVariables( expressions );
    getAnswer( answers );
}

//----------------------------------------------------------------------------//

void OutputManagerCLI::getVariables ( Table const & expressions ) const noexcept
{
    if ( !expressions.size() )
        return;

    std::cout << "Variables:\n";
    
    std::size_t l = 0;
    
    for ( auto const & expr : expressions )
    {
        std::cout << l + 1 << " variable = ";
        ++l;

        for ( auto const & elem : expr )
            std::cout << static_cast< bool >( elem );

        std::cout << std::endl;
    }
}

//----------------------------------------------------------------------------//

void OutputManagerCLI::getAnswer ( LineOfTable const & answers ) const noexcept
{
    if ( !answers.size() )
        return;

    std::cout << "Answer is ";

    for ( auto const & elem : answers )
        std::cout << static_cast< bool >( elem );

    std::cout << std::endl;
}

} // namespace bcalc
