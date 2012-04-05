#include <dirent.h>
#include <iostream>
#include <sstream>
#include "ioexception.h"
#include "songreader.h"

void usage(char* progname) {
	 printf("usage: %s song_directory\n", progname);
}

int main(int argc, char* argv[]) {
	const char* PROGNAME = "songlist";

	if (argc != 2) {
		usage((char*)PROGNAME);
		return 1;
	}

	char* dir_name = argv[1];
	DIR *dir;
	SongReader sr;
	struct dirent *ent;
	dir = opendir (dir_name);
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string filename = string(ent->d_name);
			if (filename.compare(".") == 0 || filename.compare("..") == 0) continue;
			try {
				stringstream ss;
				ss << dir_name << "/" << ent->d_name;
				//cout << "Reading " << ss.str() << "...\n";
				sr.read(ss.str());
			}
			catch (IOException e) {
				fprintf(stderr, "Couldn't read file \"%s\": %s.  Skipping...\n", ent->d_name, e.getMessage().c_str());
			}
		}
		closedir (dir);
	}
	else {
		cerr << "Couldn't open directory \"" << dir_name << "\"\n";
		return 1;
	}
	
	return 0;
}