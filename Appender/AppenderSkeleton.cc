#include "Appender/AppenderSkeleton.h"

namespace Logger {

AppenderSkeleton::AppenderSkeleton(const std::string & name) : Appender(name) {
}

AppenderSkeleton::~AppenderSkeleton() {
}

void AppenderSkeleton::DoAppend(const Record & record) {
	_Append(record);
}

bool AppenderSkeleton::ReOpen() {
	return true;
}

}