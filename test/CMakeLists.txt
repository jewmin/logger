# cmake版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

# 项目名
PROJECT(logger_test VERSION 1.0)

# 添加源文件
SET(srcs
	${PROJECT_SOURCE_DIR}/Main.cc
	${PROJECT_SOURCE_DIR}/ConfigureFailureTest.cc
	${PROJECT_SOURCE_DIR}/PriorityTest.cc
	${PROJECT_SOURCE_DIR}/RecordTest.cc
	${PROJECT_SOURCE_DIR}/LayoutTest.cc
)

# 单元测试
ADD_EXECUTABLE(unittest-logger-static ${srcs})
ADD_EXECUTABLE(unittest-logger ${srcs})
TARGET_LINK_LIBRARIES(unittest-logger-static logger-static common-static ${GTEST_LIBRARIES})
TARGET_LINK_LIBRARIES(unittest-logger logger common ${GTEST_LIBRARIES})
IF(WINDOWS)
	TARGET_LINK_LIBRARIES(unittest-logger-static ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
	TARGET_LINK_LIBRARIES(unittest-logger ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
	TARGET_COMPILE_DEFINITIONS(unittest-logger PRIVATE USING_COMMON_SHARED)
ENDIF()

# 多线程测试
# ADD_EXECUTABLE(multitest-logger-static ${PROJECT_SOURCE_DIR}/MainMutliThread.cc)
# ADD_EXECUTABLE(multitest-logger ${PROJECT_SOURCE_DIR}/MainMutliThread.cc)
# TARGET_LINK_LIBRARIES(multitest-logger-static logger-static common-static)
# TARGET_LINK_LIBRARIES(multitest-logger logger common)
# IF(WINDOWS)
# 	TARGET_LINK_LIBRARIES(multitest-logger-static ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
# 	TARGET_LINK_LIBRARIES(multitest-logger ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
# 	TARGET_COMPILE_DEFINITIONS(multitest-logger PRIVATE USING_COMMON_SHARED)
# ENDIF()

# 压力测试
# ADD_EXECUTABLE(benchtest-logger-static ${PROJECT_SOURCE_DIR}/MainBench.cc)
# ADD_EXECUTABLE(benchtest-logger ${PROJECT_SOURCE_DIR}/MainBench.cc)
# TARGET_LINK_LIBRARIES(benchtest-logger-static logger-static common-static)
# TARGET_LINK_LIBRARIES(benchtest-logger logger common)
# IF(WINDOWS)
# 	TARGET_LINK_LIBRARIES(benchtest-logger-static ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
# 	TARGET_LINK_LIBRARIES(benchtest-logger ws2_32 iphlpapi psapi userenv ${VLD_LIBRARIES})
# 	TARGET_COMPILE_DEFINITIONS(benchtest-logger PRIVATE USING_COMMON_SHARED)
# ENDIF()