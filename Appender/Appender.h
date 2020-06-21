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

#ifndef Logger_Appender_Appender_INCLUDED
#define Logger_Appender_Appender_INCLUDED

#include "Mutex.h"
#include "Record.h"
#include "Layout/Layout.h"

namespace Logger {

class LOGGER_EXTERN Appender {
	friend class Category;

public:
	virtual ~Appender();

	virtual void DoAppend(const Record & record) = 0;
	virtual bool ReOpen() = 0;
	virtual void Close() = 0;
	virtual bool RequiresLayout() const = 0;
	virtual void SetLayout(Layout * layout) = 0;

	const std::string & GetName() const;

	static Appender * GetAppender(const std::string & name);
	static void DeleteAllAppenders();
	static bool ReOpenAll();
	static void CloseAll();

protected:
	Appender(const std::string & name);

private:
	Appender(Appender &&) = delete;
	Appender(const Appender &) = delete;
	Appender & operator=(Appender &&) = delete;
	Appender & operator=(const Appender &) = delete;

private:
	const std::string name_;
};

class AppenderMapStorage {
	friend class Appender;

public:
	~AppenderMapStorage();

protected:
	AppenderMapStorage();

	std::unordered_map<std::string, Appender *> * GetAllAppenders();
	void DeleteAllAppenders();
	void _DeleteAllAppenders(std::vector<Appender *> & appenders);
	void AddAppender(Appender * appender);
	void RemoveAppender(Appender * appender);

	static AppenderMapStorage * Get();

private:
	AppenderMapStorage(AppenderMapStorage &&) = delete;
	AppenderMapStorage(const AppenderMapStorage &) = delete;
	AppenderMapStorage & operator=(AppenderMapStorage &&) = delete;
	AppenderMapStorage & operator=(const AppenderMapStorage &) = delete;

private:
	std::unordered_map<std::string, Appender *> * appender_map_;
	mutable Mutex appender_mutex_;
};

//*********************************************************************
//Appender
//*********************************************************************

inline const std::string & Appender::GetName() const {
	return name_;
}

}

#endif