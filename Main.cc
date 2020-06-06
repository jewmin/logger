#ifdef USE_VLD
#include "vld.h"
#endif

#include "Logger.h"
#include "TimeStamp.h"
#include "StringUtil.h"

void TestTimeStamp() {
	Logger::TimeStamp a;
	Logger::TimeStamp b(a.GetSeconds());
	Logger::TimeStamp c(a.GetSeconds(), a.GetMicroSeconds());
	Logger::TimeStamp d(a);
	Logger::TimeStamp e;
	e = a;
	printf("%lld, %lld, %lld\n", a.GetSeconds(), a.GetMilliSeconds(), a.GetMicroSeconds());
	printf("%lld, %lld, %lld\n", b.GetSeconds(), b.GetMilliSeconds(), b.GetMicroSeconds());
	printf("%lld, %lld, %lld\n", c.GetSeconds(), c.GetMilliSeconds(), c.GetMicroSeconds());
	printf("%lld, %lld, %lld\n", d.GetSeconds(), d.GetMilliSeconds(), d.GetMicroSeconds());
	printf("%lld, %lld, %lld\n", e.GetSeconds(), e.GetMilliSeconds(), e.GetMicroSeconds());
}

void Format(const i8 * fmt, ...) {
	va_list va;
	va_start(va, fmt);
	printf("%s\n", Logger::StringUtil::Format(fmt, va).c_str());
	va_end(va);
}

void TestStringUtil() {
	std::string s;
	printf("origin: %s trim: %s\n", s.c_str(), Logger::StringUtil::Trim(s).c_str());
	s = "\ta b c d e f g ";
	printf("origin: %s trim: %s\n", s.c_str(), Logger::StringUtil::Trim(s).c_str());
	s = "  \t \r  a b c d e f g    \r\n";
	printf("origin: %s trim: %s\n", s.c_str(), Logger::StringUtil::Trim(s).c_str());
	s = "a b c d e f g";
	printf("origin: %s trim: %s\n", s.c_str(), Logger::StringUtil::Trim(s).c_str());
	s = "   \t   \r   \n   ";
	printf("origin: %s trim: %s\n", s.c_str(), Logger::StringUtil::Trim(s).c_str());

	std::vector<std::string> v;
	s = "192.168.1.100";
	printf("%s\n", s.c_str());
	Logger::StringUtil::Split(v, s, '.');
	for (int i = 0; i < v.size(); i++) {
		printf("%s\n", v[i].c_str());
	}
	Logger::StringUtil::Split(v, s, '1', 3);
	for (int i = 0; i < v.size(); i++) {
		printf("%s\n", v[i].c_str());
	}

	Format("%s %d %d", "1234567890", 1234567890, 9876543210);
	i8 buf[1200];
	std::memset(buf, 'b', sizeof(buf));
	buf[sizeof(buf) - 1] = 0;
	Format("%s", buf);
}

i32 main(i32 argc, const i8 * * argv) {
	TestTimeStamp();
	TestStringUtil();
	return 0;
}