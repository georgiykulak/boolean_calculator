#ifndef RPE_HPP
#define RPE_HPP

#include <iostream> // TODO: Remove it
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>

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

//----------------------------------------------------------------------------//

class BaseInputManager
{
public:
    virtual ~BaseInputManager () {};
    virtual void input ( BoolRPN::Table &, std::size_t & ) = 0;
    virtual void setVariablesDecimals () = 0;
    virtual void setVariablesBinaries ( std::size_t const ) = 0; // TODO: Do not depend on size as arg, parse string with bools
    
    void setVariablesRandomly ( std::size_t const );

    void setViaTruthTable (); // full-cased testing

protected:
    BoolRPN::Table m_expressions; // TODO: Don't store data
    std::size_t m_size; // TODO: Don't store data

    void clearAndReserve ( std::size_t const );

    void convertToBinaryVariables ( std::vector< std::size_t > & );

    void findMaxBitWidth ( std::vector< std::size_t > const &, std::size_t & );

    void fillVariablesWithFalse ();

    void convertDecimalsToBinaries ( std::vector< std::size_t > & );
};

} // namespace bcalc

#endif // RPE_HPP //
