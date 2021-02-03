#include "base_input_manager.hpp"

namespace bcalc
{

//----------------------------------------------------------------------------//
// Public implementations                                                     //
//----------------------------------------------------------------------------//

void BaseInputManager::setVariablesRandomly ( std::size_t const size )
{
    assert( m_expressions.size() );

    std::srand( std::time( 0 ) );

    clearAndReserve( size );

    for ( auto & expr : m_expressions )
        for ( std::size_t i = 0; i < m_size; ++i )
            expr.push_back(
                    static_cast< Boolean >( ( std::rand() % 17 ) % 2 )
            );
}

//----------------------------------------------------------------------------//

void BaseInputManager::setViaTruthTable () // full-cased testing
{
    assert( m_expressions.size() );

    std::size_t tempsize = std::pow( 2, m_expressions.size() );
    std::size_t t1 = 0;
    std::size_t t2;
    
    clearAndReserve( tempsize );

    for ( auto & expr : m_expressions )
    {
        tempsize /= 2;
        t2 = tempsize;

        while ( t2 <= m_size )
        {
            for ( std::size_t j = t1; j < t2; ++j )
                expr.push_back( s_false );
            
            t1 += tempsize;
            t2 += tempsize;
            
            for ( std::size_t j = t1; j < t2; ++j )
                expr.push_back( s_true );
            
            t1 += tempsize;
            t2 += tempsize;
        }

        t1 = 0;
    }
}

//----------------------------------------------------------------------------//
// Protected implementations                                                  //
//----------------------------------------------------------------------------//

void BaseInputManager::clearAndReserve ( std::size_t const size )
{
    assert( size );

    m_size = size;

    for ( auto & expr : m_expressions )
    {
        LineOfTable().swap( expr );
        expr.reserve( m_size );
    }
}

//----------------------------------------------------------------------------//

void BaseInputManager::convertToBinaryVariables ( std::vector< std::size_t > & variables )
{
    std::size_t maxCounter = 1;
    
    findMaxBitWidth( variables, maxCounter );
    
    clearAndReserve( maxCounter );

    fillVariablesWithFalse();

    convertDecimalsToBinaries( variables );
}

//----------------------------------------------------------------------------//

void BaseInputManager::findMaxBitWidth ( std::vector< std::size_t > const & variables, std::size_t & maxCounter )
{
    std::size_t counter = 0;
    std::size_t tmp;
    
    for ( auto const var : variables )
    {
        tmp = var;
        
        while ( tmp )
        {
            tmp /= 2;
            ++counter;
        }
        
        if ( counter > maxCounter )
            maxCounter = counter;
        
        counter = 0;
    }
}

//----------------------------------------------------------------------------//

void BaseInputManager::fillVariablesWithFalse ()
{
    for ( auto & expr : m_expressions )
        for ( std::size_t k = 0; k < m_size; ++k )
            expr.push_back( s_false );
}

//----------------------------------------------------------------------------//

void BaseInputManager::convertDecimalsToBinaries ( std::vector< std::size_t > & variables )
{
    std::size_t i = 0;
    long j = 0;
    auto variableSize = m_expressions.size();
    
    while ( i < variableSize )
    {
        j = m_size - 1;

        while ( variables[ i ] && j >= 0 )
        {
            if ( variables[ i ] % 2 )
                m_expressions[ i ][ j ] = s_true;
            
            --j;
            variables[ i ] /= 2;
        }

        ++i;
    }
}

} // namespace bcalc
