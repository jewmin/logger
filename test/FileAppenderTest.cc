#include "gtest/gtest.h"
#include "Appender/AppenderSkeleton.h"
#include "Appender/LayoutAppender.h"
#include "Layout/SimpleLayout.h"
#include "Appender/FileAppender.h"
#include "Appender/RollingFileAppender.h"
#include "Appender/DailyRollingFileAppender.h"

class MockAsyncAppender : public Logger::AppenderSkeleton {
public:
	MockAsyncAppender(bool async) : Logger::AppenderSkeleton("MockAsyncAppender", async) {}
	virtual ~MockAsyncAppender() { Wait(); _DoAppend(); }
	virtual void _Append(const Logger::Record & record) override {
		std::printf("%s: %s\n", *record.category_, *record.message_);
	}
	virtual bool RequiresLayout() const override {
		return false;
	}
	virtual void SetLayout(Logger::Layout * layout) override {
	}
};

class MockLayoutAppender : public Logger::LayoutAppender {
public:
	MockLayoutAppender(bool async) : Logger::LayoutAppender("MockLayoutAppender", async) {}
	virtual ~MockLayoutAppender() { Wait(); _DoAppend(); }
	virtual void _Append(const Logger::Record & record) override {
		std::printf("%s: %s\n", *record.category_, *record.message_);
	}
	void Test() {
		Logger::Layout * layout = GetLayout();
		SetLayout(layout);
		Logger::Layout * new_layout = GetLayout();
		EXPECT_TRUE(layout == new_layout);
	}
};

TEST(AppenderSkeletonTest, sync) {
	MockAsyncAppender sync(false);
	sync.DoAppend(Logger::Record("AppenderSkeletonTest", "this is a sync appender skeleton message", Logger::Priority::kInfo));
}

TEST(AppenderSkeletonTest, async) {
	MockAsyncAppender async(true);
	async.DoAppend(Logger::Record("AppenderSkeletonTest", "this is a async appender skeleton message", Logger::Priority::kInfo));
}

TEST(AppenderSkeletonTest, open) {
	MockAsyncAppender async(true);
	EXPECT_EQ(async.ReOpen(), true);
	async.Close();
}

TEST(LayoutAppenderTest, sync) {
	MockLayoutAppender appender(false);
	EXPECT_EQ(appender.RequiresLayout(), true);
	appender.Test();
	appender.SetLayout(nullptr);
	appender.SetLayout(new Logger::SimpleLayout());
	appender.DoAppend(Logger::Record("LayoutAppenderTest", "this is a sync layout appender message", Logger::Priority::kInfo));
}

TEST(LayoutAppenderTest, async) {
	MockLayoutAppender appender(true);
	EXPECT_EQ(appender.RequiresLayout(), true);
	appender.SetLayout(nullptr);
	appender.SetLayout(new Logger::SimpleLayout());
	appender.DoAppend(Logger::Record("LayoutAppenderTest", "this is a sync layout appender message", Logger::Priority::kInfo));
}

TEST(FileAppenderTest, invalid_fd_ctor) {
	Logger::FileAppender file("FileAppenderTest", -1);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message", Logger::Priority::kInfo));
	EXPECT_EQ(file.ReOpen(), true);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00644));
	EXPECT_EQ(file.GetAppend(), true);
}

TEST(FileAppenderTest, trunc_ctor) {
	Logger::FileAppender file("FileAppenderTest", "test.log", false, false, 00755);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message", Logger::Priority::kInfo));
	EXPECT_EQ(file.ReOpen(), true);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00755));
	EXPECT_EQ(file.GetAppend(), false);
}

TEST(FileAppenderTest, fd_ctor) {
	i32 fd = ::open("test.log", O_CREAT | O_APPEND | O_WRONLY, 00644);
	Logger::FileAppender file("FileAppenderTest", fd);
	EXPECT_EQ(file.ReOpen(), true);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00644));
	EXPECT_EQ(file.GetAppend(), true);
}

TEST(FileAppenderTest, reopen) {
	Logger::FileAppender file("FileAppenderTest", "test.log");
	EXPECT_EQ(file.ReOpen(), true);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00644));
	EXPECT_EQ(file.GetAppend(), true);
}

TEST(FileAppenderTest, reopen_error) {
	Logger::FileAppender file("FileAppenderTest", "......");
	EXPECT_EQ(file.ReOpen(), false);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00644));
	EXPECT_EQ(file.GetAppend(), true);
}

TEST(FileAppenderTest, append) {
	Logger::FileAppender file("FileAppenderTest", "test.log");
	EXPECT_EQ(file.GetAppend(), true);
	file.SetAppend(false);
	EXPECT_EQ(file.GetAppend(), false);
	file.SetAppend(true);
	EXPECT_EQ(file.GetAppend(), true);
}

TEST(FileAppenderTest, mode) {
	Logger::FileAppender file("FileAppenderTest", "test.log");
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00644));
	file.SetMode(00755);
	EXPECT_EQ(file.GetMode(), static_cast<mode_t>(00755));
}

TEST(FileAppenderTest, async) {
	Logger::FileAppender file("FileAppenderTest", "async_test.log", true);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message", Logger::Priority::kInfo));
	Common::CThread::Sleep(100);
	file.Wait();
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a file test message after thread join", Logger::Priority::kWarning));
}

TEST(FileAppenderTest, wait) {
	Logger::FileAppender file("FileAppenderTest", "wait_test.log", true);
	file.DoAppend(Logger::Record("FileAppenderTest", "this is a wait file test message", Logger::Priority::kInfo));
}

TEST(RollingFileAppenderTest, use) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "rolltest.log");
	EXPECT_EQ(roll.GetAppend(), true);
	EXPECT_EQ(roll.GetMode(), (mode_t)00644);
	EXPECT_EQ(roll.GetMaxBackupIndex(), 1);
	EXPECT_EQ(roll.GetMaxFileSize(), static_cast<size_t>(10 * 1024 * 1024));
	roll.SetAppend(false);
	roll.SetMode(00777);
	roll.SetMaxBackupIndex(4);
	roll.SetMaximumFileSize(45);
	EXPECT_EQ(roll.GetAppend(), false);
	EXPECT_EQ(roll.GetMode(), (mode_t)00777);
	EXPECT_EQ(roll.GetMaxBackupIndex(), 4);
	EXPECT_EQ(roll.GetMaxFileSize(), static_cast<size_t>(45));
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
	Common::CThread::Sleep(100);
	roll.Wait();
	roll.DoAppend(Logger::Record("RollCategory", "this is a async rolling test message after thread join", Logger::Priority::kInfo));
	roll.DoAppend(Logger::Record("RollCategory", "this is a async other rolling test message after thread join", Logger::Priority::kError));
}

TEST(RollingFileAppenderTest, wait) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "wait_rolltest.log", true, 30, 3);
	roll.DoAppend(Logger::Record("RollCategory", "this is a wait rolling test message", Logger::Priority::kDebug));
	roll.DoAppend(Logger::Record("RollCategory", "this is a wait other rolling test message", Logger::Priority::kWarning));
}

TEST(RollingFileAppenderTest, error) {
	Logger::RollingFileAppender roll("RollingFileAppenderTest", "......", false, 30, 3);
	roll.SetMaxBackupIndex(0);
	roll.RollOver();
}

TEST(DailyRollingFileAppenderTest, use) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "dailytest.log");
	EXPECT_EQ(daily.GetAppend(), true);
	EXPECT_EQ(daily.GetMode(), (mode_t)00644);
	daily.SetAppend(false);
	daily.SetMode(00777);
	EXPECT_EQ(daily.GetAppend(), false);
	EXPECT_EQ(daily.GetMode(), (mode_t)00777);
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
	record.time_stamp_ = Common::TimeStamp(10000, 999999);
	daily.DoAppend(record);
}

TEST(DailyRollingFileAppenderTest, async) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "async_dailytest.log", true);
	daily.DoAppend(Logger::Record("DailyCategory", "this is a async daily rolling test message", Logger::Priority::kInfo));
	Common::CThread::Sleep(100);
	daily.Wait();
	daily.DoAppend(Logger::Record("DailyCategory", "this is a async daily rolling test message after thread join", Logger::Priority::kWarning));
}

TEST(DailyRollingFileAppenderTest, wait) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "wait_dailytest.log", true);
	daily.DoAppend(Logger::Record("DailyCategory", "this is a wait daily rolling test message", Logger::Priority::kInfo));
}

TEST(DailyRollingFileAppenderTest, error) {
	Logger::DailyRollingFileAppender daily("DailyRollingFileAppenderTest", "......", false);
	daily.RollOver();
}