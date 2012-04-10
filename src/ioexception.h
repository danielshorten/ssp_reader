/**
 * ioexception.h
 *
 * Header for IOException (ioexception.cpp)
 */
#ifndef _ioexception_h_included_
#define _ioexception_h_included_
#include <string>

using namespace std;

class IOException {
private:
	string message;

public:
	IOException();
	IOException(string s);
	string get_message();
};
#endif