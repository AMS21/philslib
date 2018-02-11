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
 * \file asprintf.hpp
 * \brief Exports asprintf functions.
**/
#ifndef INCG_PL_ASPRINTF_HPP
#define INCG_PL_ASPRINTF_HPP
#include "annotations.hpp" // PL_OUT, PL_IN, PL_FMT_STR, PL_NODISCARD, ...
#include <cstdarg> // std::va_list
#include <string> // std::string
#include <memory> // std::unique_ptr

namespace pl
{
/*!
 * \brief Allocates a string and prints to it in a printf style.
 * \param strp Pointer to a unique_ptr<char[]>. The unique_ptr<char[]> pointed
 *             to will be replaced with a new unique_ptr<char[]> that will
 *             own the newly allocated memory which will contain the resulting
 *             string. May not be nullptr!
 * \param fmt A null-terminated byte character string to be used as the
 *            printf style format string. May not be nullptr!
 * \param ap The va_list that contains the arguments as specified by 'fmt'.
 * \return The number of bytes written (not including the null-terminator)
 *         on success or a negative number on error.
 * \warning Will cause undefined behavior if 'strp' or 'fmt' is nullptr.
 * \note In general you probably want to be using asprintf instead.
**/
PL_NODISCARD int vasprintf(
    PL_OUT std::unique_ptr<char[]> *strp,
    PL_IN PL_FMT_STR(const char *)fmt,
    const std::va_list ap) noexcept PL_PRINTF_FUNCTION(2, 0);

/*!
 * \brief Creates a string and prints to it in a printf style.
 * \param strp Pointer to a string. The string pointed to will be resized
 *             to be large enough and be filled with the resulting string.
 *             May not be nullptr!
 * \param fmt A null-terminated byte character string to be used as the
 *            printf style format string. May not be nullptr!
 * \param ap The va_list that contains the arguments as specified by 'fmt'.
 * \return The number of bytes written (not including the null-terminator)
 *         on success or a negative number on error.
 * \warning Will cause undefined behavior if 'strp' or 'fmt' is nullptr.
 * \note In general you probably want to be using asprintf instead.
**/
PL_NODISCARD int vasprintf(
    PL_OUT std::string *strp,
    PL_IN PL_FMT_STR(const char *)fmt,
    const std::va_list ap) noexcept PL_PRINTF_FUNCTION(2, 0);

/*!
 * \brief Allocates a string and prints to it in a printf style.
 * \param strp Pointer to a unique_ptr<char[]>. The unique_ptr<char[]> will be
 *             replaced with a newly created unique_ptr<char[]> that owns
 *             newly allocated memory containing the resulting string.
 *             May not be nullptr!
 * \param fmt A null-terminated byte character string to be used as the printf
 *            style format string. May not be nullptr!
 * \return The number of bytes written (not including the null-terminator)
 *         on success or a negative number on error.
 * \warning Will cause undefined behavior if 'strp' or 'fmt' is nullptr.
**/
PL_NODISCARD int asprintf(
    PL_OUT std::unique_ptr<char[]> *strp,
    PL_IN PL_FMT_STR(const char *)fmt,
    ...) noexcept PL_PRINTF_FUNCTION(2, 3);

/*!
 * \brief Creates a string and prints to it in a printf style.
 * \param strp Pointer to a string. The string will be resized to be large
 *             enough and will be made to contain the resulting string.
 *             May not be nullptr!
 * \param fmt A null-terminated byte character string to be used as the printf
 *            style format string. May not be nullptr!
 * \return The number of bytes written (not including the null-terminator)
 *         on success or a negative number on error.
 * \warning Will cause undefined behavior if 'strp' or 'fmt' is nullptr.
**/
PL_NODISCARD int asprintf(
    PL_OUT std::string *strp,
    PL_IN PL_FMT_STR(const char *)fmt,
    ...) noexcept PL_PRINTF_FUNCTION(2, 3);
} // namespace pl
#endif // INCG_PL_ASPRINTF_HPP
