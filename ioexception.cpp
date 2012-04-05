#include "ioexception.h"

IOException::IOException() { message = ""; }
IOException::IOException(string s) { message = s; }
string IOException::get_message() { return message; }