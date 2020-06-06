#include "Appender/Appender.h"

namespace Logger {

//*********************************************************************
//Appender
//*********************************************************************

Appender::Appender(const std::string & name) : name_(name) {
	AppenderMapStorage::Get()->AddAppender(this);
}

Appender::~Appender() {
	AppenderMapStorage::Get()->RemoveAppender(this);
}

Appender * Appender::GetAppender(const std::string & name) {
	Mutex::ScopedLock lock(AppenderMapStorage::Get()->appender_mutex_);
	std::unordered_map<std::string, Appender *> * appenders = AppenderMapStorage::Get()->GetAllAppenders();
	auto got = appenders->find(name);
	if (got == appenders->end()) {
		return nullptr;
	} else {
		return got->second;
	}
}


void Appender::DeleteAllAppenders() {
	AppenderMapStorage::Get()->DeleteAllAppenders();
}

bool Appender::ReOpenAll() {
	Mutex::ScopedLock lock(AppenderMapStorage::Get()->appender_mutex_);
	bool result = true;
	std::unordered_map<std::string, Appender *> * appenders = AppenderMapStorage::Get()->GetAllAppenders();
	for (auto & it : *appenders) {
		result = result && it.second->ReOpen();
	}
	return result;
}

void Appender::CloseAll() {
	Mutex::ScopedLock lock(AppenderMapStorage::Get()->appender_mutex_);
	std::unordered_map<std::string, Appender *> * appenders = AppenderMapStorage::Get()->GetAllAppenders();
	for (auto & it : *appenders) {
		it.second->Close();
	}
}

//*********************************************************************
//AppenderMapStorage
//*********************************************************************

AppenderMapStorage::AppenderMapStorage() : appender_map_(new std::unordered_map<std::string, Appender *>()) {
}

AppenderMapStorage::~AppenderMapStorage() {
	DeleteAllAppenders();
	delete appender_map_;
}

std::unordered_map<std::string, Appender *> * AppenderMapStorage::GetAllAppenders() {
	return appender_map_;
}

void AppenderMapStorage::DeleteAllAppenders() {
	std::vector<Appender *> appenders;
	{
		Mutex::ScopedLock lock(appender_mutex_);
		appenders.reserve(appender_map_->size());
		for (auto & it : *appender_map_) {
			appenders.push_back(it.second);
		}
		appender_map_->clear();
	}
	_DeleteAllAppenders(appenders);
}

void AppenderMapStorage::_DeleteAllAppenders(std::vector<Appender *> & appenders) {
	for (auto & it : appenders) {
		delete it;
	}
}

void AppenderMapStorage::AddAppender(Appender * appender) {
	Mutex::ScopedLock lock(appender_mutex_);
	(*appender_map_)[appender->GetName()] = appender;
}

void AppenderMapStorage::RemoveAppender(Appender * appender) {
	Mutex::ScopedLock lock(appender_mutex_);
	appender_map_->erase(appender->GetName());
}

AppenderMapStorage * AppenderMapStorage::Get() {
	static AppenderMapStorage instance;
	return &instance;
}

}