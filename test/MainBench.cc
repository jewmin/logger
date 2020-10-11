#ifdef USE_VLD
#include "vld.h"
#endif
#include "Common.h"
#include "Category.h"
#include "Appender/DailyRollingFileAppender.h"
#include "Appender/RollingFileAppender.h"
#include "Layout/PatternLayout.h"

int main(int argc, char * * argv) {
	bool async = false;
	if (argc >= 2) {
		if (strcmp(argv[1], "false") == 0) {
			async = false;
		} else if (strcmp(argv[1], "true") == 0) {
			async = true;
		}
	}
	int count = 1000000;
	Logger::PatternLayout * layout = new Logger::PatternLayout();
	layout->SetConversionPattern("%d{%Y-%m-%d %H:%M:%S,%l} - %c - %P - %m%n");
	Logger::DailyRollingFileAppender * appender = new Logger::DailyRollingFileAppender("dailyAppender", "bench.log", async);
	appender->SetLayout(layout);
	Logger::Category::GetRoot()->SetPriority(Logger::Priority::kDebug);
	Logger::Category::GetRoot()->AddAppender(appender);
	auto start = std::chrono::system_clock::now();
	while (count > 0) {
		--count;
		Logger::Category::GetRoot()->Debug("this is bench message. about 150 bytes. 01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	}
	Logger::Appender::DeleteAllAppenders();
	auto end = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
#ifdef _WIN32
	std::printf("use micro seconds: %lld\n", duration.count());
#else
	std::printf("use micro seconds: %ld\n", duration.count());
#endif
	return 0;
}