// Copyright 2023 owent

#pragma once

#include <libcopp/utils/config/libcopp_build_features.h>

#include <libcopp/utils/features.h>

#include <cstddef>

#ifdef COPP_HAS_ABI_HEADERS
#  include COPP_ABI_PREFIX
#endif

LIBCOPP_COPP_NAMESPACE_BEGIN
struct stack_context;

namespace allocator {

/**
 * @brief memory allocator
 * this allocator will create buffer using posix api and protect it
 */
class LIBCOPP_COPP_API stack_allocator_posix {
 public:
  stack_allocator_posix() LIBCOPP_MACRO_NOEXCEPT;
  ~stack_allocator_posix();
  stack_allocator_posix(const stack_allocator_posix &other) LIBCOPP_MACRO_NOEXCEPT;
  stack_allocator_posix &operator=(const stack_allocator_posix &other) LIBCOPP_MACRO_NOEXCEPT;
  stack_allocator_posix(stack_allocator_posix &&other) LIBCOPP_MACRO_NOEXCEPT;
  stack_allocator_posix &operator=(stack_allocator_posix &&other) LIBCOPP_MACRO_NOEXCEPT;

  /**
   * allocate memory and attach to stack context [standard function]
   * @param ctx stack context
   * @param size stack size
   */
  void allocate(stack_context &, std::size_t) LIBCOPP_MACRO_NOEXCEPT;

  /**
   * deallocate memory from stack context [standard function]
   * @param ctx stack context
   */
  void deallocate(stack_context &) LIBCOPP_MACRO_NOEXCEPT;
};
}  // namespace allocator
LIBCOPP_COPP_NAMESPACE_END

#ifdef COPP_HAS_ABI_HEADERS
#  include COPP_ABI_SUFFIX
#endif
