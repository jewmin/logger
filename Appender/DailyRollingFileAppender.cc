#include "Appender/DailyRollingFileAppender.h"
#include "LocalTime.h"
#include "StringUtil.h"

namespace Logger {

DailyRollingFileAppender::DailyRollingFileAppender(const std::string & name, const std::string & file_name, bool append, mode_t mode)
	: FileAppender(name, file_name, append, mode) {
	std::time_t t;
	struct stat buf;
	if (::stat(file_name_.c_str(), &buf) != 0) {
		t = TimeStamp().GetSeconds();
	} else {
		t = buf.st_mtime;
	}
	LocalTime(&t, &log_time_);
}

DailyRollingFileAppender::~DailyRollingFileAppender() {
}

void DailyRollingFileAppender::RollOver() {
	Close();
	const std::string last_file_name = StringUtil::Format("%s.%04d-%02d-%02d", file_name_.c_str(), log_time_.tm_year + 1900, log_time_.tm_mon + 1, log_time_.tm_mday);
	if (0 != ::rename(file_name_.c_str(), last_file_name.c_str())) {
		std::cout << "Error renaming file " << file_name_ << " to " << last_file_name << std::endl;
	}
	if (!ReOpen()) {
		std::cout << "Error opening file " << file_name_ << std::endl;
	}
}

void DailyRollingFileAppender::_Append(const Record & record) {
	std::tm now;
	std::time_t t = record.time_stamp_.GetSeconds();
	LocalTime(&t, &now);
	if ((now.tm_mday != log_time_.tm_mday) || (now.tm_mon != log_time_.tm_mon) || (now.tm_year != log_time_.tm_year)) {
		RollOver();
		std::memcpy(&log_time_, &now, sizeof(log_time_));
	}
	FileAppender::_Append(record);
}

}