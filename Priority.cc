#include "Priority.h"

namespace Logger {

const Priority::Value MinPriority = static_cast<Priority::Value>(Priority::kNotSet);
const Priority::Value MaxPriority = static_cast<Priority::Value>(Priority::kCrash);

const std::string * Names() {
	static const std::string priority_names[] = { "NOTSET", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
	return priority_names;
}

const std::string & Priority::GetPriorityName(Value priority) {
	return Names()[((priority < MinPriority) || (priority > MaxPriority)) ? MinPriority : priority];
}

Priority::Value Priority::GetPriorityValue(const std::string & priority) {
	Value value = -1;
	for (Value i = MinPriority; i <= MaxPriority; ++i) {
		if (priority == Names()[i]) {
			value = i;
			break;
		}
	}
	return value;
}

}