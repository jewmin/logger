#include "Record.h"

namespace Logger {

Record::Record(const std::string & category, const std::string & message, Priority::Value priority)
	: category_(category), message_(message), priority_(priority) {
}

Record::~Record() {
}

}