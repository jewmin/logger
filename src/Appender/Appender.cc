#include "Appender/Appender.h"

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

Appender * Appender::GetAppender(const Common::SDString & name) {
	Common::CMutex::ScopedLock lock(AppenderContainer::Get()->appender_mutex_);
	auto got = AppenderContainer::Get()->appender_map_.find(name);
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