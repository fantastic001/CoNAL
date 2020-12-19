#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include <node_spec/Parser.hpp>

TEST(TestParser, TestBasic) {
  ::conal::activity_manager::node_spec::Parser p; 
  EXPECT_EQ(p.parse("*,*")->dump(), std::string("*,*"));
  EXPECT_EQ(p.parse("*+*")->dump(), std::string("*+*"));
  EXPECT_EQ(p.parse("!*")->dump(), std::string("!*"));
  EXPECT_EQ(p.parse("name=ana")->dump(), std::string("name=ana"));
  EXPECT_EQ(p.parse("[p1]=v1")->dump(), std::string("[p1]=v1"));
  EXPECT_EQ(p.parse("[p1]=v1+name=spec")->dump(), std::string("[p1]=v1+name=spec"));
  EXPECT_EQ(p.parse("%")->dump(), std::string("*"));
  EXPECT_EQ(p.parse("*")->dump(), std::string("*"));
  EXPECT_EQ(p.parse("name=192.168.1.1")->dump(), std::string("name=192.168.1.1"));

}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}