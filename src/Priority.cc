#include "Priority.h"

namespace Logger {

const Priority::Value MinPriority = static_cast<Priority::Value>(Priority::kNotSet);
const Priority::Value MaxPriority = static_cast<Priority::Value>(Priority::kCrash);

const i8 * * Names() {
	static const i8 * priority_names[] = { "NOTSET", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
	return priority_names;
}

const i8 * Priority::GetPriorityName(Value priority) {
	return Names()[((priority < MinPriority) || (priority > MaxPriority)) ? MinPriority : priority];
}

Priority::Value Priority::GetPriorityValue(const i8 * priority) {
	Value value = kUnknown;
	for (Value i = MinPriority; i <= MaxPriority; ++i) {
		if (0 == std::strcmp(priority, Names()[i])) {
			value = i;
			break;
		}
	}
	return value;
}

}