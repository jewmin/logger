#include "gtest/gtest.h"
#include "Appender/AppenderSkeleton.h"
#include "Appender/LayoutAppender.h"
#include "Appender/FileAppender.h"
#include "Appender/DailyRollingFileAppender.h"
#include "Appender/RollingFileAppender.h"
#include "Layout/SimpleLayout.h"

class MockAsyncAppender : public Logger::AppenderSkeleton {
public:
	MockAsyncAppender() : Logger::AppenderSkeleton("MockAsyncAppender", true) {}
	virtual ~MockAsyncAppender() { Wait(); _DoAppend(); }
	virtual void _Append(const Logger::Record & record) override {
		// std::printf("%s: %s\n", record.category_.c_str(), record.message_.c_str());
	}
	virtual void Close() override {
	}
	virtual bool RequiresLayout() const override {
		return false;
	}
	virtual void SetLayout(Logger::Layout * layout) override {
	}
};

TEST(MockAsyncAppenderTest, async) {
	MockAsyncAppender async;
	async.DoAppend(Logger::Record("MockAsyncAppenderTest", "this is a appender skeleton message", Logger::Priority::kInfo));
}

TEST(FileAppenderTest, use) {
	i32 fd = ::open("test.log", O_CREAT | O_APPEND | O_WRONLY, 00644);
	Logger::FileAppender file("FileAppenderTest", fd);
}

TEST(FileAppenderTest, reopen) {
	Logger::FileAppender file("FileAppenderTest", "test.log");
	file.ReOpen();
}

TEST(FileAppenderTest, async) {
	Logger::FileAppender file("FileAppenderTest", "async_test.log", true);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message", Logger::Priority::kInfo));
	Logger::Thread::Sleep(100);
	file.Wait();
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message after thread join", Logger::Priority::kWarning));
}

TEST(FileAppenderTest, wait) {
	Logger::FileAppender file("FileAppenderTest", "wait_test.log", true);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a wait file test message", Logger::Priority::kInfo));
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
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest_layout.log", false, false);
	EXPECT_EQ(daily.RequiresLayout(), true);
	daily.SetLayout(nullptr);
	daily.SetLayout(new Logger::SimpleLayout());
}

TEST(DailyRollingFileAppenderTest, log) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest_log.log");
	daily.DoAppend(Logger::Record("DailyCategory", "this is a daily rolling test message", Logger::Priority::kInfo));
	daily.RollOver();
	Logger::Record record("DailyCategory", "this is a 1900 test message", Logger::Priority::kInfo);
	record.time_stamp_ = Logger::TimeStamp(10000, 999999);
	daily.DoAppend(record);
}

TEST(DailyRollingFileAppenderTest, async) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "async_dailytest.log", true);
	daily.DoAppend(Logger::Record("DailyCategory", "this is a async daily rolling test message", Logger::Priority::kInfo));
	Logger::Thread::Sleep(100);
	daily.Wait();
	daily.DoAppend(Logger::Record("DailyCategory", "this is a async daily rolling test message after thread join", Logger::Priority::kWarning));
}

TEST(DailyRollingFileAppenderTest, wait) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "wait_dailytest.log", true);
	daily.DoAppend(Logger::Record("DailyCategory", "this is a wait daily rolling test message", Logger::Priority::kInfo));
}

TEST(RollingFileAppenderTest, use) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "rolltest.log", false, 30, 3);
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
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "rolltest_log.log", false, 30, 3);
	roll.DoAppend(Logger::Record("RollCategory", "this is a rolling test message", Logger::Priority::kDebug));
	roll.DoAppend(Logger::Record("RollCategory", "this is a other rolling test message", Logger::Priority::kWarning));
}

TEST(RollingFileAppenderTest, async) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "async_rolltest.log", true, 30, 3);
	roll.DoAppend(Logger::Record("RollCategory", "this is a async rolling test message", Logger::Priority::kDebug));
	roll.DoAppend(Logger::Record("RollCategory", "this is a async other rolling test message", Logger::Priority::kWarning));
	Logger::Thread::Sleep(100);
	roll.Wait();
	roll.DoAppend(Logger::Record("RollCategory", "this is a async rolling test message after thread join", Logger::Priority::kInfo));
	roll.DoAppend(Logger::Record("RollCategory", "this is a async other rolling test message after thread join", Logger::Priority::kError));
}

TEST(RollingFileAppenderTest, wait) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "wait_rolltest.log", true, 30, 3);
	roll.DoAppend(Logger::Record("RollCategory", "this is a wait rolling test message", Logger::Priority::kDebug));
	roll.DoAppend(Logger::Record("RollCategory", "this is a wait other rolling test message", Logger::Priority::kWarning));
}