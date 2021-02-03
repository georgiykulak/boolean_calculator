#ifndef BASE_INPUT_MANAGER_HPP
#define BASE_INPUT_MANAGER_HPP

#include "type_values.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace bcalc
{

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
    Table m_expressions; // TODO: Shouldn't store data
    std::size_t m_size; // TODO: Shouldn't store data

    void clearAndReserve ( std::size_t const );

    void convertToBinaryVariables ( std::vector< std::size_t > & );

    void findMaxBitWidth ( std::vector< std::size_t > const &, std::size_t & );

    void fillVariablesWithFalse ();

    void convertDecimalsToBinaries ( std::vector< std::size_t > & );
};

} // namespace bcalc

#endif // BASE_INPUT_MANAGER_HPP //