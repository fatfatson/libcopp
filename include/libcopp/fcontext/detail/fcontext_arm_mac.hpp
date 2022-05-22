// Copyright 2022 owent

#pragma once

#include <libcopp/utils/config/libcopp_build_features.h>

#include "libcopp/fcontext/detail/config.hpp"
#include "libcopp/utils/features.h"

#include <stdint.h>
#include <cstddef>

#ifdef COPP_HAS_ABI_HEADERS
#  include COPP_ABI_PREFIX
#endif

LIBCOPP_COPP_NAMESPACE_BEGIN
namespace fcontext {

extern "C" {

#define COPP_BOOST_CONTEXT_CALLDECL

struct stack_t {
  void *sp;
  std::size_t size;

  stack_t() : sp(0), size(0) {}
};

struct fp_t {
  uint32_t fc_freg[16];

  fp_t() : fc_freg() {}
};

struct fcontext_t {
  uint32_t fc_greg[11];
  stack_t fc_stack;
  fp_t fc_fp;
  void *fc_unwind_sjlj;

  fcontext_t() : fc_greg(), fc_stack(), fc_fp(), fc_unwind_sjlj(0) {}
};
}
}  // namespace fcontext
LIBCOPP_COPP_NAMESPACE_END

#ifdef COPP_HAS_ABI_HEADERS
#  include COPP_ABI_SUFFIX
#endif
