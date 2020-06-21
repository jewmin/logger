#include "Record.h"

namespace Logger {

Record::Record(const std::string & category, const std::string & message, Priority::Value priority)
	: category_(category), message_(message), priority_(priority) {
}

Record::Record(const Record & rhs)
	: category_(rhs.category_), message_(rhs.message_), priority_(rhs.priority_), time_stamp_(rhs.time_stamp_) {
}

Record::~Record() {
}

}