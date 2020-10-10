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