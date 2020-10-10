#include "Appender/AppenderSkeleton.h"

namespace Logger {

AppenderSkeleton::AppenderSkeleton(const i8 * name, bool async_log) : Appender(name), async_log_(async_log) {
	if (async_log_) {
		Start();
	}
}

AppenderSkeleton::~AppenderSkeleton() {
}

void AppenderSkeleton::DoAppend(const Record & record) {
	if (async_log_) {
		Common::CMutex::ScopedLock lock(record_mutex_);
		record_vec_.push_back(new Record(record));
	} else {
		_Append(record);
	}
}

bool AppenderSkeleton::ReOpen() {
	return true;
}

void AppenderSkeleton::Close() {
}

void AppenderSkeleton::OnRountine() {
	while (!Terminated()) {
		_DoAppend();
		Common::CThread::Sleep(10);
	}
}

void AppenderSkeleton::_DoAppend() {
	std::vector<Record *> vec;
	{
		Common::CMutex::ScopedLock lock(record_mutex_);
		vec.swap(record_vec_);
	}
	for (auto & it : vec) {
		_Append(*it);
		delete it;
	}
}

}