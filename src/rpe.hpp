#ifndef RPE_HPP
#define RPE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>

namespace bcalc
{

class BaseInputManager;

// Boolean Reverse Polish Notation Interpreter
class BoolRPN
{
public:
    using Boolean = unsigned char;
    using LineOfTable = std::vector< Boolean >;
    using Table = std::vector< LineOfTable >;

    static constexpr Boolean s_false = 0;
    static constexpr Boolean s_true = 1;

    ~BoolRPN () = default;

    BoolRPN () = default;

    BoolRPN ( std::string const & );

    void setExpression ( std::string const & ); // TODO: Upgrade

    void set ( BaseInputManager & );

    // void get ( BaseInputManager & ) const noexcept;

    void getVariables () const noexcept;
    
    void getAnswer () const noexcept;

    void getClassification () noexcept;

    void calculateExpression ();

    constexpr static int prior ( char const c ) noexcept;

    constexpr static bool is_num ( char const c ) noexcept;

    constexpr static bool is_number ( char const c ) noexcept;

    constexpr static bool is_var ( char const c ) noexcept;

    constexpr static bool is_oper ( char const c ) noexcept;

    constexpr static bool is_unar_oper ( char const c ) noexcept;

    constexpr static bool is_other ( char const c ) noexcept;

private:
    // TODO: Change the order of members to make class more compact

    // Number of bits in variables ( m )
    std::size_t m_size = 0;

    Table m_expressions; // Should be map of vectors
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

    std::string m_formula;
    std::array< bool, 5 > m_classes; // Should be map and removed from class
    bool m_classP = false; // This too

    void analyze (); // TODO: RegExp

    constexpr bool calc ( bool second, char oper, bool first ) const noexcept;
};

class BaseInputManager
{
public:
    virtual ~BaseInputManager () {};
    virtual void input ( BoolRPN::Table &, std::size_t & ) = 0;
    virtual void setVariablesDecimals () = 0;
    virtual void setVariablesBinaries ( std::size_t const ) = 0; // TODO: Do not depend on size as arg, parse string with bools
    
    void setVariablesRandomly ( std::size_t const size )
    {
        assert( m_expressions.size() );

        srand( time( NULL ) );

        clearAndReserve( size );

        for ( auto & expr : m_expressions )
            for ( std::size_t i = 0; i < m_size; ++i )
                expr.push_back(
                        static_cast< BoolRPN::Boolean >( ( rand() % 17 ) % 2 )
                );
    }

    void setViaTruthTable () // full-cased testing
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
                    expr.push_back( BoolRPN::s_false );
                
                t1 += tempsize;
                t2 += tempsize;
                
                for ( std::size_t j = t1; j < t2; ++j )
                    expr.push_back( BoolRPN::s_true );
                
                t1 += tempsize;
                t2 += tempsize;
            }

            t1 = 0;
        }
    }

protected:
    BoolRPN::Table m_expressions; // TODO: Don't store data
    std::size_t m_size; // TODO: Don't store data

    void clearAndReserve ( std::size_t const size )
    {
        assert( size );

        m_size = size;

        for ( auto & expr : m_expressions )
        {
            BoolRPN::LineOfTable().swap( expr );
            expr.reserve( m_size );
        }
    }
};

// InputManager for CLI
class InputManagerCLI final : public BaseInputManager
{
public:
    void input ( BoolRPN::Table & expressions, std::size_t & size )
    {
        assert( expressions.size() );

        m_expressions = std::move( expressions );
        m_size = size;
        size = 0;

        char answer;

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

                /*std::cout << "Do you want to see classification of boolean function? (y/n) ";
                std::cin >> answer;
                
                if ( std::toupper( answer ) == 'Y' )
                    flag = true;*/
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
    }

    void setVariablesDecimals () override
    {
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
                expr.push_back( BoolRPN::s_false );

        auto variableSize = m_expressions.size();
        while ( i < variableSize )
        {
            j = m_size - 1;

            while ( variables[ i ] && j >= 0 )
            {
                if ( variables[ i ] % 2 )
                    m_expressions[ i ][ j ] = BoolRPN::s_true;
                
                --j;
                variables[ i ] /= 2;
            }

            ++i;
        }
    }

    void setVariablesBinaries ( std::size_t const size ) override
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
                expr.push_back( static_cast< BoolRPN::Boolean >( tmp ) );
            }
        }
    }
};

} // namespace bcalc

#endif // RPE_HPP
