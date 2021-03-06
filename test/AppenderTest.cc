#include "gtest/gtest.h"
#include "Appender/Appender.h"

class MockAppender : public Logger::Appender {
public:
	MockAppender(const i8 * name) : Logger::Appender(name) {
	}

	virtual ~MockAppender() {
	}

	virtual void DoAppend(const Logger::Record & record) override {
		std::printf("%lld,%lld [%s] %s", record.time_stamp_.GetSeconds(), record.time_stamp_.GetMilliSeconds(), *record.category_, *record.message_);
	}

	virtual bool ReOpen() override {
		std::printf("%s reopen\n", *GetName());
		return true;
	}

	virtual void Close() override {
		std::printf("%s close\n", *GetName());
	}

	virtual bool RequiresLayout() const override {
		return true;
	}

	virtual void SetLayout(Logger::Layout * layout) override {
	}
};

class AppenderTest : public testing::Test {
public:
	virtual void SetUp() override {
		for (i32 i = 0; i < 3; ++i) {
			appenders_[i] = new MockAppender(*Common::SDString::Format("appender%d", i));
		}
	}

	virtual void TearDown() override {
		for (i32 i = 0; i < 3; ++i) {
			Logger::Appender * appender = Logger::Appender::GetAppender(*Common::SDString::Format("appender%d", i));
			if (appender) {
				delete appender;
			}
		}
	}

	MockAppender * appenders_[3];
};

TEST_F(AppenderTest, ctor) {
	{
		MockAppender appender("MockAppender");
		EXPECT_STREQ(*appender.GetName(), "MockAppender");
	}
	EXPECT_STREQ(*appenders_[0]->GetName(), "appender0");
	EXPECT_STREQ(*appenders_[1]->GetName(), "appender1");
	EXPECT_STREQ(*appenders_[2]->GetName(), "appender2");
}

TEST_F(AppenderTest, reopen) {
	EXPECT_EQ(Logger::Appender::ReOpenAll(), true);
}

TEST_F(AppenderTest, close) {
	Logger::Appender::CloseAll();
}

TEST_F(AppenderTest, get) {
	EXPECT_TRUE(Logger::Appender::GetAppender("appender0") != nullptr);
	EXPECT_TRUE(Logger::Appender::GetAppender("appender1") != nullptr);
	EXPECT_TRUE(Logger::Appender::GetAppender("appender2") != nullptr);
	EXPECT_TRUE(Logger::Appender::GetAppender("appender3") == nullptr);
}

TEST_F(AppenderTest, del) {
	Logger::Appender::DeleteAllAppenders();
}