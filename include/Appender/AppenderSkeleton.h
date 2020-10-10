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

#ifndef Logger_Appender_AppenderSkeleton_INCLUDED
#define Logger_Appender_AppenderSkeleton_INCLUDED

#include "Common.h"
#include "Thread.h"
#include "Mutex.h"
#include "Record.h"
#include "Appender/Appender.h"

namespace Logger {

class COMMON_EXTERN AppenderSkeleton : public Appender, public Common::CThread {
public:
	virtual ~AppenderSkeleton();

	virtual void DoAppend(const Record & record) override;
	virtual bool ReOpen() override;
	virtual void Close() override;

protected:
	AppenderSkeleton(const i8 * name, bool async_log = false);

	void _DoAppend();
	virtual void OnRountine() override;
	virtual void _Append(const Record & record) = 0;

private:
	bool async_log_;
	Common::CMutex record_mutex_;
	std::vector<Record *> record_vec_;
};

}

#endif