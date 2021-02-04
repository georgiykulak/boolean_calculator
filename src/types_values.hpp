#ifndef TYPES_VALUES_HPP
#define TYPES_VALUES_HPP

#include <vector>

namespace bcalc
{

using Boolean = unsigned char;
using LineOfTable = std::vector< Boolean >;
using Table = std::vector< LineOfTable >;

static constexpr Boolean s_false = 0;
static constexpr Boolean s_true = 1;

} // namespace bcalc

#endif // TYPES_VALUES_HPP //