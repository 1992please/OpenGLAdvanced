#include <stdio.h>
#include <Windows.h>
#include "util.h"
#include <fstream>

using namespace std;
bool ReadFile(const char* pFileName, string& outFile)
{
	ifstream f(pFileName);
	bool ret = false;
	if (f.is_open()) {
		string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();

		ret = true;
	}
	else {
		ERROR_MESSAGE0(pFileName);
	}

	return ret;
}

void ErrorMessage(const char* pFileName, uint line, const char* format, ...)
{
	char msg[1000];
	va_list args;
	va_start(args, format);
	vsnprintf_s(msg, sizeof(msg), format, args);
	va_end(args);

	char msg2[1000];
	_snprintf_s(msg2, sizeof(msg2), "%s:%d: %s", pFileName, line, msg);
	MessageBoxA(NULL, msg2, NULL, 0);   
}