#include "Category.h"
#include "Hierarchy.h"
#include "StringUtil.h"

namespace Logger {

Category * Category::GetRoot() {
	return GetCategory("");
}

void Category::SetRootPriority(Priority::Value priority) {
	GetRoot()->SetPriority(priority);
}

Priority::Value Category::GetRootPriority() {
	return GetRoot()->GetPriority();
}

Category * Category::GetCategory(const std::string & name) {
	return Hierarchy::Get()->GetCategory(name);
}

Category * Category::GetExistingCategory(const std::string & name) {
	return Hierarchy::Get()->GetExistingCategory(name);
}

void Category::Shutdown() {
	Hierarchy::Get()->Shutdown();
}

void Category::ShutdownForced() {
	Hierarchy::Get()->Shutdown();
	Appender::DeleteAllAppenders();
}

//*********************************************************************
//Category
//*********************************************************************

Category::Category(const std::string & name, Category * parent, Priority::Value priority)
	: name_(name), parent_(parent), priority_(priority), is_additive_(true), appender_map_(new std::unordered_map<std::string, Appender *>()) {
}

Category::~Category() {
	RemoveAllAppenders();
	delete appender_map_;
}

const std::string Category::GetName() const {
	return name_;
}

void Category::SetPriority(Priority::Value priority) {
	if (priority > Priority::kNotSet || GetParent()) {
		priority_ = priority;
	} else {
		throw std::invalid_argument("cannot set priority kNotSet on Root Category");
	}
}

Priority::Value Category::GetPriority() const {
	return priority_;
}

Priority::Value Category::GetChainedPriority() const {
	const Category * category = this;
	while (category->GetPriority() <= Priority::kNotSet) {
		category = category->GetParent();
	}
	return category->GetPriority();
}

bool Category::IsPriorityEnabled(Priority::Value priority) const {
	return GetChainedPriority() <= priority;
}

void Category::AddAppender(Appender * appender) {
	if (appender) {
		Mutex::ScopedLock lock(appenders_mutex_);
		(*appender_map_)[appender->GetName()] = appender;
	} else {
		throw std::invalid_argument("appender is nullptr");
	}
}

Appender * Category::GetAppender() const {
	Mutex::ScopedLock lock(appenders_mutex_);
	auto it = appender_map_->begin();
	if (it == appender_map_->end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

Appender * Category::GetAppender(const std::string & name) const {
	Mutex::ScopedLock lock(appenders_mutex_);
	auto it = appender_map_->find(name);
	if (it == appender_map_->end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void Category::RemoveAllAppenders() {
	Mutex::ScopedLock lock(appenders_mutex_);
	appender_map_->clear();
}

void Category::RemoveAppender(Appender * appender) {
	if (appender) {
		Mutex::ScopedLock lock(appenders_mutex_);
		appender_map_->erase(appender->GetName());
	} else {
		throw std::invalid_argument("appender is nullptr");
	}
}

void Category::CallAppenders(const Record & record) {
	{
		Mutex::ScopedLock lock(appenders_mutex_);
		if (!appender_map_->empty()) {
			for (auto & it : *appender_map_) {
				it.second->DoAppend(record);
			}
		}
	}
	if (GetAdditivity() && GetParent()) {
		GetParent()->CallAppenders(record);
	}
}

void Category::SetAdditivity(bool additivity) {
	is_additive_ = additivity;
}

bool Category::GetAdditivity() const {
	return is_additive_;
}

Category * Category::GetParent() {
	return parent_;
}

const Category * Category::GetParent() const {
	return parent_;
}

void Category::_Log(Priority::Value priority, const i8 * format, va_list arguments) {
	_Log(priority, StringUtil::FormatVa(format, arguments));
}

void Category::_Log(Priority::Value priority, const std::string & message) {
	Record record(GetName(), message, priority);
	CallAppenders(record);
}

void Category::Log(Priority::Value priority, const i8 * format, ...) {
	if (IsPriorityEnabled(priority)) {
		va_list va;
		va_start(va, format);
		_Log(priority, format, va);
		va_end(va);
	}
}

void Category::Log(Priority::Value priority, const std::string & message) {
	if (IsPriorityEnabled(priority)) {
		_Log(priority, message);
	}
}

void Category::LogVa(Priority::Value priority, const i8 * format, va_list va) {
	if (IsPriorityEnabled(priority)) {
		_Log(priority, format, va);
	}
}

void Category::Debug(const i8 * format, ...) {
	if (IsDebugEnabled()) {
		va_list va;
		va_start(va, format);
		_Log(Priority::kDebug, format, va);
		va_end(va);
	}
}

void Category::Debug(const std::string & message) {
	if (IsDebugEnabled()) {
		_Log(Priority::kDebug, message);
	}
}

void Category::Info(const i8 * format, ...) {
	if (IsDebugEnabled()) {
		va_list va;
		va_start(va, format);
		_Log(Priority::kDebug, format, va);
		va_end(va);
	}
}

void Category::Info(const std::string & message) {
	if (IsDebugEnabled()) {
		_Log(Priority::kDebug, message);
	}
}

void Category::Warn(const i8 * format, ...) {
	if (IsWarnEnabled()) {
		va_list va;
		va_start(va, format);
		_Log(Priority::kWarn, format, va);
		va_end(va);
	}
}

void Category::Warn(const std::string & message) {
	if (IsWarnEnabled()) {
		_Log(Priority::kWarn, message);
	}
}

void Category::Error(const i8 * format, ...) {
	if (IsErrorEnabled()) {
		va_list va;
		va_start(va, format);
		_Log(Priority::kError, format, va);
		va_end(va);
	}
}

void Category::Error(const std::string & message) {
	if (IsErrorEnabled()) {
		_Log(Priority::kError, message);
	}
}

void Category::Fatal(const i8 * format, ...) {
	if (IsFatalEnabled()) {
		va_list va;
		va_start(va, format);
		_Log(Priority::kFatal, format, va);
		va_end(va);
	}
}

void Category::Fatal(const std::string & message) {
	if (IsFatalEnabled()) {
		_Log(Priority::kFatal, message);
	}
}

}