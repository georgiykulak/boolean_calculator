#include "rpe.hpp"

namespace bcalc
{

BoolRPE::BoolRPE ( std::string const & expr )
{
    setExpression( expr );
}

void BoolRPE::setExpression ( std::string const & expr ) // regular expression
{
    assert( !expr.empty() );

    m_formula = expr;

    analyze();

    // check();
    // init();
}

void BoolRPE::setVariablesBinaries ( std::size_t const size )
{
    assert( !m_formula.empty() );
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

void BoolRPE::setVariablesDecimals ()
{
    assert( !m_formula.empty() );
    assert( m_expressions.size() );

    std::vector< std::size_t > variables( m_expressions.size() );
    std::size_t maxCounter = 1;
    std::size_t temp;
    std::size_t counter = 0;
    std::size_t i = 0;
    long j = 0;
    std::size_t l = 0;

    std::cout << "Enter through the enter every variable (decimal):\n";
    
    for ( auto & var : variables )
    {
        std::cout << l + 1 << " variable = ";
        std::cin >> var;
        temp = var;
        
        while ( temp )
        {
            temp /= 2;
            ++counter;
        }
        
        if ( counter > maxCounter )
            maxCounter = counter;
        
        counter = 0;
    }
    
    clearAndReserve( maxCounter );

    for ( auto & expr : m_expressions )
        for ( std::size_t k = 0; k < m_size; ++k )
            expr.push_back( s_false );

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

void BoolRPE::setVariablesRandomly ( std::size_t const size )
{
    assert( !m_formula.empty() );
    assert( m_expressions.size() );

    srand( time( NULL ) );

    clearAndReserve( size );

    for ( auto & expr : m_expressions )
        for ( std::size_t i = 0; i < m_size; ++i )
            expr.push_back( static_cast< Boolean >( ( rand() % 17 ) % 2 ) );
}

void BoolRPE::getVariables () const noexcept
{
    std::cout << "Variables:\n";
    
    std::size_t l = 0;
    
    for ( auto const & expr : m_expressions )
    {
        std::cout << l + 1 << " variable = ";
        ++l;

        for ( auto const & elem : expr )
            std::cout << static_cast< bool >( elem );
        std::cout << std::endl;
    }
}

void BoolRPE::getAnswer () const noexcept
{
    std::cout << "Answer is ";
    for ( auto const & elem : m_answers )
        std::cout << static_cast< bool >( elem );
    std::cout << std::endl;
}

void BoolRPE::calculateExpression ()
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

void BoolRPE::clearAndReserve ( std::size_t const size )
{
    assert( size );

    m_size = size;

    for ( auto & expr : m_expressions )
    {
        LineOfTable().swap( expr );
        expr.reserve( m_size );
    }

    LineOfTable().swap( m_answers );
    m_answers.reserve( m_size );
}

int BoolRPE::prior ( char const c ) noexcept
{
    if ( c == '(' ) return 1;
    else if ( c == '^' ) return 2;
    else if ( c == '~' ) return 3;
    else if ( c == '>' ) return 4;
    else if ( c == '<' ) return 5;
    else if ( c == '|' ) return 6;
    else if ( c == '&' ) return 7;
    else if ( c == '!' ) return 8;
    
    return 0;
}

bool BoolRPE::is_num ( char const c ) noexcept
{
    return c == '0' || c == '1';
}

bool BoolRPE::is_number ( char const c ) noexcept
{
    return std::isdigit( static_cast< unsigned char >( c ) );
}

bool BoolRPE::is_var ( char const c ) noexcept
{
    return c == 'X' || c == 'x';
}

bool BoolRPE::is_oper ( char const c ) noexcept
{
    return c == '^' || c == '~' || c == '>' || c == '<' || c == '|' || c == '&';
}

bool BoolRPE::is_unar_oper ( char c ) noexcept
{
    return c == '!';
}

bool BoolRPE::calc ( bool second, char oper, bool first ) const noexcept
{
    if ( oper == '&' ) return first & second;
    else if ( oper == '|' ) return first | second;
    else if ( oper == '<' ) return first & !second;
    else if ( oper == '>' ) return !first | second;
    else if ( oper == '~' ) return first == second;
    else if ( oper == '^' ) return first ^ second;

    return 0;
}

void BoolRPE::setViaTruthTable ()
{
    assert( m_expressions.size() );

    std::size_t tempsize = std::pow( 2, m_expressions.size() );
    
    clearAndReserve( tempsize );
    std::size_t t1 = 0;
    std::size_t t2;

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

void BoolRPE::classification () noexcept
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

void BoolRPE::analyze () // TODO: RegExp
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

bool BoolRPE::is_other ( char const c ) noexcept
{
    return c == '!' || c == '(' || c == ')' || c == '=' || c == ' ';
}

} // namespace bcalc
