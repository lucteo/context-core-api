#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* context_core_api_fcontext_t;

struct context_core_api_transfer_t {
  context_core_api_fcontext_t fctx;
  void* data;
};

context_core_api_fcontext_t context_core_api_make_fcontext(void* sp, size_t size,
                                                           void (*fn)(context_core_api_transfer_t));

context_core_api_transfer_t context_core_api_jump_fcontext(context_core_api_fcontext_t const to,
                                                           void* vp);

context_core_api_transfer_t
context_core_api_ontop_fcontext(context_core_api_fcontext_t const to, void* vp,
                                context_core_api_transfer_t (*fn)(context_core_api_transfer_t));

#ifdef __cplusplus
}
#endif
