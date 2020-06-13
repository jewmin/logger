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

#ifndef Logger_Layout_PatternLayout_INCLUDED
#define Logger_Layout_PatternLayout_INCLUDED

#include "Layout/Layout.h"

namespace Logger {

class LOGGER_EXTERN PatternLayout : public Layout {
public:
	PatternLayout();
	virtual ~PatternLayout();

	/*
	 * %%  百分号
	 * %n 换行符
	 * %c 日志类别
	 * %m 日志消息
	 * %p 日志级别(数字)
	 * %P 日志级别(字符串)
	 * %t 日志记录时间(时间戳)
	 * %M 日志记录时间(时间戳毫秒部分)
	 * %u 日志记录时间(从程序启动计算)
	 * %d{%Y-%m-%d %H:%M:%S,%l} 格式化日志记录时间
	 * %-10.20m 左对齐10~20字符 %20.50m 右对齐20~50字符
	 */
	virtual std::string Format(const Record & record) override;
	virtual void SetConversionPattern(const std::string & conversion_pattern);
	virtual std::string GetConversionPattern() const;
	virtual void ClearConversionPattern();

	class LOGGER_EXTERN PatternComponent {
	public:
		virtual ~PatternComponent() {}
		virtual void Append(std::ostringstream & out, const Record & record) = 0;
	};

private:
	std::vector<PatternComponent *> * components_;
	std::string conversion_pattern_;
};

}

#endif