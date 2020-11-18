#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include <DataDefinition.hpp>

using namespace conal::data_manager;

TEST(TestDataDefinition, TestNameAndParamsAndOptionalParamsBasic) {
  DataDefinition data("TextFile(myfile.txt, string)[a=1, b=2]");
  EXPECT_EQ(data.getName(), "TextFile");
  EXPECT_EQ(data.getParams()[0], "myfile.txt");
  EXPECT_EQ(data.getParams()[1], "string");
  EXPECT_EQ(data.getOptionalParams()["a"], "1");
  EXPECT_EQ(data.getOptionalParams()["b"], "2");
}

TEST(TestDataDefinition, TestNameAndParams) {
  DataDefinition data("TextFile(myfile.txt, string)");
  EXPECT_EQ(data.getName(), "TextFile");
  EXPECT_EQ(data.getParams()[0], "myfile.txt");
  EXPECT_EQ(data.getParams()[1], "string");
  EXPECT_EQ(data.getOptionalParams().size(), 0);
}

TEST(TestDataDefinition, TestOneParameter) {
  DataDefinition data("TextFile(myfile.txt)");
  EXPECT_EQ(data.getName(), "TextFile");
  EXPECT_EQ(data.getParams()[0], "myfile.txt");
  EXPECT_EQ(data.getParams().size(), 1);
  EXPECT_EQ(data.getOptionalParams().size(), 0);
}


TEST(TestDataDefinition, TestNoParameter) {
  DataDefinition data("TextFile()");
  EXPECT_EQ(data.getName(), "TextFile");
  EXPECT_EQ(data.getParams().size(), 0);
  EXPECT_EQ(data.getOptionalParams().size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}