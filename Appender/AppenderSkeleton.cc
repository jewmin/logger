#include "Appender/AppenderSkeleton.h"

namespace Logger {

AppenderSkeleton::AppenderSkeleton(const std::string & name, bool async_log) : Appender(name), async_log_(async_log) {
	if (async_log_) {
		Start();
	}
}

AppenderSkeleton::~AppenderSkeleton() {
	for (auto & it : record_vec_) {
		delete it;
	}
	record_vec_.clear();
}

void AppenderSkeleton::DoAppend(const Record & record) {
	if (async_log_) {
		Mutex::ScopedLock lock(record_mutex_);
		record_vec_.push_back(new Record(record));
	} else {
		_Append(record);
	}
}

bool AppenderSkeleton::ReOpen() {
	return true;
}

void AppenderSkeleton::OnRountine() {
	while (!Terminated()) {
		_DoAppend();
		Thread::Sleep(10);
	}
}

void AppenderSkeleton::_DoAppend() {
	std::vector<Record *> vec;
	{
		Mutex::ScopedLock lock(record_mutex_);
		vec.swap(record_vec_);
	}
	for (auto & it : vec) {
		_Append(*it);
		delete it;
	}
}

}