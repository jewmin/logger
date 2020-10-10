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

#ifndef Logger_Appender_RollingFileAppender_INCLUDED
#define Logger_Appender_RollingFileAppender_INCLUDED

#include "Record.h"
#include "Appender/FileAppender.h"

namespace Logger {

class COMMON_EXTERN RollingFileAppender : public FileAppender {
public:
	RollingFileAppender(const i8 * name, const i8 * file_name, bool async_log = false, size_t max_file_size = 10 * 1024 * 1024, i32 max_backup_index = -1, bool append = true, mode_t mode = 00644);
	virtual ~RollingFileAppender();

	virtual void SetMaxBackupIndex(i32 max_backup_index);
	virtual i32 GetMaxBackupIndex() const;
	virtual void SetMaximumFileSize(size_t max_file_size);
	virtual size_t GetMaxFileSize() const;
	virtual void RollOver();

protected:
	virtual void _Append(const Record & record) override;

private:
	size_t max_file_size_;
	i32 max_backup_index_;
};

}

#endif