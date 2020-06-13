#include "Layout/SimpleLayout.h"
#include "StringUtil.h"

namespace Logger {

SimpleLayout::SimpleLayout() {
}

SimpleLayout::~SimpleLayout() {
}

std::string SimpleLayout::Format(const Record & record) {
	return StringUtil::Format("%-8s: %s\n", Priority::GetPriorityName(record.priority_).c_str(), record.message_.c_str());
}

}