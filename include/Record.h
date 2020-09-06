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

#ifndef Logger_Record_INCLUDED
#define Logger_Record_INCLUDED

#include "Common.h"
#include "CObject.h"
#include "Priority.h"
#include "SDString.h"
#include "TimeStamp.h"

namespace Logger {

class COMMON_EXTERN Record : public Common::CObject {
public:
	Record(const i8 * category, const i8 * message, Priority::Value priority);
	Record(Record && other);
	Record(const Record & other);
	~Record();

private:
	Record() = delete;
	Record & operator=(Record &&) = delete;
	Record & operator=(const Record &) = delete;

public:
	const Common::SDString category_;
	const Common::SDString message_;
	const Priority::Value priority_;
	Common::TimeStamp time_stamp_;
};

}

#endif