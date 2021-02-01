#ifndef RPE_HPP
#define RPE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <cassert>
#include "stack.hpp" // TODO: Replace with std::stack or std::vector

class BoolRPE
{
public:
    ~BoolRPE () = default;

    BoolRPE ( std::size_t const vars = 1, std::size_t const size = 1 );

    void setExample ( std::string const & expr );

    void setX (); // FIX

    void setXdec (); // FIX

    void setNumbOfVar ( std::size_t const vars ); // TODO: Remove and use constructor

    void setBitSize ( std::size_t const size ); // TODO: Remove and use constructor

    void randomSetX ();

    void getX () const;

    void getAnswer () const;

    void calculate_expression ();

    void truth_table (); // full-cased testing

    void classification ();

private:
    using Boolean = unsigned char;
    using LineOfTable = std::vector< Boolean >;
    using Table = std::vector< LineOfTable >;

    static constexpr Boolean s_false = 0;
    static constexpr Boolean s_true = 1;

    // Number of variables ( n )
    int m_vars; // TODO: Remove
    // Number of bits in variables ( m )
    std::size_t m_size = 0;

    Table m_expressions;
    /*
        Variable '0'   = a(0),a(1)...a(m-1)
        Variable '1'   = a(0),a(1)...a(m-1)
        ...
        Variable 'n-1' = a(0),a(1)...a(m-1)
    */

    LineOfTable m_answers;
    /*
        Answer = a(0),a(1)...a(m-1)
    */

    std::string m_example;
    std::string m_bool_example;
    std::array< bool, 5 > m_classes; // Should be map
    bool m_classP = false;

    void resize( std::size_t const size );
    int prior ( char const c ) const; // TODO: Replace with map
    bool is_num ( char const c ) const; // TODO: Replase with std function
    bool is_number ( char const c ) const; // And this too
    bool is_var ( char const c ) const; // TODO: Use only x variable
    bool is_oper ( char const c ) const;
    bool is_unar_oper ( char const c ) const;
    bool calc ( bool second, char oper, bool first ) const; // TODO: Replace with map
};



BoolRPE::BoolRPE ( std::size_t const vars, std::size_t const size )
    :   m_expressions( vars )
{
    assert( vars && size );

    for ( auto & expr : m_expressions )
        expr.reserve( size );

    m_answers.reserve( size );

    m_vars = vars; // remove
    m_size = size;
}

void BoolRPE::setExample ( std::string const & expr )
{
    m_example = expr;
}

void BoolRPE::setX () // FIX
{
    bool tmp;
    std::size_t l = 0;

    std::cout << "Enter through the space every variable:\n";
    
    for ( auto & expr : m_expressions )
    {
        std::cout << l + 1 << " variable = ";
        ++l;
        
        for ( auto & elem : expr )
        {
            std::cin >> tmp;
            elem = static_cast< Boolean >( tmp );
        }
    }
}

void BoolRPE::setXdec () // FIX
{
    assert( m_expressions.size() && m_size );

    std::vector< std::size_t > variables( m_expressions.size() );
    std::size_t max = 1;
    std::size_t temp;
    std::size_t counter = 0;
    std::size_t i = 0;
    std::size_t j = 0;
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
        
        if ( counter > max )
            max = counter;
        
        counter = 0;
    }
    
    resize( max );

    for ( auto & expr : m_expressions )
        for ( auto & elem : expr )
            elem = s_false;

    while ( i < m_vars )
    {
        j = m_size - 1;
        
        while ( variables[ i ] > 0 && j >= 0 )
        {
            if ( variables[ i ] % 2 )
            {
                m_expressions[ i ][ j-- ] = 1;
            }
            // No need, because already filled with nulls
            // else m_expressions[ i ][ j-- ] = 0;
            
            variables[ i ] /= 2;
        }

        ++i;
    }
}

void BoolRPE::randomSetX ()
{
    srand( time( NULL ) );

    for ( auto & expr : m_expressions )
        for ( auto & elem : expr )
            elem = static_cast< Boolean >( ( rand() % 17 ) % 2 );
}

void BoolRPE::setNumbOfVar ( std::size_t const vars )
{
    assert( vars );

    m_vars = vars; // remove
    m_expressions.resize( vars );
}

void BoolRPE::setBitSize ( std::size_t const size )
{
    assert( size );
    
    m_size = size;
    
    for ( auto & expr : m_expressions )
        expr.resize( size );
}

void BoolRPE::getX () const
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

void BoolRPE::getAnswer () const
{
    std::cout << "Answer is ";
    for ( auto const & elem : m_answers )
        std::cout << static_cast< bool >( elem );
    std::cout << std::endl;
}

void BoolRPE::calculate_expression ()
{
    Stack< char > stk_c;
    Stack< bool > stk_b;
    std::size_t l = 0;
    std::size_t i = 0;
    unsigned temp = 1;

    while ( l < m_size )
    {
        while ( i < m_example.length() )
        {
            if ( is_num( m_example[ i ] ) )
            {
                m_bool_example += m_example[ i ];
                m_bool_example += ' ';
            }
            else if ( is_oper( m_example[ i ] )
                   || is_unar_oper( m_example[ i ] ) )
            {
                while ( !stk_c.empty()
                    && prior( stk_c.top() ) >= prior( m_example[ i ] ) )
                {
                    m_bool_example += stk_c.pop();
                    m_bool_example += ' ';
                }
                
                stk_c.push( m_example[ i ] );
            }
            // отсчет начинается с 1, вместо "x1" можно писать "x"
            else if ( is_var( m_example[ i ] ) )
            {
                ++i;

                if ( is_number( m_example[ i ]) && m_example[ i ] > '0' )
                {
                    temp -= 1;
                    
                    while ( is_number( m_example[ i ] ) )
                    {
                        temp += ( m_example[ i ] - 48 );
                        temp *= 10;
                        ++i;
                    }

                    temp /= 10;
                }

                m_bool_example += ( m_expressions[ temp - 1 ][ l ] + 48 );
                m_bool_example += ' ';
                temp = 1;
                --i;
            }
            else if ( m_example[ i ] == '(' )
            {
                stk_c.push( m_example[ i ] );
            }
            else if ( m_example[ i ] == ')' )
            {
                while ( stk_c.top() != '(' )
                {
                    m_bool_example += stk_c.pop();
                    m_bool_example += ' ';
                }

                if ( stk_c.top() == '(' )
                    stk_c.pop();
            }

            if ( !stk_c.empty()
                && ( m_example[ i ] == '=' || i + 1 == m_example.length() )
            )
            {
                while ( !stk_c.empty() )
                {
                    m_bool_example += stk_c.pop();
                    m_bool_example += ' ';
                }

                break;
            }

            ++i;
        }

        i = 0;

        while ( i < m_bool_example.length() )
        {
            if ( is_num( m_bool_example[ i ] ) )
            {
                stk_b.push( m_bool_example[ i ] - 48 );
            }
            else if ( is_oper( m_bool_example[ i ] ) )
            {
                stk_b.push(
                    calc( stk_b.pop(), m_bool_example[ i ], stk_b.pop() )
                );
            }
            else if ( is_unar_oper( m_bool_example[ i ] ) )
            {
                stk_b.push( !stk_b.pop() );
            }

            ++i;
        }

        i = 0;
        m_bool_example = "";
        m_answers.push_back( stk_b.pop() );
        ++l;
    }
}

void BoolRPE::resize ( std::size_t const size )
{
    assert( size );

    m_size = size;

    for ( auto & expr : m_expressions )
        expr.resize( m_size );

    m_answers.resize( m_size );
}

int BoolRPE::prior ( char const c ) const
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

bool BoolRPE::is_num ( char const c ) const
{
    return c == '0' || c == '1';
}

bool BoolRPE::is_number ( char const c ) const
{
    return c >= 48 && c <= 57;
}

bool BoolRPE::is_var ( char const c ) const
{
    return ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 );
}

bool BoolRPE::is_oper ( char const c ) const
{
    return c == '^' || c == '~' || c == '>' || c == '<' || c == '|' || c == '&';
}

bool BoolRPE::is_unar_oper ( char c ) const
{
    return c == '!';
}

bool BoolRPE::calc ( bool second, char oper, bool first ) const
{
    if ( oper == '&' ) return first & second;
    else if ( oper == '|' ) return first | second;
    else if ( oper == '<' ) return first & !second;
    else if ( oper == '>' ) return !first | second;
    else if ( oper == '~' ) return first == second;
    else if ( oper == '^' ) return first ^ second;

    return 0;
}

void BoolRPE::truth_table ()
{
    std::size_t tempsize = 1;
    std::size_t t1 = 0;
    std::size_t t2;

    for ( auto & expr : m_expressions )
    {
        expr.clear();
        tempsize *= 2;
    }

    m_size = tempsize;
    
    for ( auto & expr : m_expressions )
        expr.reserve( m_size );

    m_answers.clear();
    m_answers.reserve( m_size );

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

void BoolRPE::classification ()
{
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

#endif // RPE_HPP
