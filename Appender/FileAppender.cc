#include "Appender/FileAppender.h"

namespace Logger {

FileAppender::FileAppender(const std::string & name, const std::string & file_name, bool append = true, mode_t mode = 00644)
	: LayoutAppender(name), file_name_(file_name), flags_(O_CREAT | O_APPEND | O_WRONLY), mode_(mode) {
	if (!append) {
		flags_ |= O_TRUNC;
	}
	fd_ = ::open(file_name_.c_str(), flags_, mode_);
}

FileAppender::FileAppender(const std::string & name, i32 fd)
	: LayoutAppender(name), file_name_(""), fd_(fd), flags_(O_CREAT | O_APPEND | O_WRONLY), mode_(00644) {
}

FileAppender::~FileAppender() {
	Close();
}

bool FileAppender::ReOpen() {
	if (file_name_ != "") {
		i32 fd = ::open(file_name_.c_str(), flags_, mode_);
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
	std::string message(GetLayout()->Format(record));
	::write(fd_, message.data(), message.length());
}

}