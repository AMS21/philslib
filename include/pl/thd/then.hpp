/*!
 * \file then.hpp
 * \brief Exports the then function template that can be used to
 *        continue a future.
**/
#ifndef INCG_PL_THD_THEN_HPP
#define INCG_PL_THD_THEN_HPP
#include "../annotations.hpp" // PL_INOUT, PL_IN
#include "../invoke.hpp" // pl::invoke
#include <future> // std::future, std::async

namespace pl
{
namespace thd
{
namespace detail
{
/*!
 * \brief Implementation function of then handling the non-void case.
 *        Not to be used directly.
**/
template <typename Ty, typename Continuation>
auto thenImpl(
    PL_INOUT std::future<Ty> &future,
    PL_IN Continuation &continuation)
    -> decltype(auto)
{
    return ::pl::invoke(continuation, future.get());
}

/*!
 * \brief Implementation function of then handling the void case.
 *        Not to be used directly.
**/
template <typename Continuation>
auto thenImpl(
    PL_INOUT std::future<void> &future,
    PL_IN Continuation &continuation)
    -> decltype(auto)
{
    future.wait();
    return ::pl::invoke(continuation);
}
} // namespace detail

/*!
 * \brief Continues a future with a continuation.
 * \param future The future to continue. Only futures that did not have their
 *               .get() or .wait() member functions called may be passed in!
 * \param continuation The continuation to use. Must be a callable that takes
 *                     a value of the type that the future will hold.
 * \return A future for the result of the continuation.
 *
 * Will launch a new thread that will wait for the future passed into the
 * parameter to be ready. As soon as the future passed into the parameter is
 * ready the newly launched thread will fetch that future's value and invoke
 * the continuation passing in the value returned by that future.
**/
template <typename Ty, typename Continuation>
auto then(std::future<Ty> future, Continuation continuation)
    -> decltype(auto)
{
    return std::async(
        std::launch::async,
        [](std::future<Ty> fut, Continuation cont) {
            return detail::thenImpl(fut, cont);
        },
        std::move(future),
        std::move(continuation));
}
} // namespace thd
} // namespace pl
#endif // INCG_PL_THD_THEN_HPP
