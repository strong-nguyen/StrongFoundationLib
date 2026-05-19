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

TEST(StringHelperTest, SplitEmptyStringWithEmptyDelimeter)
{
  auto tokens = sfs::splitStr("", "");
  EXPECT_TRUE(tokens.empty());
}

TEST(StringHelperTest, SplitStringWithEmptyDelimeter)
{
  auto tokens = sfs::splitStr("abc abc", "");
  EXPECT_TRUE(tokens.empty());
}

TEST(StringHelperTest, SplitStringWithSpaceDelimeter)
{
  auto tokens = sfs::splitStr("abcabc", " ");
  EXPECT_EQ(tokens, std::vector<std::string>{"abcabc"});
}

TEST(StringHelperTest, SplitStringWithSpaceDelimeterV2)
{
  auto tokens = sfs::splitStr("abc abc", " ");
  EXPECT_EQ(tokens, (std::vector<std::string>{"abc", "abc"}));
}

TEST(StringHelperTest, SplitStringWithSpaceDelimeterV3)
{
  auto tokens = sfs::splitStr("abc  abc", " ");
  EXPECT_EQ(tokens, (std::vector<std::string>{"abc", "abc"}));
}

TEST(StringHelperTest, SplitStringWithCommaDelimeter)
{
  std::string str = "1,2,3,4,5";
  std::string delimeter = ",";
  auto tokens = sfs::splitStr(str, delimeter);
  EXPECT_EQ(tokens, (std::vector<std::string>{"1", "2", "3", "4", "5"}));
}

TEST(StringHelperTest, SplitStringWithCommaDelimeterV2)
{
  std::string str = ",,,1,2,,,3,4,5,,";
  std::string delimeter = ",";
  auto tokens = sfs::splitStr(str, delimeter);
  EXPECT_EQ(tokens, (std::vector<std::string>{"1", "2", "3", "4", "5"}));
}

TEST(StringHelperTest, SplitStringAndTransform)
{
  std::string str = ",,,1,2,,,3,4,5,,";
  std::string delimeter = ",";
  auto tokens = sfs::splitStr(str, delimeter, [](const std::string& token)
    {
      return std::stoi(token);
    });

  EXPECT_EQ(tokens, (std::vector<int>{1, 2, 3, 4, 5}));
}