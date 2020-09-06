#include "gtest/gtest.h"
#include "ConfigureFailure.h"

TEST(ConfigureFailureTest, throw) {
	EXPECT_THROW(throw Logger::ConfigureFailure("throw test expection"), Logger::ConfigureFailure);
}