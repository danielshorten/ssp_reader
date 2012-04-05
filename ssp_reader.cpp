#include <iostream>
#include "ioexception.h"
#include "songreader.h"

using namespace std;

void usage(char* progname) {
	 printf("usage: %s file\n", progname);
}

int main(int argc, char* argv[]) {
	const char* PROGNAME = "ssp";
	
	SongReader sr;
	
	if (argc != 2) {
		usage((char*)PROGNAME);
		return 1;
	}
	
	try {
		sr.read(argv[1]);
	}
	catch (IOException e) {
		cout << e.get_message() << endl;
	}
	
	printf("Title: %s\nAuthor: %s\nCopyright: %s\nCCLI #: %s\nKeys: %s\n",
		sr.get_title(), sr.get_author(), sr.get_copyright(), sr.get_ccli_num(), sr.get_keys());
	return 0;
}