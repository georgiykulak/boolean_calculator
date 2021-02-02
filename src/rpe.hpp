#ifndef RPE_HPP
#define RPE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>

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

    void setViaTruthTable (); // full-cased testing

    void setVariablesDecimals ();

    void setVariablesBinaries ( std::size_t const );

    void setVariablesRandomly ( std::size_t const );

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
    using Boolean = unsigned char;
    using LineOfTable = std::vector< Boolean >;
    using Table = std::vector< LineOfTable >;

    static constexpr Boolean s_false = 0;
    static constexpr Boolean s_true = 1;

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

    void clearAndReserve ( std::size_t const size );
    
    constexpr bool calc ( bool second, char oper, bool first ) const noexcept;
};

} // namespace bcalc

#endif // RPE_HPP
