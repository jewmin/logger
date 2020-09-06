#include "Hierarchy.h"

namespace Logger {

Hierarchy * Hierarchy::Get() {
	static Hierarchy instance;
	return &instance;
}

Hierarchy::Hierarchy() : category_map_(new std::unordered_map<std::string, Category *>()) {
}

Hierarchy::~Hierarchy() {
	Shutdown();
	DeleteAllCategories();
	delete category_map_;
}

Category * Hierarchy::GetExistingCategory(const std::string & name) {
	Mutex::ScopedLock lock(category_mutex_);
	return _GetExistingCategory(name);
}

Category * Hierarchy::_GetExistingCategory(const std::string & name) {
	auto got = category_map_->find(name);
	if (got == category_map_->end()) {
		return nullptr;
	} else {
		return got->second;
	}
}

Category * Hierarchy::GetCategory(const std::string & name) {
	Mutex::ScopedLock lock(category_mutex_);
	return _GetCategory(name);
}

Category * Hierarchy::_GetCategory(const std::string & name) {
	Category * category = _GetExistingCategory(name);
	if (!category) {
		if (name == "") {
			category = new Category(name, nullptr, Priority::kInfo);
		} else {
			std::string parent_name;
			std::string::size_type index = name.find_last_of('.');
			if (index == std::string::npos) {
				parent_name = "";
			} else {
				parent_name = name.substr(0, index);
			}
			category = new Category(name, _GetCategory(parent_name));
		}
		(*category_map_)[name] = category;
	}
	return category;
}

void Hierarchy::Shutdown() {
	Mutex::ScopedLock lock(category_mutex_);
	for (auto & it : *category_map_) {
		it.second->RemoveAllAppenders();
	}
}

void Hierarchy::DeleteAllCategories() {
	Mutex::ScopedLock lock(category_mutex_);
	for (auto & it : *category_map_) {
		delete it.second;
	}
	category_map_->clear();
}

}