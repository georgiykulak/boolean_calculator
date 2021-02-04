#include "rpe.hpp"

namespace bcalc
{

//----------------------------------------------------------------------------//
// Public implementations                                                     //
//----------------------------------------------------------------------------//

BoolRPN::BoolRPN ( std::string const & expr )
{
    setExpression( expr );
}

//----------------------------------------------------------------------------//

void BoolRPN::setExpression ( std::string const & expr ) // regular expression
{
    assert( !expr.empty() );

    m_formula = expr;

    analyze();

    // check();
    // init();
}

//----------------------------------------------------------------------------//

bool BoolRPN::set ( BaseInputManager & mngr )
{
    assert( !m_formula.empty() );

    bool shouldGetClassification = mngr.input( m_expressions, m_size );

    // check ...

    LineOfTable().swap( m_answers );
    m_answers.reserve( m_size );

    return shouldGetClassification;
}

//----------------------------------------------------------------------------//

void BoolRPN::get ( BaseOutputManager & mngr ) const noexcept
{
    assert( !m_formula.empty() );

    mngr.output( getVariables(), getAnswer() );
}

//----------------------------------------------------------------------------//

Table const & BoolRPN::getVariables () const noexcept
{
    return m_expressions;
}

//----------------------------------------------------------------------------//

LineOfTable const & BoolRPN::getAnswer () const noexcept
{
    return m_answers;
}

//----------------------------------------------------------------------------//

void BoolRPN::getClassification () noexcept
{
    assert( m_size );

    bool flag = true;
    bool exit_flag = false;
    bool const c0 = m_answers[ 0 ];
    bool const f = m_answers[ m_size - 1 ];
    bool c = m_answers[ 0 ];

    if ( m_answers[ 0 ] )
        m_classes[ 0 ] = true;

    if ( m_answers[ m_size - 1 ] != 1 )
        m_classes[ 1 ] = true;

    for ( std::size_t i = 0; i < m_size / 2; ++i )
    {
        if ( m_answers[ i ] == m_answers[ m_size - i - 1 ] )
        {
            m_classes[ 2 ] = true;
            break;
        }
    }

    for ( int i = 0; i < m_size - 1; ++i )
    {
        for ( int k = i + 1; k < m_size; ++k )
        {
            for ( int j = 0; j < m_expressions.size(); ++j )
            {
                if ( m_expressions[ j ][ i ] > m_expressions[ j ][ k ] )
                {
                    flag = false;
                    break;
                }
            }

            if ( flag && m_answers[ i ] > m_answers[ k ] )
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

    for ( auto && expr : m_expressions )
    {
        if ( m_answers[ j ] ^ c0 )
            c ^= 1;

        j *= 2;
    }
    
    if ( c != f )
        m_classes[ 4 ] = true;

    m_classP = true;

    for ( auto & elem : m_classes )
    {
        if ( !elem )
        {
            m_classP = false;
            break;
        }
    }

    if ( !m_classes[ 0 ] )
        std::cout << "T0: Zero is constant\n";
    else
        std::cout << "T0: Zero is NOT constant\n";
    
    if ( !m_classes[ 1 ] )
        std::cout << "T1: One is constant\n";
    else
        std::cout << "T1: One is NOT constant\n";
    
    if ( !m_classes[ 2 ] )
        std::cout << "S:  The function is self-dual\n";
    else
        std::cout << "S:  The function is NOT self-dual\n";
    
    if ( !m_classes[ 3 ] )
        std::cout << "M:  The function is monotone\n";
    else
        std::cout << "M:  The function is NOT monotone\n";
    
    if ( !m_classes[ 4 ] )
        std::cout << "L:  The function is linear\n";
    else
        std::cout << "L:  The function is NOT linear\n";
    
    if ( m_classP )
        std::cout << "The function is full\n";
    else
        std::cout << "The function is NOT full\n";
}

//----------------------------------------------------------------------------//

void BoolRPN::calculateExpression ()
{
    assert( !m_formula.empty() );
    assert( m_expressions.size() );

    std::vector< char > stk_c;
    std::vector< bool > stk_b;
    std::string bool_formula;
    std::size_t l = 0;
    std::size_t i = 0;
    unsigned temp = 1;

    while ( l < m_size )
    {
        while ( i < m_formula.length() )
        {
            if ( is_num( m_formula[ i ] ) )
            {
                bool_formula += m_formula[ i ];
                bool_formula += ' ';
            }
            else if ( is_oper( m_formula[ i ] )
                   || is_unar_oper( m_formula[ i ] ) )
            {
                while ( !stk_c.empty()
                    && prior( stk_c.back() ) >= prior( m_formula[ i ] ) )
                {
                    bool_formula += stk_c.back();
                    stk_c.pop_back();
                    bool_formula += ' ';
                }
                
                stk_c.push_back( m_formula[ i ] );
            }
            // отсчет начинается с 1, вместо "x1" можно писать "x"
            else if ( is_var( m_formula[ i ] ) )
            {
                ++i;

                if ( is_number( m_formula[ i ]) && m_formula[ i ] > '0' )
                {
                    temp -= 1;
                    
                    while ( is_number( m_formula[ i ] ) )
                    {
                        temp += ( m_formula[ i ] - 48 );
                        temp *= 10;
                        ++i;
                    }

                    temp /= 10;
                }

                bool_formula += ( m_expressions[ temp - 1 ][ l ] + 48 );
                bool_formula += ' ';
                temp = 1;
                --i;
            }
            else if ( m_formula[ i ] == '(' )
            {
                stk_c.push_back( m_formula[ i ] );
            }
            else if ( m_formula[ i ] == ')' )
            {
                while ( stk_c.back() != '(' )
                {
                    bool_formula += stk_c.back();
                    stk_c.pop_back();
                    bool_formula += ' ';
                }

                if ( stk_c.back() == '(' )
                    stk_c.pop_back();
            }

            if ( !stk_c.empty()
                && ( m_formula[ i ] == '=' || i + 1 == m_formula.length() )
            )
            {
                while ( !stk_c.empty() )
                {
                    bool_formula += stk_c.back();
                    stk_c.pop_back();
                    bool_formula += ' ';
                }

                break;
            }

            ++i;
        }

        i = 0;

        while ( i < bool_formula.length() )
        {
            if ( is_num( bool_formula[ i ] ) )
            {
                stk_b.push_back( bool_formula[ i ] - 48 );
            }
            else if ( is_oper( bool_formula[ i ] ) )
            {
                auto second = stk_b.back();
                stk_b.pop_back();
                auto first = stk_b.back();
                stk_b.pop_back();

                stk_b.push_back(
                    calc( second, bool_formula[ i ], first )
                );
            }
            else if ( is_unar_oper( bool_formula[ i ] ) )
            {
                stk_b.back() = !stk_b.back();
            }

            ++i;
        }

        i = 0;
        bool_formula = "";
        m_answers.push_back( stk_b.back() );
        stk_b.pop_back();
        ++l;
    }
}

//----------------------------------------------------------------------------//

constexpr int BoolRPN::prior ( char const c ) noexcept
{
    if ( c == '(' ) return 1;
    else if ( c == '^' ) return 2;
    else if ( c == '~' ) return 3;
    else if ( c == '>' ) return 4;
    else if ( c == '<' ) return 5;
    else if ( c == '|' ) return 6;
    else if ( c == '&' ) return 7;
    else if ( c == '!' ) return 8;

    assert( false );
    
    return false;
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_num ( char const c ) noexcept
{
    return c == '0' || c == '1';
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_number ( char const c ) noexcept
{
    return std::isdigit( static_cast< unsigned char >( c ) );
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_var ( char const c ) noexcept
{
    return c == 'X' || c == 'x';
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_oper ( char const c ) noexcept
{
    return c == '^' || c == '~' || c == '>' || c == '<' || c == '|' || c == '&';
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_unar_oper ( char c ) noexcept
{
    return c == '!';
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::is_other ( char const c ) noexcept
{
    return c == '!' || c == '(' || c == ')' || c == '=' || c == ' ';
}

//----------------------------------------------------------------------------//
// Private implementations                                                    //
//----------------------------------------------------------------------------//

void BoolRPN::analyze () // TODO: RegExp
{
    assert( m_formula.length() );

    std::size_t size = m_formula.length();
    std::size_t i = 0;
    std::size_t j = 0;
    std::size_t tempi;
    std::size_t flag_corr = true;
    std::size_t flag = false;
    std::size_t counter1 = 0;
    long loop = 0;
    std::size_t maxvar = 1;
    std::size_t temp;
    
    while ( flag_corr )
    {
        flag_corr = false;
        
        while ( i < size )
        {
            if ( !is_number( m_formula[ i ] )
              && !is_other( m_formula[ i ] )
              && !is_oper( m_formula[ i ] )
              && !is_var( m_formula[ i ] )
            )
            {
                std::cout << "The " << i + 1 << " symbol is not suitable!\n";
                std::cout << "Try to enter formula again\n";
                
                std::getline( std::cin, m_formula );
                flag_corr = true;
                break;
            }

            if ( m_formula[ i ] == '(' )
                ++loop;
            if ( m_formula[ i ] == ')' )
                --loop;
            ++i;
        }

        if ( loop )
        {
            std::cout << "There is an missing loop!\n"; // Replace with exception
            std::cout << "Try to enter formula again\n";
            
            std::getline( std::cin, m_formula );
            flag_corr = true;
        }
    }

    i = 0;
    while ( i < size )
    {
        if ( m_formula[ i ] == '!' )
        {
            counter1 = tempi = i;
            while ( true )
            {
                ++counter1;
                ++i;
                
                if ( m_formula[ i ] == ' ' || m_formula[ i ] == '!')
                    continue;
                else
                    break;
            }

            if ( counter1 - tempi > 1 )
            {
                if ( !( ( counter1 - tempi ) % 2 ) )
                    for ( int j = tempi; j < counter1; ++j )
                        m_formula[ j ] = ' ';
                else
                    for ( int j = tempi; j < counter1 - 1; ++j )
                        m_formula[ j ] = ' ';
                flag = true;
            }

            counter1 = 0;
            i = tempi;
        }
        else if ( is_var( m_formula[ i ] ) )
        {
            ++i;
            temp = 1;

            if ( !is_number( m_formula[ i ] ) )
            {
                ++i;
                continue;
            }
            
            j = i;
            
            if ( is_number( m_formula[ j ] ) && m_formula[ j ] > '0' )
            {
                temp -= 1;

                while ( is_number( m_formula[ j ] ) )
                {
                    temp += ( m_formula[ j ] - 48 );
                    temp *= 10;
                    ++j;
                }
                temp /= 10;
            }
            
            if ( maxvar < temp )
                maxvar = temp;
        }

        ++i;
    }

    m_expressions.resize( maxvar );

    if ( flag )
    {
        std::cout << "This is more correct expression:\n";
        for ( int i = 0; i < size; ++i )
            if ( m_formula[ i ] != ' ' )
                std::cout << m_formula[ i ];

        std::cout << std::endl;
    }
}

//----------------------------------------------------------------------------//

constexpr bool BoolRPN::calc ( bool second, char oper, bool first ) const noexcept
{
    if ( oper == '&' ) return first & second;
    else if ( oper == '|' ) return first | second;
    else if ( oper == '<' ) return first & !second;
    else if ( oper == '>' ) return !first | second;
    else if ( oper == '~' ) return first == second;
    else if ( oper == '^' ) return first ^ second;

    assert( false );

    return false;
}

} // namespace bcalc
