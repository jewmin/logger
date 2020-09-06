#include "Record.h"

namespace Logger {

Record::Record(const i8 * category, const i8 * message, Priority::Value priority)
	: category_(category), message_(message), priority_(priority) {
}

Record::Record(Record && other) : category_(std::move(other.category_)), message_(std::move(other.message_)), priority_(other.priority_), time_stamp_(other.time_stamp_) {
}

Record::Record(const Record & other) : category_(other.category_), message_(other.message_), priority_(other.priority_), time_stamp_(other.time_stamp_) {
}

Record::~Record() {
}

}