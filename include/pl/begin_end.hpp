/*!
 * \file begin_end.hpp
 * \brief Exports functions and macros to fetch begin and end iterators
 *        as well as reverse_iterators.
**/
#ifndef INCG_PL_BEGIN_END_HPP
#define INCG_PL_BEGIN_END_HPP
#include "annotations.hpp" // PL_IN
#include <cstddef> // std::size_t
#include <iterator> // std::reverse_iterator

/*!
 * \def PL_BEGIN_END(cont)
 * \brief Macro that can be used to call a function that expects begin and
 *        end iterators to cont.
**/

/*!
 * \def PL_CBEGIN_CEND(cont)
 * \brief Macro that can be used to call a function that expects cbegin and
 *        cend iterators to cont.
**/

/*!
 * \def PL_RBEGIN_REND(cont)
 * \brief Macro that can be used to call a function that expects rbegin and
 *        rend iterators to cont.
**/

/*!
 * \def PL_CRBEGIN_CREND(cont)
 * \brief Macro that can be used to call a function that expects crbegin and
 *        crend iterators to cont.
**/

namespace pl
{
/*!
 * \brief Returns an iterator to the beginning of the container.
 * \param container The container for which to get an iterator to its
 *        beginning.
 * \return An iterator to the beginning of container.
**/
template <typename Container>
constexpr auto begin(PL_IN Container &container) noexcept -> decltype(auto)
{
    return container.begin();
}

/*!
 * \brief Returns a const_iterator to the beginning of the container.
 * \param container The container for which to get a const_iterator to its
 *        beginning.
 * \return A const_iterator to the beginning of container.
**/
template <typename Container>
constexpr auto begin(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return container.begin();
}

/*!
 * \brief Returns an iterator to the beginning of the array.
 * \param array The array for which to get an iterator to its beginning.
 * \return An iterator to the beginning of array.
**/
template <typename Type, std::size_t Size>
constexpr Type *begin(PL_IN Type (&array)[Size]) noexcept
{
    return array;
}

/*!
 * \brief Returns a const_iterator to the beginning of the array.
 * \param array The array for which to get a const_iterator to its beginning.
 * \return A const_iterator to the beginning of the array.
**/
template <typename Type, std::size_t Size>
constexpr const Type *begin(PL_IN const Type (&array)[Size]) noexcept
{
    return array;
}

/*!
 * \brief Returns a const_iterator to the beginning of the container.
 * \param container The container for which to get a const_iterator to its
 *        beginning.
 * \return A const_iterator to the beginning of the container.
**/
template <typename Container>
constexpr auto cbegin(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return ::pl::begin(container);
}

/*!
 * \brief Returns a const_iterator to the beginning of the array
 * \param array The array for which to get a const_iterator to its beginning.
 * \return A const_iterator to the beginning of the array.
**/
template <typename Type, std::size_t Size>
constexpr auto cbegin(PL_IN const Type (&array)[Size]) noexcept -> decltype(auto)
{
    return ::pl::begin(array);
}

/*!
 * \brief Returns the end iterator of a container.
 * \param container The container to get the end iterator for.
 * \return The end iterator of container.
**/
template <typename Container>
constexpr auto end(PL_IN Container &container) noexcept -> decltype(auto)
{
    return container.end();
}

/*!
 * \brief Returns the end const_iterator of a container.
 * \param container The container to get the end const_iterator for.
 * \return The end cont_iterator of container.
**/
template <typename Container>
constexpr auto end(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return container.end();
}

/*!
 * \brief Returns the end iterator of an array.
 * \param array The array to get the end iterator for.
 * \return The end iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr Type *end(PL_IN Type (&array)[Size]) noexcept
{
    return ::pl::begin(array) + Size;
}

/*!
 * \brief Returns the end const_iterator of an array.
 * \param array The array to get the end const_iterator for.
 * \return The end const_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr const Type *end(PL_IN const Type (&array)[Size]) noexcept
{
    return ::pl::begin(array) + Size;
}

/*!
 * \brief Returns the end const_iterator of a container.
 * \param container The container to get the end const_iterator for.
 * \return The end const_iterator of container.
**/
template <typename Container>
constexpr auto cend(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return ::pl::end(container);
}

/*!
 * \brief Returns the end const_iterator of an array.
 * \param array The array to get the end const_iterator for.
 * \return The end const_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr auto cend(PL_IN const Type (&array)[Size]) noexcept -> decltype(auto)
{
    return ::pl::end(array);
}

/*!
 * \brief Returns the begin reverse_iterator of a container.
 * \param container The container to get the the begin reverse_iterator for.
 * \return The begin reverse_iterator of container.
**/
template <typename Container>
constexpr auto rbegin(PL_IN Container &container) noexcept -> decltype(auto)
{
    return container.rbegin();
}

/*!
 * \brief Returns the begin const_reverse_iterator of a container.
 * \param container The containere to get the begin const_reverse_iterator for.
 * \return The begin const_reverse_iterator of container.
**/
template <typename Container>
constexpr auto rbegin(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return container.rbegin();
}

/*!
 * \brief Returns the begin reverse_iterator of an array.
 * \param array The array to get the begin reverse_iterator for.
 * \return The begin reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr std::reverse_iterator<Type *> rbegin(PL_IN Type (&array)[Size]) noexcept
{
    return std::reverse_iterator<Type *>{ array + Size };
}

/*!
 * \brief Returns the begin const_reverse_iterator of an array.
 * \param array The array to get the begin const_reverse_iterator for.
 * \return The begin const_reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr std::reverse_iterator<const Type *> rbegin(PL_IN const Type (&array)[Size]) noexcept
{
    return std::reverse_iterator<const Type *>{ array + Size };
}

/*!
 * \brief Returns the begin const_reverse_iterator of a container.
 * \param container THe container to get the begin const_reverse_iterator for.
 * \return The begin const_reverse_iterator of container.
**/
template <typename Container>
constexpr auto crbegin(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return ::pl::rbegin(container);
}

/*!
 * \brief Returns the begin const_reverse_iterator of an array.
 * \param array The array to get the begin const_reverse_iterator for.
 * \return The begin const_reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr auto crbegin(PL_IN const Type (&array)[Size]) noexcept
{
    return ::pl::rbegin(array);
}

/*!
 * \brief Returns the end reverse_iterator of a container.
 * \param container The container to get the end reverse_iterator for.
 * \return The end reverse_iterator of container.
**/
template <typename Container>
constexpr auto rend(PL_IN Container &container) noexcept -> decltype(auto)
{
    return container.rend();
}

/*!
 * \brief Returns the end const_reverse_iterator of a container.
 * \param container The container to get the end const_reverse_iterator for.
 * \return The end const_reverse_iterator of container.
**/
template <typename Container>
constexpr auto rend(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return container.rend();
}

/*!
 * \brief Returns the end reverse_iterator of an array.
 * \param array The array to get the end reverse_iterator for.
 * \return The end reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr std::reverse_iterator<Type *> rend(PL_IN Type (&array)[Size]) noexcept
{
    return std::reverse_iterator<Type *>{ array };
}

/*!
 * \brief Returns the end const_reverse_iterator of an array.
 * \param array The array to get the end const_reverse_iterator for.
 * \return The end const_reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr std::reverse_iterator<const Type *> rend(PL_IN const Type (&array)[Size]) noexcept
{
    return std::reverse_iterator<const Type *>{ array };
}

/*!
 * \brief Returns the end const_reverse_iterator of a container.
 * \param container The container to get the end const_reverse_iterator for.
 * \return The end const_reverse_iterator of container.
**/
template <typename Container>
constexpr auto crend(PL_IN const Container &container) noexcept -> decltype(auto)
{
    return ::pl::rend(container);
}

/*!
 * \brief Returns the end const_reverse_iterator of an array.
 * \param array The array to get the end const_reverse_iterator for.
 * \return The end const_reverse_iterator of array.
**/
template <typename Type, std::size_t Size>
constexpr auto crend(PL_IN const Type (&array)[Size]) noexcept -> decltype(auto)
{
    return ::pl::rend(array);
}
} // namespace pl

#define PL_BEGIN_END(cont) ::pl::begin(cont), ::pl::end(cont)
#define PL_CBEGIN_CEND(cont) ::pl::cbegin(cont), ::pl::cend(cont)
#define PL_RBEGIN_REND(cont) ::pl::rbegin(cont), ::pl::rend(cont)
#define PL_CRBEGIN_CREND(cont) ::pl::crbegin(cont), ::pl::crend(cont)
#endif // INCG_PL_BEGIN_END_HPP
