#ifndef BASE_OUTPUT_MANAGER_HPP
#define BASE_OUTPUT_MANAGER_HPP

#include "base_input_manager.hpp"

namespace bcalc
{

class BaseOutputManager
{
public:
    virtual ~BaseOutputManager () {};

    virtual void output (
            Table const &
        ,   LineOfTable const &
    ) const noexcept = 0;
    virtual void getVariables ( Table const & ) const noexcept = 0;
    virtual void getAnswer ( LineOfTable const & ) const noexcept = 0;
};

} // namespace bcalc

#endif // BASE_OUTPUT_MANAGER_HPP //