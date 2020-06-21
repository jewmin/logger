#include "gtest/gtest.h"
#include "Appender/AppenderSkeleton.h"
#include "Appender/LayoutAppender.h"
#include "Appender/FileAppender.h"
#include "Appender/DailyRollingFileAppender.h"
#include "Appender/RollingFileAppender.h"
#include "Layout/SimpleLayout.h"

TEST(FileAppenderTest, use) {
	i32 fd = ::open("test.log", O_CREAT | O_APPEND | O_WRONLY, 00644);
	Logger::FileAppender file("FileAppenderTest", fd);
}

TEST(FileAppenderTest, reopen) {
	Logger::FileAppender file("FileAppenderTest", "test.log");
	file.ReOpen();
}

TEST(DailyRollingFileAppenderTest, use) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest.log");
	EXPECT_EQ(daily.GetAppend(), true);
	EXPECT_EQ(daily.GetMode(), 00644);
	daily.SetAppend(false);
	daily.SetMode(00777);
	EXPECT_EQ(daily.GetAppend(), false);
	EXPECT_EQ(daily.GetMode(), 00777);
}

TEST(DailyRollingFileAppenderTest, layout) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest.log", false);
	EXPECT_EQ(daily.RequiresLayout(), true);
	daily.SetLayout(nullptr);
	daily.SetLayout(new Logger::SimpleLayout());
}

TEST(DailyRollingFileAppenderTest, log) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest.log");
	daily.DoAppend(Logger::Record("DailyCategory", "this is a daily rolling test message", Logger::Priority::kInfo));
	daily.RollOver();
	Logger::Record record("DailyCategory", "this is a 1900 test message", Logger::Priority::kInfo);
	record.time_stamp_ = Logger::TimeStamp(10000, 999999);
	daily.DoAppend(record);
}

TEST(RollingFileAppenderTest, use) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "rolltest.log", 30, 3);
	EXPECT_EQ(roll.GetAppend(), true);
	EXPECT_EQ(roll.GetMode(), 00644);
	EXPECT_EQ(roll.GetMaxBackupIndex(), 3);
	EXPECT_EQ(roll.GetMaxFileSize(), 30);
	roll.SetAppend(false);
	roll.SetMode(00777);
	roll.SetMaxBackupIndex(4);
	roll.SetMaximumFileSize(45);
	EXPECT_EQ(roll.GetAppend(), false);
	EXPECT_EQ(roll.GetMode(), 00777);
	EXPECT_EQ(roll.GetMaxBackupIndex(), 4);
	EXPECT_EQ(roll.GetMaxFileSize(), 45);
	roll.SetAppend(true);
}

TEST(RollingFileAppenderTest, log) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "rolltest.log", 30, 3);
	roll.DoAppend(Logger::Record("RollCategory", "this is a rolling test message", Logger::Priority::kDebug));
	roll.DoAppend(Logger::Record("RollCategory", "this is a other rolling test message", Logger::Priority::kWarning));
}