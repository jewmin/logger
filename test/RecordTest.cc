#include "gtest/gtest.h"
#include "Record.h"

TEST(RecordTest, ctor) {
	Logger::Record record("category", "message", Logger::Priority::kDebug);
	EXPECT_STREQ(*record.category_, "category");
	EXPECT_STREQ(*record.message_, "message");
	EXPECT_EQ(record.priority_, Logger::Priority::kDebug);

	Logger::Record copy_record(record);
	EXPECT_STREQ(*copy_record.category_, "category");
	EXPECT_STREQ(*copy_record.message_, "message");
	EXPECT_EQ(copy_record.priority_, Logger::Priority::kDebug);
	EXPECT_EQ(copy_record.time_stamp_, record.time_stamp_);

	Logger::Record move_record(std::move(record));
	EXPECT_STREQ(*move_record.category_, "category");
	EXPECT_STREQ(*move_record.message_, "message");
	EXPECT_EQ(move_record.priority_, Logger::Priority::kDebug);
	EXPECT_EQ(move_record.time_stamp_, copy_record.time_stamp_);
}