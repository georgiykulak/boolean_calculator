#ifndef BASE_INPUT_MANAGER_HPP
#define BASE_INPUT_MANAGER_HPP

#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace bcalc
{
using Boolean = unsigned char;
using LineOfTable = std::vector< Boolean >;
using Table = std::vector< LineOfTable >;

static constexpr Boolean s_false = 0;
static constexpr Boolean s_true = 1;

class BaseInputManager
{
public:
    virtual ~BaseInputManager () {};
    virtual void input ( Table &, std::size_t & ) = 0;
    virtual void setVariablesDecimals () = 0;
    virtual void setVariablesBinaries ( std::size_t const ) = 0; // TODO: Do not depend on size as arg, parse string with bools
    
    void setVariablesRandomly ( std::size_t const );

    void setViaTruthTable (); // full-cased testing

protected:
    Table m_expressions; // TODO: Don't store data
    std::size_t m_size; // TODO: Don't store data

    void clearAndReserve ( std::size_t const );

    void convertToBinaryVariables ( std::vector< std::size_t > & );

    void findMaxBitWidth ( std::vector< std::size_t > const &, std::size_t & );

    void fillVariablesWithFalse ();

    void convertDecimalsToBinaries ( std::vector< std::size_t > & );
};

} // namespace bcalc

#endif // BASE_INPUT_MANAGER_HPP //