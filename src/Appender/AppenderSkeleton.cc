/*
 * MIT License
 *
 * Copyright (c) 2019 jewmin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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