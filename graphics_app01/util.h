#ifndef UTIL_H
#define UTIL_H
#include "types.h"
#include <stdlib.h>
#include <string>
#include <assert.h>

bool ReadFile(const char* fileName, std::string& outFile);

#define RANDOM rand
#define SRANDOM srand((unsigned)0)

void ErrorMessage(const char* pFileName, uint line, const char* msg, ...);

#define ERROR_MESSAGE0(msg) ErrorMessage(__FILE__, __LINE__, msg)
#define ERROR_MESSAGE(msg, ...) ErrorMessage(__FILE__, __LINE__, msg, __VA_ARGS__)


#define INVALID_UNIFORM_LOCATION 0xffffffff

#endif // !UTIL_H

