#include "../src/StringHelper.h"

#include <gtest/gtest.h>

namespace sfs = sf::string;

TEST(StringHelperTest, TrimLeftWithNoSpace)
{
  std::string str = "abc";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimLeftWithEmptyString)
{
  std::string str = "";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimLeftWith1TrailingSpace)
{
  std::string str = " ";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimLeftWith1TrailingSpaceV2)
{
  std::string str = " abc";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimLeftWith5TrailingSpace)
{
  std::string str = "     ";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimLeftWith5TrailingSpaceV2)
{
  std::string str = "     abc";
  sfs::trimLeft(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimRighttWithNoSpace)
{
  std::string str = "abc";
  sfs::trimRight(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimRighttWithEmptyString)
{
  std::string str = "";
  sfs::trimRight(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimRightWith1TrailingSpace)
{
  std::string str = " ";
  sfs::trimRight(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimRightWith1TrailingSpaceV2)
{
  std::string str = "abc ";
  sfs::trimRight(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimRightWith5TrailingSpace)
{
  std::string str = "     ";
  sfs::trimRight(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimRightWith5TrailingSpaceV2)
{
  std::string str = "abc     ";
  sfs::trimRight(str);
  EXPECT_EQ(str, "abc");
}

TEST(StringHelperTest, TrimEmptyString)
{
  std::string str;
  sfs::trim(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimWith1TrailingSpace)
{
  std::string str = " ";
  sfs::trim(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimWith5TrailingSpace)
{
  std::string str = "     ";
  sfs::trim(str);
  EXPECT_EQ(str, "");
}

TEST(StringHelperTest, TrimWith5TrailingSpaceV2)
{
  std::string str = "  abc   ";
  sfs::trim(str);
  EXPECT_EQ(str, "abc");
}