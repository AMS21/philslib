/*! \file utility.hpp
 *  \brief Defines various general utilities.
**/

#ifndef INCG_PL_UTILITY_HPP
#define INCG_PL_UTILITY_HPP
#include "macros.hpp" // PL_INOUT, PL_UNUSED
#include <cstdint> // std::uint8_t
#include <initializer_list> // std::initializer_list
#include <utility> // std::forward
namespace pl
{
/*!
 * \brief Toggles the boolean passed in.
 * \param b The boolean to be toggled.
 * \return The new value of the the boolean passed in.
 *
 * If the boolean passed in is true it will be assigned false and the
 * value returned will be false.
 * If the boolean passed in is false it will be assigned true and the
 * value return will be true.
**/
bool toggleBool(PL_INOUT bool &b);

/*!
 * \brief Converts a character to an integer.
 * \param ch The character to be converted.
 * \return Returns a value of [0..9] for a character of the range ['0'..'9'].
 * \warning Do not pass in characters outside of the range ['0'..'9'].
**/
std::uint8_t charToInt(unsigned char ch);

/*!
 * \brief Calls a unary callable with every argument in the template type
 *        parameter pack individually.
 * \param callable The unary callable to be invoked with every argument of
 *        the template type parameter pack.
 * \param args The arguments that callable will be called with. callable will
 *        be called with each and every element of this template type
 *        parameter pack individually one after the other.
 * \example pl::forEachArgument([](const auto &e) { std::cout << e << ' ';},
 *                              1, 2.1, "hello", .3F, 44U, std::string{ "world" });
**/
template <typename Callable, typename ...Args>
void forEachArgument(Callable &&callable, Args &&...args)
{
    PL_UNUSED(
        std::initializer_list<int>{
            (callable(std::forward<Args>(args)), 0)...
        }
    );
}
} // namespace pl
#endif // INCG_PL_UTILITY_HPP
