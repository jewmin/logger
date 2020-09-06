#include "gtest/gtest.h"
#include "Layout/Layout.h"
#include "Layout/BasicLayout.h"
#include "Layout/SimpleLayout.h"
#include "Layout/PatternLayout.h"
#include "ConfigureFailure.h"

class LayoutTest : public testing::Test {
public:
	LayoutTest() : test_time_(1592018476, 12345) {} // 2020-06-13 11:21:16
	Logger::TimeStamp test_time_;
};

TEST_F(LayoutTest, basic) {
	Logger::Record record("LayoutTest", "test basic layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::BasicLayout basic;
	EXPECT_STREQ(basic.Format(record).c_str(), "1592018476 INFO LayoutTest: test basic layout\n");
}

TEST_F(LayoutTest, simple) {
	Logger::Record record("LayoutTest", "test simple layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::SimpleLayout simple;
	EXPECT_STREQ(simple.Format(record).c_str(), "INFO    : test simple layout\n");
}

TEST_F(LayoutTest, empty_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "");
	EXPECT_STREQ(pattern.Format(record).c_str(), "");
}

TEST_F(LayoutTest, basic_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("%m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "%m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "test pattern layout\n");
}

TEST_F(LayoutTest, category_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: [%c] %m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: [LayoutTest] test pattern layout\n");
}

TEST_F(LayoutTest, priority_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: [INFO(2)] [LayoutTest] test pattern layout\n");
}

TEST_F(LayoutTest, timestamp_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: %t,%M [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: %t,%M [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: 1592018476,12 [INFO(2)] [LayoutTest] test pattern layout\n");
}

TEST_F(LayoutTest, starttime_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: %ums [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: %ums [%P(%p)] [%c] %m%n");
	std::printf("%s", pattern.Format(record).c_str());
}

TEST_F(LayoutTest, date_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: %d [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: %d [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: Sat Jun 13 11:21:16 2020 [INFO(2)] [LayoutTest] test pattern layout\n");
}

TEST_F(LayoutTest, datetime_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: %d{%Y-%m-%d %H:%M:%S,%l} [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: %d{%Y-%m-%d %H:%M:%S,%l} [%P(%p)] [%c] %m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: 2020-06-13 11:21:16,012 [INFO(2)] [LayoutTest] test pattern layout\n");
}

TEST_F(LayoutTest, format_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	pattern.SetConversionPattern("this is 100%% message: %-20.30d{%Y-%m-%d %H:%M:%S,%l} [%5P(%-3p)] [%c] %10.15m%n");
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "this is 100%% message: %-20.30d{%Y-%m-%d %H:%M:%S,%l} [%5P(%-3p)] [%c] %10.15m%n");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: 2020-06-13 11:21:16,012 [ INFO(2  )] [LayoutTest] test pattern la\n");
}

TEST_F(LayoutTest, error_pattern) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	EXPECT_THROW(pattern.SetConversionPattern("this is %o 100%% message: %-20.30d{%Y-%m-%d %H:%M:%S,%l} [%5P(%-3p)] [%c] %10.15m%n"), Logger::ConfigureFailure);
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "");
	EXPECT_STREQ(pattern.Format(record).c_str(), "");
}

TEST_F(LayoutTest, error_pattern2) {
	Logger::Record record("LayoutTest", "test pattern layout", Logger::Priority::kInfo);
	record.time_stamp_ = test_time_;
	Logger::PatternLayout pattern;
	EXPECT_THROW(pattern.SetConversionPattern("this is 100%% message: %-20.30d{%Y-%m-%d %H:%M:%S,%l} [%5P(%-3p)] [%c] %10.15m%n%"), Logger::ConfigureFailure);
	EXPECT_STREQ(pattern.GetConversionPattern().c_str(), "");
	EXPECT_STREQ(pattern.Format(record).c_str(), "this is 100% message: 2020-06-13 11:21:16,012 [ INFO(2  )] [LayoutTest] test pattern la");
}