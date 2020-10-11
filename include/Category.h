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

#ifndef Logger_Category_INCLUDED
#define Logger_Category_INCLUDED

#include "Common.h"
#include "CObject.h"
#include "Mutex.h"
#include "Priority.h"
#include "SDString.h"
#include "Record.h"
#include "Appender/Appender.h"

namespace Logger {

class COMMON_EXTERN Category : public Common::CObject {
	friend class Hierarchy;

public:
	virtual ~Category();

	virtual const Common::SDString & GetName() const;
	virtual void SetPriority(Priority::Value priority);
	virtual Priority::Value GetPriority() const;
	virtual Priority::Value GetChainedPriority() const;
	virtual bool IsPriorityEnabled(Priority::Value priority) const;

	virtual void AddAppender(Appender * appender);
	virtual Appender * GetAppender() const;
	virtual Appender * GetAppender(const i8 * name) const;
	virtual void RemoveAllAppenders();
	virtual void RemoveAppender(Appender * appender);
	virtual void CallAppenders(const Record & record);
	virtual void SetAdditivity(bool additivity);
	virtual bool GetAdditivity() const;
	virtual Category * GetParent();
	virtual const Category * GetParent() const;
	virtual void Log(Priority::Value priority, const i8 * format, ...);
	virtual void Log(Priority::Value priority, const Common::SDString & message);
	virtual void LogVa(Priority::Value priority, const i8 * format, va_list va);

	void Debug(const i8 * format, ...);
	void Debug(const Common::SDString & message);
	bool IsDebugEnabled() const;

	void Info(const i8 * format, ...);
	void Info(const Common::SDString & message);
	bool IsInfoEnabled() const;

	void Warn(const i8 * format, ...);
	void Warn(const Common::SDString & message);
	bool IsWarnEnabled() const;

	void Error(const i8 * format, ...);
	void Error(const Common::SDString & message);
	bool IsErrorEnabled() const;

	void Fatal(const i8 * format, ...);
	void Fatal(const Common::SDString & message);
	bool IsFatalEnabled() const;

	static Category * GetRoot();
	static void SetRootPriority(Priority::Value priority);
	static Priority::Value GetRootPriority();
	static Category * GetCategory(const i8 * name);
	static Category * GetExistingCategory(const i8 * name);
	static void Shutdown();
	static void ShutdownForced();

protected:
	Category(const i8 * name, Category * parent, Priority::Value priority = Priority::kNotSet);
	virtual void _Log(Priority::Value priority, const i8 * format, va_list arguments);
	virtual void _Log(Priority::Value priority, const Common::SDString & message);

private:
	Category(Category &&) = delete;
	Category(const Category &) = delete;
	Category & operator=(Category &&) = delete;
	Category & operator=(const Category &) = delete;

private:
	const Common::SDString name_;
	Category * parent_;
	volatile Priority::Value priority_;
	volatile bool is_additive_;
	std::map<const Common::SDString, Appender *> appender_map_;
	mutable Common::CMutex appenders_mutex_;
};

inline bool Category::IsDebugEnabled() const {
	return IsPriorityEnabled(Priority::kDebug);
}

inline bool Category::IsInfoEnabled() const {
	return IsPriorityEnabled(Priority::kInfo);
}

inline bool Category::IsWarnEnabled() const {
	return IsPriorityEnabled(Priority::kWarn);
}

inline bool Category::IsErrorEnabled() const {
	return IsPriorityEnabled(Priority::kError);
}

inline bool Category::IsFatalEnabled() const {
	return IsPriorityEnabled(Priority::kFatal);
}

}

#endif