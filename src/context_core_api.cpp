#include "context_core_api.h"

// #include <boost/context/continuation.hpp>

// using ctx = boost::context::detail;

context_core_api_fcontext_t
context_core_api_make_fcontext(void* sp, size_t size, void (*fn)(context_core_api_transfer_t)) {
  return nullptr;
  //   return ctx::jump_fcontext(sp, size, fn);
}

context_core_api_transfer_t context_core_api_jump_fcontext(context_core_api_fcontext_t const to,
                                                           void* vp) {
  return {nullptr, nullptr};
  //   return ctx::jump_fcontext(to, vp);
}

context_core_api_transfer_t
context_core_api_ontop_fcontext(context_core_api_fcontext_t const to, void* vp,
                                context_core_api_transfer_t (*fn)(context_core_api_transfer_t)) {
  return {nullptr, nullptr};
  //   return ctx::ontop_fcontext(to, vp, fn);
}
