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

#include "Appender/DailyRollingFileAppender.h"
#include "LocalTime.h"
#include "SDString.h"

namespace Logger {

DailyRollingFileAppender::DailyRollingFileAppender(const i8 * name, const i8 * file_name, bool async_log, bool append, mode_t mode)
	: FileAppender(name, file_name, async_log, append, mode) {
	std::time_t t;
	struct stat buf;
	if (::stat(*file_name_, &buf) != 0) {
		t = Common::TimeStamp().GetSeconds();
	} else {
		t = buf.st_mtime;
	}
	jc_localtime(&t, &log_time_);
}

DailyRollingFileAppender::~DailyRollingFileAppender() {
	Wait();
	_DoAppend();
}

void DailyRollingFileAppender::RollOver() {
	Close();
	const Common::SDString last_file_name = Common::SDString::Format("%s.%04d-%02d-%02d", *file_name_, log_time_.tm_year + 1900, log_time_.tm_mon + 1, log_time_.tm_mday);
	if (0 != ::rename(*file_name_, *last_file_name)) {
		std::printf("Error renaming file %s to %s\n", *file_name_, *last_file_name);
	}
	if (!ReOpen()) {
		std::printf("Error opening file %s\n", *file_name_);
	}
}

void DailyRollingFileAppender::_Append(const Record & record) {
	std::tm now;
	std::time_t t = record.time_stamp_.GetSeconds();
	jc_localtime(&t, &now);
	if ((now.tm_mday != log_time_.tm_mday) || (now.tm_mon != log_time_.tm_mon) || (now.tm_year != log_time_.tm_year)) {
		RollOver();
		std::memcpy(&log_time_, &now, sizeof(log_time_));
	}
	FileAppender::_Append(record);
}

}