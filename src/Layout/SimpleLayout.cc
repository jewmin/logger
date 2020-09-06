#include "Layout/SimpleLayout.h"

namespace Logger {

SimpleLayout::SimpleLayout() {
}

SimpleLayout::~SimpleLayout() {
}

Common::SDString SimpleLayout::Format(const Record & record) {
	return Common::SDString::Format("%-8s: %s\n", *Priority::GetPriorityName(record.priority_), *record.message_);
}

}