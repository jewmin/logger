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

#include "Appender/FileAppender.h"

namespace Logger {

FileAppender::FileAppender(const i8 * name, const i8 * file_name, bool async_log, bool append, mode_t mode)
	: LayoutAppender(name, async_log), file_name_(file_name), flags_(O_CREAT | O_APPEND | O_WRONLY), mode_(mode) {
	if (!append) {
		flags_ |= O_TRUNC;
	}
	fd_ = ::open(*file_name_, flags_, mode_);
}

FileAppender::FileAppender(const i8 * name, i32 fd, bool async_log)
	: LayoutAppender(name, async_log), file_name_(""), fd_(fd), flags_(O_CREAT | O_APPEND | O_WRONLY), mode_(00644) {
}

FileAppender::~FileAppender() {
	Wait();
	_DoAppend();
	Close();
}

bool FileAppender::ReOpen() {
	if (file_name_ != "") {
		i32 fd = ::open(*file_name_, flags_, mode_);
		if (fd == -1) {
			return false;
		}
		if (fd_ != -1) {
			::close(fd_);
		}
		fd_ = fd;
	}
	return true;
}

void FileAppender::Close() {
	if (fd_ != -1) {
		::close(fd_);
		fd_ = -1;
	}
}

void FileAppender::SetAppend(bool append) {
	if (append) {
		flags_ &= ~O_TRUNC;
	} else {
		flags_ |= O_TRUNC;
	}
}

bool FileAppender::GetAppend() const {
	return 0 == (flags_ & O_TRUNC);
}

void FileAppender::SetMode(mode_t mode) {
	mode_ = mode;
}

mode_t FileAppender::GetMode() const {
	return mode_;
}

void FileAppender::_Append(const Record & record) {
	Common::SDString message(GetLayout()->Format(record));
	if (fd_ != -1) {
		::write(fd_, *message, static_cast<u32>(message.Size()));
	}
}

}