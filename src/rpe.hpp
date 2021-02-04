#ifndef RPE_HPP
#define RPE_HPP

#include <iostream> // TODO: Remove it
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include "base_input_manager.hpp"
#include "base_output_manager.hpp"

namespace bcalc
{

// Boolean Reverse Polish Notation Interpreter
class BoolRPN
{
public:
    ~BoolRPN () = default;

    BoolRPN () = default;

    BoolRPN ( std::string const & );

    void setExpression ( std::string const & ); // TODO: Upgrade

    bool set ( BaseInputManager & );

    void get ( BaseOutputManager & ) const noexcept;

    Table const & getVariables () const noexcept;
    
    LineOfTable const & getAnswer () const noexcept;

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
    std::array< bool, 5 > m_classes; // TODO: Should be map and removed from class
    bool m_classP = false; // TODO: This too

    void analyze (); // TODO: RegExp

    constexpr bool calc ( bool second, char oper, bool first ) const noexcept;
};

class Classificator
{
public:
    static constexpr std::size_t s_classes = 6;

    using ClassificationDictionary = std::array< bool, s_classes >;
    using Pair = std::pair< std::string_view, std::string_view >;

    static constexpr
    std::array< Pair, s_classes > s_strings =
    {
            Pair{
                    "T0: Zero is NOT constant"
                ,   "T0: Zero is constant"
            }
        ,   Pair{
                    "T1: One is NOT constant"
                ,   "T1: One is constant"
            }
        ,   Pair{
                    "S:  The function is NOT self-dual"
                ,   "S:  The function is self-dual"
            }
        ,   Pair{
                    "M:  The function is NOT monotone"
                ,   "M:  The function is monotone"
            }
        ,   Pair{
                    "L:  The function is NOT linear"
                ,   "L:  The function is linear"
            }
        ,   Pair{
                    "The function is full"
                ,   "The function is NOT full"
            }
    };

    struct Loop
    {
        template< class F, std::size_t I = 0 >
        constexpr static void forEach ( ClassificationDictionary const & dict, F f )
        {
            constexpr auto & pr = s_strings[ I ];

            f( dict[ I ] ? pr.first : pr.second );

            if constexpr ( I + 1 < s_classes )
                Loop::forEach< F, I + 1 >( dict, f );
        }
    };

    ClassificationDictionary const & get (
            Table const & expressions
        ,   LineOfTable const & answers
    ) noexcept
    {
        // TODO: if ( isNotSetViaTruthTable( expressions ) ) return;

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

        return m_classes;
    }

private:
    ClassificationDictionary m_classes;
};

} // namespace bcalc

#endif // RPE_HPP //
