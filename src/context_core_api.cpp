#include "context_core_api.h"

#include <boost/context/continuation.hpp>

namespace ctx = boost::context::detail;

context_core_api_fcontext_t
context_core_api_make_fcontext(void* sp, size_t size, void (*fn)(context_core_api_transfer_t)) {
  return ctx::make_fcontext(sp, size, reinterpret_cast<void (*)(ctx::transfer_t)>(fn));
}

context_core_api_transfer_t context_core_api_jump_fcontext(context_core_api_fcontext_t const to,
                                                           void* vp) {
  auto r = ctx::jump_fcontext(to, vp);
  return {r.fctx, r.data};
}

context_core_api_transfer_t
context_core_api_ontop_fcontext(context_core_api_fcontext_t const to, void* vp,
                                context_core_api_transfer_t (*fn)(context_core_api_transfer_t)) {
  auto r = ctx::ontop_fcontext(to, vp, reinterpret_cast<ctx::transfer_t (*)(ctx::transfer_t)>(fn));
  return {r.fctx, r.data};
}
