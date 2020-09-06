#include "Layout/BasicLayout.h"

namespace Logger {

BasicLayout::BasicLayout() {
}

BasicLayout::~BasicLayout() {
}

Common::SDString BasicLayout::Format(const Record & record) {
	return Common::SDString::Format("%lld %s %s: %s\n", record.time_stamp_.GetSeconds(), *Priority::GetPriorityName(record.priority_), *record.category_, *record.message_);
}

}