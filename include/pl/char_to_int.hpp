/*!
 * \file char_to_int.hpp
 * \brief Exports a function to convert characters to integers.
**/
#ifndef INCG_PL_CHAR_TO_INT_HPP
#define INCG_PL_CHAR_TO_INT_HPP
#include <cstdint> // std::uint8_t

namespace pl
{
/*!
 * \brief Converts a character to an integer.
 * \param ch The character to be converted.
 * \return Returns a value of [0..9] for a character of the range ['0'..'9'].
 * \warning Do not pass in characters outside of the range ['0'..'9'].
**/
inline std::uint8_t charToInt(unsigned char ch)
{
    static constexpr unsigned char subtractThis = '0';
    return static_cast<std::uint8_t>(ch - subtractThis);
}
} // namespace pl
#endif // INCG_PL_CHAR_TO_INT_HPP