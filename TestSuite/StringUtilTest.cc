#include "gtest/gtest.h"
#include "StringUtil.h"

TEST(StringUtilTest, format) {
	std::string s = Logger::StringUtil::Format("%s %d %lld", "1234567890", 1234567890, 9876543210);
	EXPECT_STREQ(s.c_str(), "1234567890 1234567890 9876543210");
	std::printf("%s\n", s.c_str());
}

TEST(StringUtilTest, renew_format) {
	i8 buf[1200];
	std::memset(buf, 'b', sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = 0;
	std::string s = Logger::StringUtil::Format("%s", buf);
	EXPECT_STREQ(s.c_str(), buf);
	std::printf("%s\n", s.c_str());
}

TEST(StringUtilTest, empty_trim) {
	std::string s = Logger::StringUtil::Trim(std::string());
	EXPECT_EQ(s.empty(), true);
}

TEST(StringUtilTest, space_trim) {
	std::string s = Logger::StringUtil::Trim("                ");
	EXPECT_STREQ(s.c_str(), "");
	s = Logger::StringUtil::Trim("  \t  \t\t\t\r  \r\r\r  \n\n\n  \n\n  ");
	EXPECT_STREQ(s.c_str(), "");
}

TEST(StringUtilTest, left_trim) {
	std::string s = Logger::StringUtil::Trim("   \t   \r\n  left");
	EXPECT_STREQ(s.c_str(), "left");
}

TEST(StringUtilTest, right_trim) {
	std::string s = Logger::StringUtil::Trim("right   \t   \r\n  ");
	EXPECT_STREQ(s.c_str(), "right");
}

TEST(StringUtilTest, trim) {
	std::string s = Logger::StringUtil::Trim("   \t   \r\n  left    right   \t   \r\n  ");
	EXPECT_STREQ(s.c_str(), "left    right");
}

TEST(StringUtilTest, split) {
	std::vector<std::string> v;
	Logger::StringUtil::Split(v, "192.168.1.100", '.');
	EXPECT_STREQ(v[0].c_str(), "192");
	EXPECT_STREQ(v[1].c_str(), "168");
	EXPECT_STREQ(v[2].c_str(), "1");
	EXPECT_STREQ(v[3].c_str(), "100");
}

TEST(StringUtilTest, limit_split) {
	std::vector<std::string> v;
	Logger::StringUtil::Split(v, "192.168.1.100", '1', 3);
	EXPECT_EQ(v.size(), 3);
	EXPECT_STREQ(v[0].c_str(), "");
	EXPECT_STREQ(v[1].c_str(), "92.");
	EXPECT_STREQ(v[2].c_str(), "68.1.100");
}