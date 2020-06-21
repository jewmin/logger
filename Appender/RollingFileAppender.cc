#include "Appender/RollingFileAppender.h"
#include "LocalTime.h"
#include "StringUtil.h"

namespace Logger {

RollingFileAppender::RollingFileAppender(const std::string & name, const std::string & file_name, size_t max_file_size, i32 max_backup_index, bool append, mode_t mode)
	: FileAppender(name, file_name, append, mode), max_file_size_(max_file_size), max_backup_index_(max_backup_index > 0 ? max_backup_index : 1) {
}

RollingFileAppender::~RollingFileAppender() {
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
		std::string last_file_name = StringUtil::Format("%s.%d", file_name_.c_str(), max_backup_index_);
		::remove(last_file_name.c_str());
		for (i32 i = max_backup_index_; i > 1; --i) {
			std::string log_file_name = StringUtil::Format("%s.%d", file_name_.c_str(), i);
			::rename(log_file_name.c_str(), last_file_name.c_str());
			last_file_name = log_file_name;
		}
		::rename(file_name_.c_str(), last_file_name.c_str());
	}
	if (!ReOpen()) {
		std::cout << "Error opening file " << file_name_ << std::endl;
	}
}

void RollingFileAppender::_Append(const Record & record) {
	FileAppender::_Append(record);
	off_t offset = ::lseek(fd_, 0, SEEK_END);
	if (offset > 0 && static_cast<size_t>(offset) >= max_file_size_) {
		RollOver();
	}
}

}