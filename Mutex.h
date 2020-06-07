/*
 * MIT License
 *
 * Copyright (c) 2019 jewmin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef Logger_Mutex_INCLUDED
#define Logger_Mutex_INCLUDED

#include "Logger.h"

namespace Logger {

template<class M>
class ScopedLock {
public:
	explicit ScopedLock(M & mutex) : mutex_(mutex) { mutex_.Lock(); }
	virtual ~ScopedLock() { mutex_.Unlock(); }

private:
	ScopedLock() = delete;
	ScopedLock(ScopedLock &&) = delete;
	ScopedLock(const ScopedLock &) = delete;
	ScopedLock & operator=(ScopedLock &&) = delete;
	ScopedLock & operator=(const ScopedLock &) = delete;

private:
	M & mutex_;
};

class Mutex {
public:
	typedef ScopedLock<Mutex> ScopedLock;

	Mutex() {}
	~Mutex() {}

	void Lock();
	bool TryLock();
	void Unlock();

private:
	Mutex(Mutex &&) = delete;
	Mutex(const Mutex &) = delete;
	Mutex & operator=(Mutex &&) = delete;
	Mutex & operator=(const Mutex &) = delete;

private:
	std::mutex mutex_;
};

//*********************************************************************
//Mutex
//*********************************************************************

inline void Mutex::Lock() {
	mutex_.lock();
}

inline bool Mutex::TryLock() {
	return mutex_.try_lock();
}

inline void Mutex::Unlock() {
	mutex_.unlock();
}

}

#endif