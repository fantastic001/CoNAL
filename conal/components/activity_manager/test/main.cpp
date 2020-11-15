#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include <node_spec/Parser.hpp>

TEST(TestParser, TestBasic) {
  ::conal::activity_manager::node_spec::Parser p; 
  EXPECT_EQ(p.parse("*,*")->dump(), std::string("*,*"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}