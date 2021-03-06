#pragma once

#include <string>
#include <fstream>
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#ifdef WIN32
#include "unistd.h"
#include <Windows.h>
#define MUTEXTYPE HANDLE
#define IS_MUTEX_VALID(mux) ((mux != NULL) && (mux != INVALID_HANDLE_VALUE)) 
#define CREATEMUTEX(mux) mux = CreateMutex(NULL, FALSE, NULL)
#define DESTROYMUTEX(mux) if(IS_MUTEX_VALID(mux)) CloseHandle(mux)
#define ACQUIRE_MUTEX(mux) WaitForSingleObject(mux, INFINITE)
#define RELEASE_MUTEX(mux) ReleaseMutex(mux)

//#define PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#else
#include <pthread.h>
#include <unistd.h>

static inline pthread_mutex_t __CreateMutex() 
{
	pthread_mutex_t mux;
	pthread_mutex_init(&mux, NULL);
	return mux;
}

#define MUTEXTYPE pthread_mutex_t
#define CREATEMUTEX(mux) mux = __CreateMutex()
#define DESTROYMUTEX(mux) pthread_mutex_destroy(&mux)
#define IS_MUTEX_VALID(mux) 1
#define ACQUIRE_MUTEX(mux) pthread_mutex_lock(&mux)
#define RELEASE_MUTEX(mux) pthread_mutex_unlock(&mux)
#ifdef __SIMULATION__
#define PACK( __Declaration__ ) __Declaration__
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif
#endif

#define FILETYPE_IN std::ifstream* 
#define FILETYPE_OUT std::ofstream* 
#define NOT_EOF(stream) stream->good()

//Reads line until \n or delimeter is read
static inline std::string READ_LINE_MACRO(FILETYPE_IN AFile, char delimeter = '\n')
{
	if (!NOT_EOF(AFile))
		return "";

	std::string ret;
	std::getline(*AFile, ret, delimeter);
	return ret;
}

#define OPEN_FILE_R(path) new std::ifstream(path)
#define OPEN_FILE_W(path) new std::ofstream(path)
#define WRITE_LINE(stream, text) stream << text << std::endl
#define READ_CHUNK(stream, delimeter) READ_LINE_MACRO(stream, delimeter)
#define READ_LINE(stream) READ_LINE_MACRO(stream, '\n')
#define CLOSE_FILE(stream) stream->close(); delete stream

inline bool fileExists(const std::string& name) {
	return fs::exists(name);
}

inline void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}