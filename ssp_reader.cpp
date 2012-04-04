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
		cout << e.getMessage() << endl;
	}
	
	printf("Title: %s\nAuthor: %s\nCopyright: %s\nCCLI #: %s\nKeys: %s\n",
		sr.getTitle(), sr.getAuthor(), sr.getCopyright(), sr.getCcliNum(), sr.getKeys());
	return 0;
}