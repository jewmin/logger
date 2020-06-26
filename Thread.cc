#include "Thread.h"

namespace Logger {

Thread::Thread() : thread_(nullptr), terminated_(false) {
}

Thread::~Thread() {
	if (thread_) {
		Terminate();
		thread_->join();
		delete thread_;
	}
}

void Thread::Start() {
	if (!thread_) {
		thread_ = new std::thread(ThreadRoutine, this);
	}
}

void Thread::ThreadRoutine(Thread * thread) {
	thread->OnRountine();
}

void Thread::Sleep(i32 milliseconds) {
	std::this_thread::sleep_for(std::chrono::microseconds(milliseconds));
}

std::thread::id Thread::CurrentThreadId() {
	return std::this_thread::get_id();
}

}