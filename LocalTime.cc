#include "LocalTime.h"

namespace Logger {

#ifdef _WIN32
void LocalTime(const std::time_t * time, std::tm * tm) {
	localtime_s(tm, time);
}
#else
void LocalTime(const std::time_t * time, std::tm * tm) {
	localtime_r(time, tm);
}
#endif

}