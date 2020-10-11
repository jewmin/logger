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

#ifndef Logger_Hierarchy_INCLUDED
#define Logger_Hierarchy_INCLUDED

#include "Common.h"
#include "CObject.h"
#include "Mutex.h"
#include "Category.h"
#include "SDString.h"

namespace Logger {

class Hierarchy : public Common::CObject {
public:
	~Hierarchy();

	Category * GetExistingCategory(const Common::SDString & name);
	Category * GetCategory(const Common::SDString & name);
	void Shutdown();
	void DeleteAllCategories();

	static Hierarchy * Get();

protected:
	Hierarchy();

	Category * _GetExistingCategory(const Common::SDString & name);
	Category * _GetCategory(const Common::SDString & name);

private:
	Hierarchy(Hierarchy &&) = delete;
	Hierarchy(const Hierarchy &) = delete;
	Hierarchy & operator=(Hierarchy &&) = delete;
	Hierarchy & operator=(const Hierarchy &) = delete;

protected:
	std::map<const Common::SDString, Category *> category_map_;
	mutable Common::CMutex category_mutex_;
};

}

#endif