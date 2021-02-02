#ifndef RPE_HPP
#define RPE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>

namespace bcalc
{

// TODO: Reorder functions in rpe.cpp like there
class BoolRPE
{
public:
    ~BoolRPE () = default;

    BoolRPE () = default;

    BoolRPE ( std::string const & );

    void setExpression ( std::string const & ); // TODO: Upgrade

    void setViaTruthTable (); // full-cased testing

    void setVariablesRandomly ( std::size_t const );

    void setVariablesBinaries ( std::size_t const );

    void setVariablesDecimals ();

    void getVariables () const noexcept;

    void getAnswer () const noexcept;

    void calculateExpression ();

    void classification () noexcept;

    static int prior ( char const c ) noexcept; // TODO: Replace with map

    static bool is_num ( char const c ) noexcept;

    static bool is_number ( char const c ) noexcept;

    static bool is_var ( char const c ) noexcept; // TODO: Use only x variable

    static bool is_oper ( char const c ) noexcept;

    static bool is_unar_oper ( char const c ) noexcept;

    static bool is_other ( char const c ) noexcept;

private:
    using Boolean = unsigned char;
    using LineOfTable = std::vector< Boolean >;
    using Table = std::vector< LineOfTable >;

    static constexpr Boolean s_false = 0;
    static constexpr Boolean s_true = 1;

    // Number of bits in variables ( m )
    std::size_t m_size = 0;

    Table m_expressions; // Should be map
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

    void clearAndReserve ( std::size_t const size );
    
    bool calc ( bool second, char oper, bool first ) const noexcept; // TODO: Replace with map

    void analyze (); // TODO: RegExp
};

} // namespace bcalc

#endif // RPE_HPP
