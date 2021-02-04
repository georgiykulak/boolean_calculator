#include "classificator.hpp"

namespace bcalc
{

void Classificator::get (
        Table const & expressions
    ,   LineOfTable const & answers
    ,   InputFunction inputGetter
    ,   OutputFunction outputGetter
) noexcept
{
    if ( isNotSetViaTruthTable( expressions ) )
        return;

    if ( !inputGetter() )
        return;

    auto const sizeOfVars = expressions.size();
    
    assert( sizeOfVars );
    
    auto const size = answers.size();
    
    assert( size );

    // check ...

    bool flag = true;
    bool exit_flag = false;
    bool const c0 = answers[ 0 ];
    bool const f = answers[ size - 1 ];
    bool c = answers[ 0 ];

    for ( auto & b : m_classes )
        b = false;

    if ( answers[ 0 ] )
        m_classes[ 0 ] = true;

    if ( answers[ size - 1 ] != 1 )
        m_classes[ 1 ] = true;

    for ( std::size_t i = 0; i < size / 2; ++i )
    {
        if ( answers[ i ] == answers[ size - i - 1 ] )
        {
            m_classes[ 2 ] = true;
            break;
        }
    }

    for ( int i = 0; i < size - 1; ++i )
    {
        for ( int k = i + 1; k < size; ++k )
        {
            for ( int j = 0; j < sizeOfVars; ++j )
            {
                if ( expressions[ j ][ i ] > expressions[ j ][ k ] )
                {
                    flag = false;
                    break;
                }
            }

            if ( flag && answers[ i ] > answers[ k ] )
            {
                m_classes[ 3 ] = true;
                exit_flag = true;
                break;
            }

            flag = true;
        }

        if ( exit_flag )
            break;
    }

    int j = 1;

    for ( auto const & expr : expressions )
    {
        if ( answers[ j ] ^ c0 )
            c ^= 1;

        j *= 2;
    }
    
    if ( c != f )
        m_classes[ 4 ] = true;

    m_classes[ 5 ] = true;

    for ( std::size_t i = 0; i < 5; ++i )
    {
        if ( !m_classes[ i ] )
        {
            m_classes[ 5 ] = false;
            break;
        }
    }

    Classificator::Loop::forEach( m_classes, outputGetter );
}

//----------------------------------------------------------------------------//

bool Classificator::isNotSetViaTruthTable (
        Table const & expressions
) const noexcept
{
    // TODO: isNotSetViaTruthTable function

    return false;
}

} // namespace bcalc
