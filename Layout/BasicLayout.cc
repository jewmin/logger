#include "Layout/BasicLayout.h"
#include "StringUtil.h"

namespace Logger {

BasicLayout::BasicLayout() {
}

BasicLayout::~BasicLayout() {
}

std::string BasicLayout::Format(const Record & record) {
	return StringUtil::Format("%lld %s %s: %s\n", record.time_stamp_.GetSeconds(), Priority::GetPriorityName(record.priority_).c_str(), record.category_.c_str(), record.message_.c_str());
}

}