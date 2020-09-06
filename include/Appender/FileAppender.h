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

#ifndef Logger_Appender_FileAppender_INCLUDED
#define Logger_Appender_FileAppender_INCLUDED

#include "Appender/LayoutAppender.h"

namespace Logger {

class COMMON_EXTERN FileAppender : public LayoutAppender {
public:
	FileAppender(const std::string & name, const std::string & file_name, bool async_log = false, bool append = true, mode_t mode = 00644);
	FileAppender(const std::string & name, i32 fd, bool async_log = false);
	virtual ~FileAppender();
	
	virtual bool ReOpen() override;
	virtual void Close() override;

	virtual void SetAppend(bool append);
	virtual bool GetAppend() const;
	virtual void SetMode(mode_t mode);
	virtual mode_t GetMode() const;

protected:
	virtual void _Append(const Record & record) override;

protected:
	const std::string file_name_;
	i32 fd_;
	i32 flags_;
	mode_t mode_;
};

}

#endif