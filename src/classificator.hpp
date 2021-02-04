#ifndef CLASSIFICATOR_HPP
#define CLASSIFICATOR_HPP

#include "types_values.hpp"

#include <array>
#include <string_view>
#include <cassert>

namespace bcalc
{

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
    ) noexcept;

private:
    ClassificationDictionary m_classes;
};

} // namespace bcalc

#endif // CLASSIFICATOR_HPP //