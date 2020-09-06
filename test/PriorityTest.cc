#include "gtest/gtest.h"
#include "Priority.h"

TEST(PriorityTest, get_value) {
	EXPECT_EQ(Logger::Priority::GetPriorityValue("haha"), Logger::Priority::kUnknown);
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
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kUnknown), "NOTSET");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kNotSet), "NOTSET");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kDebug), "DEBUG");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kInfo), "INFO");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kWarn), "WARNING");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kWarning), "WARNING");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kError), "ERROR");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kFatal), "FATAL");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kCrash), "FATAL");
	EXPECT_STREQ(Logger::Priority::GetPriorityName(Logger::Priority::kCrash + 1), "NOTSET");
}