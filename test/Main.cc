#ifdef USE_VLD
#include "vld.h"
#endif
#include "gtest/gtest.h"

int main(int argc, char * * argv) {
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	return result;
}