#include "context_core_api.h"
#include <catch2/catch_test_macros.hpp>

struct stack_memory {
  stack_memory() {
    constexpr size_t stack_size = 1024;
    data_ = reinterpret_cast<uint8_t*>(malloc(stack_size));
    size_ = stack_size;
  }
  ~stack_memory() { free(data_); }

  void* end() const { return data_; }
  size_t size() const { return size_; }

private:
  uint8_t* data_;
  size_t size_;
};

TEST_CASE("Can create fcontexts, without crashing", "[context_core_api_make_fcontext]") {
  // Arrange
  stack_memory stack;
  struct t {
    static void context_fun(context_core_api_transfer_t) {}
  };

  // Act
  auto res = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);

  // Assert: if we are here, we didn't crash
  REQUIRE(true);
}

TEST_CASE("Two calls to context_core_api_make_fcontext will have different results",
          "[context_core_api_make_fcontext]") {
  // Arrange
  stack_memory stack1;
  stack_memory stack2;
  struct t {
    static void context_fun(context_core_api_transfer_t) {}
  };

  // Act
  auto res1 = context_core_api_make_fcontext(stack1.end(), stack1.size(), &t::context_fun);
  auto res2 = context_core_api_make_fcontext(stack2.end(), stack2.size(), &t::context_fun);

  // Assert: if we are here, we didn't crash
  REQUIRE(res1 != res2);
}

TEST_CASE("Function passed to context_core_api_make_fcontext is executed",
          "[context_core_api_make_fcontext]") {
  // Arrange
  stack_memory stack;
  bool called = false;
  struct t {
    static void context_fun(context_core_api_transfer_t param) {
      // Execute the code in the new context
      *reinterpret_cast<bool*>(param.data) = true;
      // Jump back to the previous context
      context_core_api_jump_fcontext(param.fctx, nullptr);
    }
  };

  // Act
  auto ctx = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);
  auto r = context_core_api_jump_fcontext(ctx, &called);

  // Assert
  REQUIRE(called);
}

TEST_CASE("Can jump into a context", "[context_core_api_jump_fcontext]") {
  // Arrange
  stack_memory stack;
  bool called = false;
  struct t {
    static void context_fun(context_core_api_transfer_t param) {
      // Execute the code in the new context
      *reinterpret_cast<bool*>(param.data) = true;
      // Jump back to the previous context
      context_core_api_jump_fcontext(param.fctx, nullptr);
    }
  };
  auto ctx = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);

  // Act
  auto r = context_core_api_jump_fcontext(ctx, &called);

  // Assert
  REQUIRE(r.fctx != nullptr);
  REQUIRE(called);
}

TEST_CASE("Can jump into a context, in a loop", "[context_core_api_jump_fcontext]") {
  // Arrange
  stack_memory stack;
  int count = 0;
  struct t {
    static void context_fun(context_core_api_transfer_t param) {
      auto ctx = param.fctx;
      while (true) {
        // Execute the code in the new context
        *reinterpret_cast<int*>(param.data) += 1;

        // Repeatedly jump back to the parent context
        auto r = context_core_api_jump_fcontext(param.fctx, nullptr);
        ctx = r.fctx;
      }
    }
  };
  auto ctx = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);
  constexpr int num_loops = 10;

  // Act
  for (int i = 0; i < num_loops; i++) {
    auto r = context_core_api_jump_fcontext(ctx, &count);
    ctx = r.fctx;
  }

  // Assert
  REQUIRE(ctx != nullptr);
  REQUIRE(count == num_loops);
}

TEST_CASE("context_core_api_jump_fcontext can pass data around",
          "[context_core_api_jump_fcontext]") {
  // Arrange
  stack_memory stack;
  bool called = false;
  struct t {
    static void context_fun(context_core_api_transfer_t param) {
      // Check the value passed by main control flow
      REQUIRE(reinterpret_cast<uintptr_t>(param.data) == 1);
      // Jump back to the previous context, sending back value 2
      context_core_api_jump_fcontext(param.fctx, reinterpret_cast<void*>(uintptr_t(2)));
    }
  };
  auto ctx = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);

  // Act: send value 1, expect to get back value 2
  auto r = context_core_api_jump_fcontext(ctx, reinterpret_cast<void*>(uintptr_t(1)));
  printf("prev: %p\n", r.fctx);

  // Assert
  REQUIRE(reinterpret_cast<uintptr_t>(r.data) == 2);
}

TEST_CASE("Can jump into a function on top of a context", "[context_core_api_ontop_fcontext]") {
  // Arrange
  stack_memory stack;
  int arr[3] = {0, 0, 0};
  struct t {
    static void context_fun(context_core_api_transfer_t param) {
      // for context_core_api_ontop_fcontext to work, we need to enter this context at least once.
      auto r = context_core_api_jump_fcontext(param.fctx, param.data);

      // Execute the code in the new context
      auto int_ptr = reinterpret_cast<int*>(r.data);
      *int_ptr = 1;

      // Jump back to the previous context
      context_core_api_jump_fcontext(r.fctx, int_ptr + 1);
    }
    static context_core_api_transfer_t ontop_fun(context_core_api_transfer_t param) {
      auto int_ptr = reinterpret_cast<int*>(param.data);
      *int_ptr = 2;
      return {param.fctx, int_ptr + 1};
    }
  };
  auto ctx = context_core_api_make_fcontext(stack.end(), stack.size(), &t::context_fun);
  ctx = context_core_api_jump_fcontext(ctx, nullptr).fctx;

  // Act
  auto r = context_core_api_ontop_fcontext(ctx, arr, &t::ontop_fun);

  // Assert
  REQUIRE(r.fctx != nullptr);
  REQUIRE(arr[0] == 2);       // first the function is called
  REQUIRE(arr[1] == 1);       // then the base context
  REQUIRE(arr[2] == 0);       // nobody will touch the third element
  REQUIRE(r.data == &arr[2]); // pointer to the third element will be returned
}
