#include "gtest/gtest.h"
#include "Priority.h"

TEST(PriorityTest, get_value) {
	EXPECT_EQ(Logger::Priority::GetPriorityValue("haha"), -1);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("NOTSET"), Logger::Priority::kNotSet);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("DEBUG"), Logger::Priority::kDebug);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("INFO"), Logger::Priority::kInfo);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("WARNING"), Logger::Priority::kWarn);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("WARNING"), Logger::Priority::kWarning);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("ERROR"), Logger::Priority::kError);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("FATAL"), Logger::Priority::kFatal);
	EXPECT_EQ(Logger::Priority::GetPriorityValue("FATAL"), Logger::Priority::kCrash);
}

TEST(PriorityTest, get_name) {
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kNotSet - 1).c_str(), "NOTSET");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kNotSet).c_str(), "NOTSET");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kDebug).c_str(), "DEBUG");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kInfo).c_str(), "INFO");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kWarn).c_str(), "WARNING");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kWarning).c_str(), "WARNING");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kError).c_str(), "ERROR");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kFatal).c_str(), "FATAL");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kCrash).c_str(), "FATAL");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kCrash + 1).c_str(), "NOTSET");
}