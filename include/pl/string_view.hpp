/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

/*!
 * \file string_view.hpp
 * \brief Exports the StringView type.
**/
#ifndef INCG_PL_STRING_VIEW_HPP
#define INCG_PL_STRING_VIEW_HPP
#include "annotations.hpp" // PL_NODISCARD, PL_IN, PL_OUT, PL_INOUT, PL_NULL_TERMINATED, PL_IMPLICIT
#include "compiler.hpp"          // PL_COMPILER, PL_COMPILER_MSVC
#include "meta/remove_cvref.hpp" // pl::meta::remove_cvref_t
#include <algorithm>             // std::min, std::copy_n
#include <ciso646>               // and, not
#include <cstddef>               // std::size_t, std::ptrdiff_t
#include <iterator>              // std::reverse_iterator
#include <ostream>               // std::basic_ostream
#include <stdexcept>             // std::out_of_range
#include <string>                // std::char_Traits, std::basic_string
#include <type_traits> // std::is_same, std::is_pointer, std::remove_const_t, std::remove_pointer_t

#if PL_COMPILER == PL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4814) // in C++14 'constexpr' will not imply 'const';
                                // consider explicitly specifying 'const'
#endif                          // PL_COMPILER == PL_COMPILER_MSVC
namespace pl {
namespace detail {
template <typename CharT>
struct empty_string;

template <>
struct empty_string<char> {
    static constexpr const char* value = "";
};

template <>
struct empty_string<char16_t> {
    static constexpr const char16_t* value = u"";
};

template <>
struct empty_string<char32_t> {
    static constexpr const char32_t* value = U"";
};

template <>
struct empty_string<wchar_t> {
    static constexpr const wchar_t* value = L"";
};
} // namespace detail

/*!
 * \brief An object that refers to a constant contiguous sequence of char-like
 *        objects with the first element of the sequence at position zero.
 *        CharT shall be the character type to use, possible types are
 *        char, char16_t, char32_t and wchar_t.
 *        Traits shall be the CharTraits class specifying the operations on the
 *        chraracter type. Traits::char_type must be the same type as CharT or
 *        the behaviour is undefined.
 *        Shall always refer to a null-terminated string.
 *        This type is to be used in parameters of functions in order to view
 *        a 'string', it is to be preferred over const std::string& like
 *        parameters in general in order to avoid copying overhead for
 *        C style strings.
**/
template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicStringView {
public:
    using this_type              = BasicStringView;
    using traits_type            = Traits;
    using value_type             = CharT;
    using pointer                = CharT*;
    using const_pointer          = const CharT*;
    using reference              = CharT&;
    using const_reference        = const CharT&;
    using const_iterator         = const_pointer;
    using iterator               = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator       = const_reverse_iterator;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;

    /*!
     * \brief Default constructor. Constructs a string view that points to
     *        an empty string stored in the data section. The string pointed
     *        to contains merely the null-terminator.
     *        The size of the string view after construction will be 0.
    **/
    constexpr BasicStringView() noexcept
        : m_data{detail::empty_string<value_type>::value},
          m_size{static_cast<std::size_t>(0U)}
    {
    }

    /*!
     * \brief Copy constructor. Constructs a view of the same content as
     *        the view passed into the parameter.
    **/
    constexpr BasicStringView(PL_IN const this_type&) noexcept = default;

    /*!
     * \brief Replaces this view with the one passed into the parameter.
     * \return A reference to this object.
     * \note Constant complexity.
    **/
    this_type& operator=(PL_IN const this_type&) noexcept = default;

    /*!
     * \brief Constructs a view of the null-terminated character string pointed
     *        to by 'string'.
     *        The length of the view is determined as if by
     *        Traits::length(string).
     *        After construction, data() is equal to 'string', and size() is
     *        equal to Traits::length(string).
     * \param string Pointer to a valid null-terminated C style string.
     * \warning The behavior is undefined if
     *          [string, string + Traits::length(string)) is not a valid range.
     * \note Complexity is linear in Traits::length(string).
    **/
    template <
        typename Ty,
        typename = std::
            enable_if_t<std::is_pointer<meta::remove_cvref_t<Ty>>::value
                        and std::
                                is_same<std::
                                            remove_const_t<std::
                                                               remove_pointer_t<meta::
                                                                                    remove_cvref_t<Ty>>>,
                                        value_type>::value>>
    PL_IMPLICIT constexpr BasicStringView(
        PL_IN PL_NULL_TERMINATED(Ty&&) string) noexcept
        : m_data{string},
          m_size{traits_type::length(string)}
    {
    }

    /*!
     * \brief Constructs a view of a null-terminated string owned by a
     *        std::basic_string object.
     * \param string The std::basic_string object to view.
     * \note Constant complexity.
     * \warning The std::basic_string must contain a valid null-terminated
     *          string and may not contain embedded null-characters!
    **/
    template <typename Allocator>
    PL_IMPLICIT constexpr BasicStringView(
        PL_IN const std::basic_string<value_type, traits_type, Allocator>&
                    string) noexcept : m_data{string.data()},
                               m_size{string.size()}
    {
    }

    /*!
     * \brief Deleted constructor from a basic_string rvalue. This is added
     *        for safety as constructing a view to a temporary string always
     *        creates a dangling string view.
    **/
    template <typename Allocator>
    PL_IMPLICIT constexpr BasicStringView(
        PL_IN std::basic_string<value_type, traits_type, Allocator>&&) noexcept
        = delete;

    /*!
     * \brief Constructs from a character array.
     * \param array The character array to view.
     * \note Constant complexity.
     * \warning 'array' must be initialized and must contain a valid
     *          null-terminated string. Furthermore 'array' may not contain
     *          any embedded null characters.
    **/
    template <std::size_t Size>
    constexpr BasicStringView(
        PL_IN PL_NULL_TERMINATED(const value_type (&array)[Size])) noexcept
        : m_data{array},
          m_size{Size - 1U}
    {
    }

    /*!
     * \brief Constructs a string view from a pointer to a C style string and
     *        a size.
     * \param string A null-terminated string to view.
     * \param size The length of 'string'. Must be the same as calling
     *             Traits::length(string).
     * \note Constant complexity.
     *       The string must be a null-terminated character string but may not
     *       contain embedded null-characters.
     *       This constructor is provided so that you can provide the size if
     *       you already know it, preventing overhead from calling
     *       Traits::length.
     * \warning The behavior is undefined if ['string', 'string' + 'size') is
     *          not a valid range.
    **/
    constexpr BasicStringView(
        PL_IN     PL_NULL_TERMINATED(const_pointer) string,
        size_type size) noexcept : m_data{string},
                                   m_size{size}
    {
    }

    /*!
     * \brief Returns an iterator to the first character of the view.
     * \return const_iterator to the first character
     * \note Constant complexity.
    **/
    constexpr const_iterator begin() const noexcept { return cbegin(); }
    /*!
     * \brief Returns an iterator to the first character of the view.
     * \return const_iterator to the first character
     * \note Constant complexity.
    **/
    constexpr const_iterator cbegin() const noexcept { return data(); }
    /*!
     * \brief Returns an iterator to the character following the last character
     *        of the view.
     * \return const_iterator to the character following the last character.
     * \note Constant complexity.
     * \warning The iterator returned acts as a placeholder, attempting to
     *          indirect through it results in undefined behavior.
    **/
    constexpr const_iterator end() const noexcept { return cend(); }
    /*!
     * \brief Returns an iterator to the character following the last character
     *        of the view.
     * \return const_iterator to the character following the last character.
     * \note Constant complexity.
     * \warning The iterator returned acts as a placeholder, attempting to
     *          indirect through it results in undefined behavior.
    **/
    constexpr const_iterator cend() const noexcept { return cbegin() + size(); }
    /*!
     * \brief Returns a reverse iterator to the first character of the reversed
     *        view. It corresponds to the last character of the non-reversed
     *        view.
     * \return const_reverse_iterator to the first character
     * \note Constant complexity.
    **/
    const_reverse_iterator rbegin() const noexcept { return crbegin(); }
    /*!
     * \brief Returns a reverse iterator to the first character of the reversed
     *        view. It corresponds to the last character of the non-reversed
     *        view.
     * \return const_reverse_iterator to the first character
     * \note Constant complexity.
    **/
    const_reverse_iterator crbegin() const noexcept
    {
        return reverse_iterator{cend()};
    }

    /*!
     * \brief Returns a reverse iterator to the character following the last
     *        character of the reversed view. It corresponds to the character
     *        preceding the first character of the non-reversed view.
     * \return const_reverse_iterator to the character following the last
     *         character.
     * \note Constant complexity.
     * \warning The iterator returned character acts as a
     *          placeholder, attempting to indirect through it results
     *          in undefined behavior.
    **/
    const_reverse_iterator rend() const noexcept { return crend(); }
    /*!
     * \brief Returns a reverse iterator to the character following the last
     *        character of the reversed view. It corresponds to the character
     *        preceding the first character of the non-reversed view.
     * \return const_reverse_iterator to the character following the last
     *         character.
     * \note Constant complexity.
     * \warning The iterator returned character acts as a
     *          placeholder, attempting to indirect through it results
     *          in undefined behavior.
    **/
    const_reverse_iterator crend() const noexcept
    {
        return reverse_iterator{cbegin()};
    }

    /*!
     * \brief Returns the number of CharT elements in the view,
     *        i.e. std::distance(begin(), end()).
     * \return The number of CharT elements in the view.
     * \note Constant complexity.
    **/
    constexpr size_type size() const noexcept { return m_size; }
    /*!
     * \brief Checks if the view has no characters, i.e. whether size() == 0.
     * \return true if the view is empty, false otherwise.
     * \note Constant complexity.
    **/
    PL_NODISCARD constexpr bool empty() const noexcept
    {
        return size() == static_cast<std::size_t>(0U);
    }

    /*!
     * \brief Returns a const reference to the character at specified location
     *        'position'.
     * \param position The position of the character to return
     * \return Const reference to the requested character
     * \note Constant complexity.
     *       operator[](size()) returns CharT() as all the strings viewed
     *       have to be null-terminated.
     * \warning No bounds checking is performed: the behavior is undefined
     *          if 'position' > size().
    **/
    constexpr const_reference operator[](size_type position) const noexcept
    {
        return data()[position];
    }

    /*!
     * \brief Returns a reference to the character at specified location
     *        'position'. Bounds checking is performed, exception of type
     *        std::out_of_range will be thrown on invalid access.
     * \param position The position of the character to return
     * \return Const reference to the requested character.
     * \throws std::out_of_range if 'position' > size().
     * \note Constant complexity.
     *       at(size()) returns CharT() as all strings viewed have to be
     *       null-terminated.
    **/
    constexpr const_reference at(size_type position) const
    {
        if (position > size()) {
            throw std::out_of_range{
                "BasicStringView::at position was out of bounds."};
        }

        return operator[](position);
    }

    /*!
     * \brief Returns reference to the first character in the view.
     * \return reference to the first character, equivalent to operator[](0)
     * \note Constant complexity.
     *       Returns CharT() if empty() == true as all strings viewed have to
     *       be null-terminated.
    **/
    constexpr const_reference front() const noexcept { return operator[](0U); }
    /*!
     * \brief Returns reference to the last character in the view.
     * \return Reference to the last character, equivalent to
     *         operator[](size() - 1).
     * \note Constant complexity.
     * \warning The behavior is undefined if empty() == true.
    **/
    constexpr const_reference back() const noexcept
    {
        return operator[](size() - 1U);
    }

    /*!
     * \brief Returns a pointer to the underlying character array. The pointer
     *        is such that the range [data(); data() + size()] is valid and the
     *        values in it correspond to the values of the view.
     * \return A pointer to the underlying character array.
     * \note Constant complexity.
     *       Returns a pointer to a null-terminated string as all strings
     *       viewed have to be null-terminated.
    **/
    constexpr const_pointer data() const noexcept { return m_data; }
    /*!
     * \brief Moves the start of the view forward by 'charactersToRemove'
     *        characters.
     * \param charactersToRemove number of characters to remove from the start
     *                           of the view
     * \note Constant complexity.
     *       if 'charactersToRemove' > size() all characters are removed from
     *       the view, leaving it pointing to an empty string.
    **/
    constexpr void remove_prefix(size_type charactersToRemove) noexcept
    {
        if (charactersToRemove > size()) {
            charactersToRemove = size();
        }

        m_data += charactersToRemove;
        m_size -= charactersToRemove;
    }

    /*!
     * \brief Exchanges the view with that of 'other'.
     * \param other view to swap with
     * \note Constant complexity.
    **/
    constexpr void swap(PL_INOUT this_type& other) noexcept
    {
        const const_pointer pointer = m_data;
        m_data                      = other.m_data;
        other.m_data                = pointer;
        const size_type size        = m_size;
        m_size                      = other.m_size;
        other.m_size                = size;
    }

    /*!
     * \brief Convenience function to create a corresponding std::basic_string
     *        object for this view.
     * \param allocator The allocator to use for the std::basic_string.
     * \return The resulting std::basic_string object.
    **/
    template <typename Allocator = std::allocator<CharT>>
    std::basic_string<value_type, traits_type, Allocator> to_string(
        PL_IN const Allocator& allocator = Allocator{}) const
    {
        return std::basic_string<value_type, traits_type, Allocator>(
            begin(), end(), allocator);
    }

    /*!
     * \brief Compares this string view with another one.
     * \param other The other string view to compare to.
     * \return A value < 0 if this string view is considered less than
     *         'other'.
     *         0 if this string view is considered equal to 'other'.
     *         A value > 0 if this string view is considered greate than
     *         'other'.
    **/
    constexpr int compare(this_type other) const noexcept
    {
        const size_type length{std::min(size(), other.size())};
        int result{traits_type::compare(data(), other.data(), length)};

        if (result == 0) {
            result
                = size() == other.size() ? 0 : (size() < other.size() ? -1 : 1);
        }

        return result;
    }

    /*!
     * \brief Compares this string view to a null-terminated character
     *        string.
     * \param other The null-terminated character string to compare to.
     * \return A value < 0 if this string view is considered less than
     *         'other'.
     *         0 if this string view is considered equal to 'other'.
     *         A value > 0 if this string view is considered greate than
     *         'other'.
    **/
    constexpr int compare(PL_IN PL_NULL_TERMINATED(const_pointer) string) const
        noexcept
    {
        return compare(this_type{string});
    }

    /*!
     * \brief Checks if the string view begins with the given prefix.
     * \param character A single character to compare to the start of the string
     *                  view
     * \return true if the string view begins with the provided prefix, false
     *         otherwise.
    **/
    constexpr bool starts_with(value_type character) const noexcept
    {
        return not empty() and traits_type::eq(character, front());
    }

    /*!
     * \brief Checks if the string view begins with the given prefix.
     * \param stringView A character sequence to compare to the start of the
     *                   string view
     * \return true if the string view begins with the provided prefix, false
     *         otherwise.
    **/
    constexpr bool starts_with(this_type stringView) const noexcept
    {
        return size() >= stringView.size()
               and traits_type::compare(
                       data(), stringView.data(), stringView.size())
                       == 0;
    }

    /*!
     * \brief Checks if the string view ends with the given suffix.
     * \param character A single character to compare to the end of the
     *                  string view
     * \return true if the string view ends with the provided suffix,
     *         false otherwise.
    **/
    constexpr bool ends_with(value_type character) const noexcept
    {
        return not empty() and traits_type::eq(character, back());
    }

    /*!
     * \brief Checks if the string view ends with the given suffix.
     * \param stringView A character sequence to compare to the end of the
     *                   string view
     * \return true if the string view ends with the provided suffix,
     *         false otherwise.
    **/
    constexpr bool ends_with(this_type stringView) const noexcept
    {
        return size() >= stringView.size()
               and traits_type::compare(
                       data() + size() - stringView.size(),
                       stringView.data(),
                       stringView.size())
                       == 0;
    }

private:
    const_pointer m_data;
    size_type     m_size;
};

template <typename CharT, typename Traits>
constexpr void swap(
    PL_INOUT BasicStringView<CharT, Traits>& first,
    PL_INOUT BasicStringView<CharT, Traits>& second) noexcept
{
    first.swap(second);
}

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    PL_INOUT std::basic_ostream<CharT, Traits>& os,
    BasicStringView<CharT, Traits>              string)
{
    return os.write(string.data(), string.size());
}

template <typename CharT, typename Traits>
constexpr bool operator==(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return x.compare(y) == 0;
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator==(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x == BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator==(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} == y;
}

template <typename CharT, typename Traits>
constexpr bool operator==(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x == BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
constexpr bool operator==(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} == y;
}

template <typename CharT, typename Traits>
constexpr bool operator!=(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return not(x == y);
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator!=(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x != BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator!=(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} != y;
}

template <typename CharT, typename Traits>
constexpr bool operator!=(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x != BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
constexpr bool operator!=(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} != y;
}

template <typename CharT, typename Traits>
constexpr bool operator<(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return x.compare(y) < 0;
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator<(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x < BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator<(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} < y;
}

template <typename CharT, typename Traits>
constexpr bool operator<(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x < BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
inline bool operator<(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} < y;
}

template <typename CharT, typename Traits>
constexpr bool operator>(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return x.compare(y) > 0;
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator>(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x > BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator>(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} > y;
}

template <typename CharT, typename Traits>
constexpr bool operator>(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x > BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
constexpr bool operator>(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} > y;
}

template <typename CharT, typename Traits>
constexpr bool operator<=(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return x.compare(y) <= 0;
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator<=(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x <= BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator<=(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} <= y;
}

template <typename CharT, typename Traits>
constexpr bool operator<=(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x <= BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
constexpr bool operator<=(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} <= y;
}

template <typename CharT, typename Traits>
constexpr bool operator>=(
    BasicStringView<CharT, Traits> x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return x.compare(y) >= 0;
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator>=(
    BasicStringView<CharT, Traits> x,
    PL_IN const std::basic_string<CharT, Traits, Allocator>& y) noexcept
{
    return x >= BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits, typename Allocator>
constexpr bool operator>=(
    PL_IN const std::basic_string<CharT, Traits, Allocator>& x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} >= y;
}

template <typename CharT, typename Traits>
constexpr bool operator>=(
    BasicStringView<CharT, Traits> x,
    PL_IN PL_NULL_TERMINATED(const CharT*) y) noexcept
{
    return x >= BasicStringView<CharT, Traits>{y};
}

template <typename CharT, typename Traits>
constexpr bool operator>=(
    PL_IN PL_NULL_TERMINATED(const CharT*) x,
    BasicStringView<CharT, Traits> y) noexcept
{
    return BasicStringView<CharT, Traits>{x} >= y;
}

using StringView    = BasicStringView<char>;
using U16StringView = BasicStringView<char16_t>;
using U32StringView = BasicStringView<char32_t>;
using WStringView   = BasicStringView<wchar_t>;

inline namespace literals {
inline namespace string_view_literals {
constexpr StringView operator""_sv(
    PL_IN       PL_NULL_TERMINATED(const char*) string,
    std::size_t size) noexcept
{
    return StringView{string, size};
}

constexpr U16StringView operator""_sv(
    PL_IN       PL_NULL_TERMINATED(const char16_t*) string,
    std::size_t size) noexcept
{
    return U16StringView{string, size};
}

constexpr U32StringView operator""_sv(
    PL_IN       PL_NULL_TERMINATED(const char32_t*) string,
    std::size_t size) noexcept
{
    return U32StringView{string, size};
}

constexpr WStringView operator""_sv(
    PL_IN       PL_NULL_TERMINATED(const wchar_t*) string,
    std::size_t size) noexcept
{
    return WStringView{string, size};
}
} // inline namespace string_view_literals
} // inline namespace literals
} // namespace pl
#if PL_COMPILER == PL_COMPILER_MSVC
#pragma warning(pop)
#endif // PL_COMPILER == PL_COMPILER_MSVC
#endif // INCG_PL_STRING_VIEW_HPP
