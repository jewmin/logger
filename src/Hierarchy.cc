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