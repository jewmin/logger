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

#include "Appender/RollingFileAppender.h"
#include "SDString.h"

namespace Logger {

RollingFileAppender::RollingFileAppender(const i8 * name, const i8 * file_name, bool async_log, size_t max_file_size, i32 max_backup_index, bool append, mode_t mode)
	: FileAppender(name, file_name, async_log, append, mode), max_file_size_(max_file_size), max_backup_index_(max_backup_index > 0 ? max_backup_index : 1) {
}

RollingFileAppender::~RollingFileAppender() {
	Wait();
	_DoAppend();
}

void RollingFileAppender::SetMaxBackupIndex(i32 max_backup_index) {
	max_backup_index_ = max_backup_index;
}

i32 RollingFileAppender::GetMaxBackupIndex() const {
	return max_backup_index_;
}

void RollingFileAppender::SetMaximumFileSize(size_t max_file_size) {
	max_file_size_ = max_file_size;
}

size_t RollingFileAppender::GetMaxFileSize() const {
	return max_file_size_;
}

void RollingFileAppender::RollOver() {
	Close();
	if (max_backup_index_ > 0) {
		Common::SDString last_file_name = Common::SDString::Format("%s.%d", *file_name_, max_backup_index_);
		::remove(*last_file_name);
		for (i32 i = max_backup_index_; i > 1; --i) {
			Common::SDString log_file_name = Common::SDString::Format("%s.%d", *file_name_, i);
			::rename(*log_file_name, *last_file_name);
			last_file_name = log_file_name;
		}
		::rename(*file_name_, *last_file_name);
	}
	if (!ReOpen()) {
		std::printf("Error opening file %s\n", *file_name_);
	}
}

void RollingFileAppender::_Append(const Record & record) {
	FileAppender::_Append(record);
	if (fd_ != -1) {
		off_t offset = ::lseek(fd_, 0, SEEK_END);
		if (offset > 0 && static_cast<size_t>(offset) >= max_file_size_) {
			RollOver();
		}
	}
}

}