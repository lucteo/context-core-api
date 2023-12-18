#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief A handle for a context.
///
/// Represents the state of control flow. One can suspend the control flow at various points, and a
/// context handle is returned. The context handle can then be used to resume execution the control
/// flow.
///
/// In a simplified view, it represents the registers used for program execution.
///
/// @see context_core_api_make_fcontext(), context_core_api_transfer_t
typedef void* context_core_api_fcontext_t;

/// @brief Structure used to transfer control between stackfull coroutines
///
/// This is used to pass the **parent** context and additional data to the new execution context.
/// Ideally, after the execution of the new code, the context represented by `fctx` shall be
/// resumed.
///
/// @sa context_core_api_fcontext_t, context_core_api_make_fcontext
struct context_core_api_transfer_t {
  /// @brief The parent context that needs to be resumed after the invoked code is executed.
  context_core_api_fcontext_t fctx;
  /// @brief Opaque pointer for the data passed to the new code to be executed.
  void* data;
};

/// @brief Creates a new stackfull coroutine.
///
/// @param sp   The pointer to the end of the stack memory to be used by the coroutine
/// @param size The size of the stack memory
/// @param fn   The function to be called to start the coroutine
///
/// @return Handle to the context at the start of the new coroutine.
///
/// The new coroutine will operate on the given stack memory. Please note that the stack memory is
/// provided as a pair of (end, size), instead of the typical (start, size) or (start, end).
///
/// The function passed in will not be immediatelly executed. One needs to call
/// `context_core_api_jump_fcontext` to actually start executing the function. Also, the parameters
/// passed to this function are specified when jumping to the context.
///
/// This is equivalent with:
///   - create a new stack for the coroutine
///   - create a suspension at the start of the given function:
///       `suspended_ctx = suspend()`
///   - when the suspension point is resumed, the resuling data will be passed to the given function
///   - return `suspended_ctx`
///
/// @sa context_core_api_fcontext_t, context_core_api_jump_fcontext
context_core_api_fcontext_t
context_core_api_make_fcontext(void* sp, size_t size,
                               void (*fn)(struct context_core_api_transfer_t));

/// @brief Jump to the given context.
///
/// @param to The context to jump to.
/// @param vp Argument to pass to the new context.
///
/// @return The current context that was suspended and the argument used when jumping.
///
/// This will switch the current control flow and resumes executing the control flow represented by
/// `to`. Before doing so, it will suspend the current control flow, returning a context handle to
/// it.
///
/// This is equivalent with:
///   suspended_ctx = suspend();
///   switch_to(to, suspended_ctx, vp);
///   {ctx, data} = get_resume_data();
///   return {ctx, data};
///
/// If the control is resumed to a context that was created with `context_core_api_make_fcontext`
/// the argumend passed in will be passed as parameter to the given coroutine function. If not, the
/// data will be returned at the point of resumption.
///
/// @sa context_core_api_fcontext_t, context_core_api_make_fcontext, context_core_api_ontop_fcontext
struct context_core_api_transfer_t
context_core_api_jump_fcontext(context_core_api_fcontext_t const to, void* vp);

/// @brief Execute the content of a given function on top of a given context.
///
/// @param to The context on top of which we need to execute the function.
/// @param vp The data that needs to be passed to the given function when executing it.
/// @param fn The function to be executed on top of the given context.
/// @return The context that was resumed in order to continue this, and the data sent to the last
/// jump.
///
/// This is similar to `context_core_api_jump_fcontext`, but instead of just transfering the control
/// to the place indicated by the given context, it will execute the given function first. The
/// context of the current suspention point is passed to the given function.
///
/// This is equivalent to:
///   - suspend the current control flow; let `c1` be the context handle;
///   - switch the stack to the coroutine indicated by `to`;
///   - call function `fn` passing `c1` and the data represented by `vp`;
///   - let `r` be the result of calling `fn`
///   - resume the context `r.fctx`, passing `r.data` as data;
///     - i.e., call: `context_core_api_jump_fcontext(r.fctx, r.data)`
///
/// Note: for `context_core_api_ontop_fcontext` to work, the `to` context need to be entered at
/// least once (i.e., can't be a newly created coroutine).
///
/// It is expected that the context of the current resumption point will be eventually resumed.
///
/// @sa context_core_api_jump_fcontext, context_core_api_make_fcontext
struct context_core_api_transfer_t context_core_api_ontop_fcontext(
    context_core_api_fcontext_t const to, void* vp,
    struct context_core_api_transfer_t (*fn)(struct context_core_api_transfer_t));

#ifdef __cplusplus
}
#endif
