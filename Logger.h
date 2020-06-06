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

#ifndef Logger_INCLUDED
#define Logger_INCLUDED

// stl标准库头文件
#include <new>
#include <set>
#include <map>
#include <list>
#include <ctime>
#include <atomic>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <thread>

// C头文件
#include <assert.h>
#ifdef _WIN32
#	include <io.h>
#	include <process.h>
#else
#	include <unistd.h>
#endif

// 定义标准类型，内部统一使用
typedef char				i8;
typedef short				i16;
typedef int					i32;
typedef long long			i64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

#ifdef _WIN32
	/* Windows - set up dll import/export decorators. */
#	if defined(BUILDING_LOGGER_SHARED)
		/* Building shared library. */
#		define LOGGER_EXTERN __declspec(dllexport)
#		define LOGGER_EXTERN_TEMPLATE __declspec(dllexport)
#	elif defined(USING_LOGGER_SHARED)
		/* Using shared library. */
#		define LOGGER_EXTERN __declspec(dllimport)
#		define LOGGER_EXTERN_TEMPLATE /* nothing */
#	else
		/* Building static library. */
#		define LOGGER_EXTERN /* nothing */
#		define LOGGER_EXTERN_TEMPLATE /* nothing */
#	endif
#elif __GNUC__ >= 4
#	define LOGGER_EXTERN __attribute__((visibility("default")))
#	define LOGGER_EXTERN_TEMPLATE __attribute__((visibility("default")))
#else
#	define LOGGER_EXTERN /* nothing */
#	define LOGGER_EXTERN_TEMPLATE /* nothing */
#endif

#endif