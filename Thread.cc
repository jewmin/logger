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
	thread->OnTerminated();
}

}