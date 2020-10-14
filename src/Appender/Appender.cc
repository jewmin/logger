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

#include "Appender/Appender.h"
#include "Mutex.h"
#include "CObject.h"

namespace Logger {

class AppenderContainer : public Common::CObject {
	friend class Appender;

public:
	~AppenderContainer();

protected:
	AppenderContainer();

	void DeleteAllAppenders();
	void DeleteAllAppenders(std::vector<Appender *> & appenders);
	void AddAppender(Appender * appender);
	void RemoveAppender(Appender * appender);

	static AppenderContainer * Get();

private:
	AppenderContainer(AppenderContainer &&) = delete;
	AppenderContainer(const AppenderContainer &) = delete;
	AppenderContainer & operator=(AppenderContainer &&) = delete;
	AppenderContainer & operator=(const AppenderContainer &) = delete;

private:
	std::map<const Common::SDString, Appender *> appender_map_;
	mutable Common::CMutex appender_mutex_;
};

//*********************************************************************
//Appender
//*********************************************************************

Appender::Appender(const i8 * name) : name_(name) {
	AppenderContainer::Get()->AddAppender(this);
}

Appender::~Appender() {
	AppenderContainer::Get()->RemoveAppender(this);
}

Appender * Appender::GetAppender(const i8 * name) {
	Common::CMutex::ScopedLock lock(AppenderContainer::Get()->appender_mutex_);
	auto got = AppenderContainer::Get()->appender_map_.find(Common::SDString(name));
	if (got == AppenderContainer::Get()->appender_map_.end()) {
		return nullptr;
	} else {
		return got->second;
	}
}

void Appender::DeleteAllAppenders() {
	AppenderContainer::Get()->DeleteAllAppenders();
}

bool Appender::ReOpenAll() {
	Common::CMutex::ScopedLock lock(AppenderContainer::Get()->appender_mutex_);
	bool result = true;
	for (auto & it : AppenderContainer::Get()->appender_map_) {
		result = result && it.second->ReOpen();
	}
	return result;
}

void Appender::CloseAll() {
	Common::CMutex::ScopedLock lock(AppenderContainer::Get()->appender_mutex_);
	for (auto & it : AppenderContainer::Get()->appender_map_) {
		it.second->Close();
	}
}

//*********************************************************************
//AppenderContainer
//*********************************************************************

AppenderContainer::AppenderContainer() {
}

AppenderContainer::~AppenderContainer() {
	DeleteAllAppenders();
}

void AppenderContainer::DeleteAllAppenders() {
	std::vector<Appender *> appenders;
	{
		Common::CMutex::ScopedLock lock(appender_mutex_);
		appenders.reserve(appender_map_.size());
		for (auto & it : appender_map_) {
			appenders.push_back(it.second);
		}
		appender_map_.clear();
	}
	DeleteAllAppenders(appenders);
}

void AppenderContainer::DeleteAllAppenders(std::vector<Appender *> & appenders) {
	for (auto & it : appenders) {
		delete it;
	}
}

void AppenderContainer::AddAppender(Appender * appender) {
	Common::CMutex::ScopedLock lock(appender_mutex_);
	appender_map_[appender->GetName()] = appender;
}

void AppenderContainer::RemoveAppender(Appender * appender) {
	Common::CMutex::ScopedLock lock(appender_mutex_);
	appender_map_.erase(appender->GetName());
}

AppenderContainer * AppenderContainer::Get() {
	static AppenderContainer instance;
	return &instance;
}

}