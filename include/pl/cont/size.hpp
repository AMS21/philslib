/*!
 * \file size.hpp
 * \brief Exports the size function that return the size of a container or
 *        C-style array.
**/
#ifndef INCG_PL_CONT_SIZE_HPP
#define INCG_PL_CONT_SIZE_HPP
#include "../macros.hpp" // PL_IN, PL_UNUSED
#include <cstddef> // std::size_t

namespace pl
{
namespace cont
{
/*!
 * \brief Gets the size of a container, which is the amount of elements
 *        currently stored in that container.
 * \param cont The container to get the size of.
 * \return The size of the container passed into the parameter.
**/
template <typename Cont>
auto size(PL_IN Cont &cont) noexcept -> decltype(auto)
{
    return cont.size();
}

/*!
 * \brief Gets the size of a C-Array.
 * \param arr The C-Array to get the size of.
 * \return The size of the C-Array passed into the parameter is returned.
**/
template <typename Type, std::size_t Size>
std::size_t size(PL_IN Type (&arr)[Size]) noexcept
{
    PL_UNUSED(arr);
    return Size;
}
} // namespace cont
} // namespace pl
#endif // INCG_PL_CONT_SIZE_HPP
