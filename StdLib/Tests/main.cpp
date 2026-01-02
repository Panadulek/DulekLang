#include <gtest/gtest.h>
#include "memory_menagment_test.h"
TEST(StdLibTest, BasicAssertion) {
  EXPECT_EQ(1 + 1, 2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
