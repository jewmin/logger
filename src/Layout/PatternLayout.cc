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

#include "Layout/PatternLayout.h"
#include "LocalTime.h"
#include "ConfigureFailure.h"

namespace Logger {

//*********************************************************************
//PatternComponent
//*********************************************************************

// %%c 日志类别
class CategoryNameComponent : public PatternLayout::PatternComponent {
public:
	CategoryNameComponent() {}
	virtual ~CategoryNameComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << *record.category_;
	}
};

// %%m 日志消息
class MessageComponent : public PatternLayout::PatternComponent {
public:
	MessageComponent() {}
	virtual ~MessageComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << *record.message_;
	}
};

// %%p 日志级别(数字)
class PriorityComponent : public PatternLayout::PatternComponent {
public:
	PriorityComponent() {}
	virtual ~PriorityComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << record.priority_;
	}
};

// %%P 日志级别(字符串)
class PriorityNameComponent : public PatternLayout::PatternComponent {
public:
	PriorityNameComponent() {}
	virtual ~PriorityNameComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << Priority::GetPriorityName(record.priority_);
	}
};

// %%s 日志记录时间(时间戳)
class SecondsSinceEpochComponent : public PatternLayout::PatternComponent {
public:
	SecondsSinceEpochComponent() {}
	virtual ~SecondsSinceEpochComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << record.time_stamp_.GetSeconds();
	}
};

// %%S 日志记录时间(时间戳毫秒部分)
class MillisPortionComponent : public PatternLayout::PatternComponent {
public:
	MillisPortionComponent() {}
	virtual ~MillisPortionComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << record.time_stamp_.GetMilliSeconds();
	}
};

// %%u 日志记录时间(从程序启动计算)
class ProcessorTimeComponent : public PatternLayout::PatternComponent {
public:
	ProcessorTimeComponent() {}
	virtual ~ProcessorTimeComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << std::clock();
	}
};

// %%d{%%Y-%%m-%%d %%H:%%M:%%S,%%l} 格式化日志记录时间
class TimeStampComponent : public PatternLayout::PatternComponent {
public:
	TimeStampComponent(Common::SDString time_format) {
		if (time_format == "") {
			time_format = "%a %b %d %H:%M:%S %Y";
		}
		size_t pos = time_format.Find("%l");
		if (pos == Common::SDString::npos) {
			print_millis_ = false;
			time_format1_ = time_format; 
		} else {
			print_millis_ = true;
			time_format1_ = time_format.SubStr(0, pos);
			time_format2_ = time_format.SubStr(pos + 2);
		}
	}
	virtual ~TimeStampComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		struct std::tm current_time;
		std::time_t t = record.time_stamp_.GetSeconds();
		jc_localtime(&t, &current_time);

		i8 formatted[100];
		Common::SDString time_format;
		if (print_millis_) {
			std::ostringstream format_stream;
			format_stream << *time_format1_ << *Common::SDString::Format("%03d", record.time_stamp_.GetMilliSeconds()) << *time_format2_;
			time_format = format_stream.str().c_str();
		} else {
			time_format = time_format1_;
		}
		std::strftime(formatted, sizeof(formatted), *time_format, &current_time);
		out << formatted;
	}

private:
	Common::SDString time_format1_;
	Common::SDString time_format2_;
	bool print_millis_;
};

// 普通字符
class StringLiteralComponent : public PatternLayout::PatternComponent {
public:
	StringLiteralComponent(const Common::SDString & literal) : literal_(literal) {}
	virtual ~StringLiteralComponent() {}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		out << *literal_;
	}

private:
	Common::SDString literal_;
};

// 对齐方式参考printf
class FormatModifierComponent : public PatternLayout::PatternComponent {
public:
	FormatModifierComponent(PatternLayout::PatternComponent * component, size_t min_width, size_t max_width, bool align_left)
		: component_(component), min_width_(min_width), max_width_(max_width), align_left_(align_left) {}
	virtual ~FormatModifierComponent() {
		delete component_;
	}
	virtual void Append(std::ostringstream & out, const Record & record) override {
		std::ostringstream s;
		component_->Append(s, record);
		std::string msg = s.str();
		if (max_width_ > 0 && max_width_ < msg.length()) {
			msg.erase(max_width_);
		}
		size_t fill_count = min_width_ - msg.length();
		if (min_width_ > msg.length()) {
			if (align_left_) {
				out << msg << std::string(fill_count, ' ');
			} else {
				out << std::string(fill_count, ' ') << msg;
			}
		} else {
			out << msg;
		}
	}

private:
	PatternLayout::PatternComponent * component_;
	size_t min_width_;
	size_t max_width_;
	bool align_left_;
};

//*********************************************************************
//PatternLayout
//*********************************************************************

PatternLayout::PatternLayout() {
}

PatternLayout::~PatternLayout() {
	ClearConversionPattern();
}

Common::SDString PatternLayout::Format(const Record & record) {
	std::ostringstream message;
	for (Common::TList<PatternComponent *>::Iterator it = components_.Begin(); it != components_.End(); it++) {
		(*it)->Append(message, record);
	}
	return Common::SDString(message.str().c_str());
}

void PatternLayout::SetConversionPattern(const i8 * conversion_pattern) {
	std::istringstream conversion_stream(conversion_pattern);
	Common::SDString literal;

	i8 ch;
	PatternLayout::PatternComponent * component = nullptr;
	i32 min_width = 0;
	size_t max_width = 0;
	ClearConversionPattern();
	while (conversion_stream.get(ch)) {
		if (ch == '%') {
			{
				i8 ch2;
				conversion_stream.get(ch2);
				if ((ch2 == '-') || ((ch2 >= '0') && (ch2 <= '9'))) {
					conversion_stream.putback(ch2);
					conversion_stream >> min_width;
					conversion_stream.get(ch2);
				}
				if (ch2 == '.') {
					conversion_stream >> max_width;
				} else {
					conversion_stream.putback(ch2);
				}
			}
			if (!conversion_stream.get(ch)) {
				std::ostringstream msg;
				msg << "unterminated conversion specifier in '" << conversion_pattern << "' at index " << conversion_stream.tellg();
				throw ConfigureFailure(msg.str().c_str());
			}
			Common::SDString spec_postfix;
			{
				i8 ch2;
				if (conversion_stream.get(ch2)) {
					if (ch2 == '{') {
						while(conversion_stream.get(ch2) && (ch2 != '}')) {
							spec_postfix += ch2;
						}
					} else {
						conversion_stream.putback(ch2);
					}
				}
			}
			switch (ch) {
			case '%':
				literal += ch;
				break;

			case 'n':
				{
					std::ostringstream endline;
					endline << std::endl;
					literal += endline.str().c_str();
				}
				break;

			case 'c':
				component = new CategoryNameComponent();
				break;

			case 'm':
				component = new MessageComponent();
				break;

			case 'p':
				component = new PriorityComponent();
				break;

			case 'P':
				component = new PriorityNameComponent();
				break;

			case 't':
				component = new SecondsSinceEpochComponent();
				break;

			case 'M':
				component = new MillisPortionComponent();
				break;

			case 'u':
				component = new ProcessorTimeComponent();
				break;

			case 'd':
				component = new TimeStampComponent(spec_postfix);
				break;

			default:
				std::ostringstream msg;
				msg << "unknown conversion specifier '" << ch << "' in '" << conversion_pattern << "' at index " << conversion_stream.tellg();
				throw ConfigureFailure(msg.str().c_str());
			}
			if (component) {
				if (!literal.Empty()) {
					components_.PushBack(new StringLiteralComponent(literal));
					literal = "";
				}
				if ((min_width != 0) || (max_width != 0)) {
					component = new FormatModifierComponent(component, std::abs(min_width), max_width, min_width < 0);
					min_width = 0;
					max_width = 0;
				}
				components_.PushBack(component);
				component = nullptr;
			}
		} else {
			literal += ch;
		}
	}
	if (!literal.Empty()) {
		components_.PushBack(new StringLiteralComponent(literal));
	}

	conversion_pattern_ = conversion_pattern;
}

const i8 * PatternLayout::GetConversionPattern() const {
	return *conversion_pattern_;
}

void PatternLayout::ClearConversionPattern() {
	for (Common::TList<PatternComponent *>::Iterator it = components_.Begin(); it != components_.End(); it++) {
		delete *it;
	}
	components_.Clear();
	conversion_pattern_ = "";
}

}