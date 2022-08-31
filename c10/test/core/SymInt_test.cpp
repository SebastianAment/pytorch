#include <gtest/gtest.h>

#include <c10/core/SymInt.h>
#include <c10/core/SymIntNodeImpl.h>

using namespace c10;
#ifndef C10_MOBILE
void check(int64_t value) {
  EXPECT_TRUE(SymInt::check_range(value));
  const auto i = SymInt(value);
  EXPECT_FALSE(i.is_symbolic());
  EXPECT_EQ(i.as_int_unchecked(), value);
}

TEST(SymIntTest, ConcreteInts) {
  check(INT64_MAX);
  check(0);
  check(-1);
  // This is 2^62, which is the most negative number we can support.
  check(-4611686018427387904LL);
}

TEST(SymIntTest, AddNode) {
  auto n = c10::make_intrusive<SymIntNodeImpl>();
  auto i = n->toSymInt();
  EXPECT_TRUE(i.is_symbolic());
}

TEST(SymIntTest, SelfAssignment) {
  auto n = c10::make_intrusive<SymIntNodeImpl>();
  auto i = n->toSymInt();
  EXPECT_TRUE(i.is_symbolic());

  SymInt copy = i;
  EXPECT_TRUE(copy.is_symbolic());
  copy = copy;
  EXPECT_TRUE(copy.is_symbolic());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
  copy = std::move(copy);
#pragma GCC diagnostic pop
  EXPECT_TRUE(copy.is_symbolic());
}

TEST(SymIntTest, CheckRange) {
  EXPECT_FALSE(SymInt::check_range(INT64_MIN));
}
#endif
