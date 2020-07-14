#ifdef USE_VLD
#include "vld.h"
#endif
#include "Logger.h"
#include "Category.h"
#include "Appender/DailyRollingFileAppender.h"
#include "Appender/RollingFileAppender.h"
#include "Layout/PatternLayout.h"

void thread_work(int count, int timeout, Logger::Category * category) {
	while (count > 0) {
		category->Info("this is %d info message, timeout %d", count, timeout);
		--count;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}
}

// int main(int argc, char * * argv) {
// 	bool async = true;
// 	if (argc >= 2) {
// 		if (strcmp(argv[1], "false") == 0) {
// 			async = false;
// 		} else if (strcmp(argv[1], "true") == 0) {
// 			async = true;
// 		}
// 	}
// 	int count = 10;
// 	std::thread threads[10];
// 	Logger::Appender * appender1 = new Logger::DailyRollingFileAppender("dailyAppender", "daily.log", async);
// 	Logger::Appender * appender2 = new Logger::RollingFileAppender("rollingAppender", "rolling.log", async);
// 	Logger::Appender * appender3 = new Logger::FileAppender("fileAppender", 1, async);
// 	Logger::Category::GetCategory("level1.level2.level3")->SetPriority(Logger::Priority::kDebug);
// 	Logger::Category::GetCategory("level1.level2")->SetPriority(Logger::Priority::kInfo);
// 	Logger::Category::GetCategory("level1")->SetPriority(Logger::Priority::kError);
// 	Logger::Category::GetCategory("level1.level2.level3")->AddAppender(appender1);
// 	Logger::Category::GetCategory("level1")->AddAppender(appender2);
// 	Logger::Category::GetCategory("level1")->AddAppender(appender3);
// 	Logger::Category * category[3] = { Logger::Category::GetCategory("level1.level2.level3"), Logger::Category::GetCategory("level1.level2"), Logger::Category::GetCategory("level1") };
// 	for (int i = 0; i < count; ++i) {
// 		threads[i] = std::thread(thread_work, 1000, (i + 1) * 10, category[i % 3]);
// 	}
// 	for (int i = 0; i < count; ++i) {
// 		threads[i].join();
// 	}
// 	Logger::Appender::DeleteAllAppenders();
// 	return 0;
// }

int main(int argc, char * * argv) {
	bool async = false;
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
	std::printf("use micro seconds: %lld\n", duration.count());
	return 0;
}