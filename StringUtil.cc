#include "StringUtil.h"

namespace Logger {


std::string StringUtil::Format(const i8 * format, ...) {
	va_list args;
	va_start(args, format);
	std::string s = FormatVa(format, args);
	va_end(args);
	return s;
}

std::string StringUtil::FormatVa(const i8 * format, va_list args) {
	size_t size = 1024;
	i8 * buffer = new i8[size];

	while (true) {
		va_list args_copy;
#ifdef _WIN32
		args_copy = args;
#else
		va_copy(args_copy, args);
#endif
		i32 n = std::vsnprintf(buffer, size, format, args_copy);
		va_end(args_copy);

		if (n > -1 && static_cast<size_t>(n) < size) {
			std::string s(buffer);
			delete [] buffer;
			return s;
		}

		size = n > -1 ? n + 1 : size << 1;
		delete [] buffer;
		buffer = new i8[size];
	}
}

std::string StringUtil::Trim(const std::string & s) {
	static const i8 * white_space = " \t\r\n";
	if (s.empty()) {
		return s;
	}
	std::string::size_type b = s.find_first_not_of(white_space);
	if (b == std::string::npos) {
		return "";
	}
	std::string::size_type e = s.find_last_not_of(white_space);
	return std::string(s, b, e - b + 1);
}

u32 StringUtil::Split(std::vector<std::string> & v, const std::string & s, i8 delimiter, u32 max_segments) {
	v.clear();
	std::string::size_type left = 0;
	u32 num;
	for (num = 1; num < max_segments; ++num) {
		std::string::size_type right = s.find(delimiter, left);
		if (right == std::string::npos) {
			break;
		} else {
			v.push_back(s.substr(left, right - left));
			left = right + 1;
		}
	}
	v.push_back(s.substr(left));
	return num;
}

}