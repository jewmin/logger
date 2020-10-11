#include "Hierarchy.h"

namespace Logger {

Hierarchy * Hierarchy::Get() {
	static Hierarchy instance;
	return &instance;
}

Hierarchy::Hierarchy() {
}

Hierarchy::~Hierarchy() {
	Shutdown();
	DeleteAllCategories();
}

Category * Hierarchy::GetExistingCategory(const Common::SDString & name) {
	Common::CMutex::ScopedLock lock(category_mutex_);
	return _GetExistingCategory(name);
}

Category * Hierarchy::_GetExistingCategory(const Common::SDString & name) {
	auto got = category_map_.find(name);
	if (got == category_map_.end()) {
		return nullptr;
	} else {
		return got->second;
	}
}

Category * Hierarchy::GetCategory(const Common::SDString & name) {
	Common::CMutex::ScopedLock lock(category_mutex_);
	return _GetCategory(name);
}

Category * Hierarchy::_GetCategory(const Common::SDString & name) {
	Category * category = _GetExistingCategory(name);
	if (!category) {
		if (name == "") {
			category = new Category(*name, nullptr, Priority::kInfo);
		} else {
			Common::SDString parent_name;
			size_t index = name.RFind(".");
			if (index == Common::SDString::npos) {
				parent_name = "";
			} else {
				parent_name = name.SubStr(0, index);
			}
			category = new Category(*name, _GetCategory(parent_name));
		}
		category_map_[name] = category;
	}
	return category;
}

void Hierarchy::Shutdown() {
	Common::CMutex::ScopedLock lock(category_mutex_);
	for (auto & it : category_map_) {
		it.second->RemoveAllAppenders();
	}
}

void Hierarchy::DeleteAllCategories() {
	Common::CMutex::ScopedLock lock(category_mutex_);
	for (auto & it : category_map_) {
		delete it.second;
	}
	category_map_.clear();
}

}