#include "gtest/gtest.h"
#include "Category.h"

class MockCategoryAppender : public Logger::Appender {
public:
	MockCategoryAppender(const std::string & name) : Logger::Appender(name) {
	}

	virtual ~MockCategoryAppender() {
	}

	virtual void DoAppend(const Logger::Record & record) override {
		std::printf("%lld,%lld [%s][%s] %s\n", record.time_stamp_.GetSeconds(), record.time_stamp_.GetMilliSeconds(), record.category_.c_str(), GetName().c_str(), record.message_.c_str());
	}

	virtual bool ReOpen() override {
		std::printf("%s reopen\n", GetName().c_str());
		return true;
	}

	virtual void Close() override {
		std::printf("%s close\n", GetName().c_str());
	}

	virtual bool RequiresLayout() const override {
		return true;
	}

	virtual void SetLayout(Logger::Layout * layout) override {
	}
};

class CategoryTest : public testing::Test {
public:
	virtual void SetUp() override {
		Logger::Category::GetRoot()->SetPriority(Logger::Priority::kInfo);
		Logger::Category::GetCategory("root.first.second")->SetPriority(Logger::Priority::kNotSet);
		Logger::Category::GetCategory("root.first")->SetPriority(Logger::Priority::kNotSet);
		Logger::Category::GetCategory("root")->SetPriority(Logger::Priority::kNotSet);
	}

	virtual void TearDown() override {
		Logger::Category::ShutdownForced();
	}
};

TEST_F(CategoryTest, root) {
	Logger::Category * root = Logger::Category::GetRoot();
	EXPECT_TRUE(root != nullptr);
	EXPECT_STREQ(root->GetName().c_str(), "");
}

TEST_F(CategoryTest, root_priority) {
	EXPECT_EQ(Logger::Category::GetRootPriority(), Logger::Priority::kInfo);
	Logger::Category::SetRootPriority(Logger::Priority::kFatal);
	EXPECT_EQ(Logger::Category::GetRootPriority(), Logger::Priority::kFatal);
}

TEST_F(CategoryTest, root_noset_priority) {
	EXPECT_ANY_THROW(Logger::Category::SetRootPriority(Logger::Priority::kNotSet));
}

TEST_F(CategoryTest, get) {
	Logger::Category * c1 = Logger::Category::GetExistingCategory("test");
	// EXPECT_TRUE(c1 == nullptr);
	Logger::Category * c2 = Logger::Category::GetCategory("test");
	EXPECT_TRUE(c2 != nullptr);
	EXPECT_STREQ(c2->GetName().c_str(), "test");
	c1 = Logger::Category::GetExistingCategory("test");
	EXPECT_TRUE(c1 == c2);
}

TEST_F(CategoryTest, empty_shutdown) {
	Logger::Category::Shutdown();
}

TEST_F(CategoryTest, empty_shutdown_force) {
	Logger::Category::ShutdownForced();
}

TEST_F(CategoryTest, noset_priority) {
	Logger::Category * c = Logger::Category::GetCategory("test");
	EXPECT_EQ(c->GetPriority(), Logger::Priority::kNotSet);
	c->SetPriority(Logger::Priority::kNotSet);
	EXPECT_EQ(c->GetPriority(), Logger::Priority::kNotSet);
}

TEST_F(CategoryTest, const_parent) {
	const Logger::Category * c = Logger::Category::GetCategory("test");
	EXPECT_TRUE(c->GetParent() == const_cast<const Logger::Category *>(Logger::Category::GetRoot()));
}

TEST_F(CategoryTest, chain_priority) {
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	EXPECT_EQ(c->GetChainedPriority(), Logger::Priority::kInfo);
	c->SetPriority(Logger::Priority::kFatal);
	EXPECT_EQ(c->GetChainedPriority(), Logger::Priority::kFatal);
	c->SetPriority(Logger::Priority::kNotSet);
	c->GetParent()->SetPriority(Logger::Priority::kError);
	EXPECT_EQ(c->GetChainedPriority(), Logger::Priority::kError);
}

TEST_F(CategoryTest, enable_priority) {
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	EXPECT_EQ(c->IsDebugEnabled(), false);
	EXPECT_EQ(c->IsInfoEnabled(), true);
	EXPECT_EQ(c->IsWarnEnabled(), true);
	EXPECT_EQ(c->IsErrorEnabled(), true);
	EXPECT_EQ(c->IsFatalEnabled(), true);
}

TEST_F(CategoryTest, log_warn) {
	Logger::Category::GetCategory("root.first")->SetPriority(Logger::Priority::kWarn);
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	c->Debug("this is debug message, disable");
	c->Info("this is info message, disable");
	c->Warn("this is warn message, enable");
	c->Error("this is error message, enable");
	c->Fatal("this is fatal message, enable");
}

TEST_F(CategoryTest, log_debug) {
	Logger::Category::SetRootPriority(Logger::Priority::kDebug);
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	c->Debug("this is debug message, enable");
	c->Info("this is info message, enable");
	c->Warn("this is warn message, enable");
	c->Error("this is error message, enable");
	c->Fatal("this is fatal message, enable");
}

TEST_F(CategoryTest, log_debug_string) {
	Logger::Category::SetRootPriority(Logger::Priority::kDebug);
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	c->Debug(std::string("this is debug message, enable"));
	c->Info(std::string("this is info message, enable"));
	c->Warn(std::string("this is warn message, enable"));
	c->Error(std::string("this is error message, enable"));
	c->Fatal(std::string("this is fatal message, enable"));
}

TEST_F(CategoryTest, appender) {
	Logger::Category * c = Logger::Category::GetCategory("root.first");
	c->AddAppender(new MockCategoryAppender("appender"));
	Logger::Appender * a = new MockCategoryAppender("more_appender");
	c->AddAppender(a);
	c->AddAppender(a);
}

TEST_F(CategoryTest, appender_throw) {
	Logger::Category * c = Logger::Category::GetCategory("root.first");
	EXPECT_ANY_THROW(c->AddAppender(nullptr));
}

class CategoryAppenderTest : public CategoryTest {
public:
	virtual void SetUp() override {
		CategoryTest::SetUp();
		Logger::Category::GetCategory("root.first")->AddAppender(new MockCategoryAppender("root.first"));
		Logger::Category::GetCategory("root.first")->AddAppender(new MockCategoryAppender("root.second"));
		Logger::Category::GetCategory("root.first.second")->AddAppender(new MockCategoryAppender("root.first.second"));
	}

	virtual void TearDown() override {
		CategoryTest::TearDown();
	}

	static void Format(const i8 * format, ...) {
		va_list va;
		va_start(va, format);
		for (int i = 0; i < 10; i++) {
			Logger::Category::GetRoot()->LogVa(i, format, va);
		}
		va_end(va);
	}
};

TEST_F(CategoryAppenderTest, get_appender) {
	EXPECT_TRUE(Logger::Category::GetCategory("root")->GetAppender() == nullptr);
	EXPECT_TRUE(Logger::Category::GetCategory("root.first")->GetAppender() != nullptr);
	EXPECT_STREQ(Logger::Category::GetCategory("root.first.second")->GetAppender()->GetName().c_str(), "root.first.second");

	EXPECT_TRUE(Logger::Category::GetCategory("root")->GetAppender("root.first") == nullptr);
	EXPECT_STREQ(Logger::Category::GetCategory("root.first")->GetAppender("root.second")->GetName().c_str(), "root.second");
	EXPECT_STREQ(Logger::Category::GetCategory("root.first.second")->GetAppender("root.first.second")->GetName().c_str(), "root.first.second");
}

TEST_F(CategoryAppenderTest, remove_appender) {
	Logger::Category::GetCategory("root")->RemoveAllAppenders();
	EXPECT_ANY_THROW(Logger::Category::GetCategory("root.first")->RemoveAppender(nullptr));
	Logger::Category::GetCategory("root.first")->RemoveAppender(Logger::Appender::GetAppender("root.second"));
	Logger::Category::GetCategory("root.first.second")->RemoveAppender(Logger::Appender::GetAppender("root.second"));
}

TEST_F(CategoryAppenderTest, log_debug) {
	Logger::Category::SetRootPriority(Logger::Priority::kDebug);
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	c->Debug("this is %s message, enable", Logger::Priority::GetPriorityName(c->GetPriority()).c_str());
	c->Info("this is %s message, enable", Logger::Priority::GetPriorityName(c->GetPriority()).c_str());
	c->Warn("this is %s message, enable", Logger::Priority::GetPriorityName(c->GetPriority()).c_str());
	c->Error("this is %s message, enable", Logger::Priority::GetPriorityName(c->GetPriority()).c_str());
	c->Fatal("this is %s message, enable", Logger::Priority::GetPriorityName(c->GetPriority()).c_str());
}

TEST_F(CategoryAppenderTest, log_debug_string) {
	Logger::Category::SetRootPriority(Logger::Priority::kDebug);
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	c->SetAdditivity(false);
	c->Debug(std::string("this is debug message, enable"));
	c->Info(std::string("this is info message, enable"));
	c->Warn(std::string("this is warn message, enable"));
	c->Error(std::string("this is error message, enable"));
	c->Fatal(std::string("this is fatal message, enable"));
}

TEST_F(CategoryAppenderTest, log) {
	Logger::Category * c = Logger::Category::GetCategory("root.first.second");
	for (i32 i = 0; i < 6; i++) {
		c->Log(i, "test log %d %s", i, Logger::Priority::GetPriorityName(i).c_str());
		c->Log(i, std::string("string message"));
	}
	Format("[%s]: %d", "abcdefg", 1234567890);
}