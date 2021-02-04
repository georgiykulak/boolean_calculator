#include "rpe.hpp"
#include "cli_input_manager.hpp"

namespace bcalc
{

bool InputManagerCLI::input ( Table & expressions, std::size_t & size )
{
    assert( expressions.size() );

    m_expressions = std::move( expressions );
    m_size = size;
    size = 0;

    char answer;
    bool shouldGetClassification = false;

    std::cout << "Do you want to add elements manually? (y/n) ";
    std::cin >> answer;

    if ( std::toupper( answer ) != 'Y' )
    {
        std::cout << "Select the option (r - random / t - truth table) ";
        std::cin >> answer;
        
        if ( std::toupper( answer ) == 'R' )
        {
            std::size_t bits;

            std::cout << "Enter the digit depth of variables ";
            std::cin >> bits;

            setVariablesRandomly( bits );
        }
        else
        {
            setViaTruthTable();

            std::cout <<
                "Do you want to see classification of boolean function? (y/n) ";
            std::cin >> answer;
            
            if ( std::toupper( answer ) == 'Y' )
                shouldGetClassification = true;
        }
    }
    else
    {
        std::cout << "Decimal or bitwise inputs? (d/b) ";
        std::cin >> answer;
        
        if ( std::toupper( answer ) == 'B' )
        {
            std::size_t bits;
            std::cout << "Enter the digit depth of variables ";
            std::cin >> bits;

            setVariablesBinaries( bits ); // Do not use bits
        }
        else
        {
            setVariablesDecimals();
        }
    }

    expressions = std::move( m_expressions );
    size = m_size;

    return shouldGetClassification;
}

//----------------------------------------------------------------------------//

void InputManagerCLI::setVariablesDecimals ()
{
    assert( m_expressions.size() );

    std::vector< std::size_t > variables( m_expressions.size() );
    std::size_t l = 0;

    std::cout << "Enter through the enter every variable (decimal):\n";
    
    for ( auto & var : variables )
    {
        std::cout << l + 1 << " variable = ";
        std::cin >> var;
        ++l;
    }

    convertToBinaryVariables( variables );
}

//----------------------------------------------------------------------------//

void InputManagerCLI::setVariablesBinaries ( std::size_t const size )
{
    assert( m_expressions.size() );

    clearAndReserve( size );

    bool tmp;
    std::size_t l = 0;

    std::cout << "Enter through the space every variable:\n";
    
    for ( auto & expr : m_expressions )
    {
        std::cout << l + 1 << " variable = ";
        ++l;
        
        for ( std::size_t i = 0; i < m_size; ++i )
        {
            std::cin >> tmp;
            expr.push_back( static_cast< Boolean >( tmp ) );
        }
    }
}

} // namespace bcalc
