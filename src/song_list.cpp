/**
 * song_list.cpp
 *
 * Command-line utility for generating a list (CSV format) of songs from a
 * directory of SongShowPlus files.
 */

#include <dirent.h>
#include <iostream>
#include <sstream>
#include <string>
#include "ioexception.h"
#include "songreader.h"

void usage(char* progname) {
	 printf("usage: %s song_directory\n", progname);
}

int main(int argc, char* argv[]) {
	const char* PROGNAME = "songlist";
	const char* DEFAULT_OUTPUT = "output.csv";

	if (argc < 2) {
		usage((char*)PROGNAME);
		return 1;
	}

	char* dir_name = argv[1];
	char* output_filename = (char*) DEFAULT_OUTPUT;
	if (argc == 3)
		output_filename = argv[2];
	DIR* dir;
	SongReader sr;
	struct dirent *ent;
	FILE* output;
	
	dir = opendir (dir_name);
	if (dir != NULL) {
		output = fopen(output_filename, "w");
		if (output == NULL) {
			fprintf(stderr, "%s: error: Couldn't open \"%s\" for writing.", PROGNAME, output_filename);
			closedir(dir);
			return 1;
		}
		// Headers
		fprintf(output, "Song Title,CCLI #,Author(s),Key(s)\n", sr.get_title(), sr.get_ccli_num());
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
				fprintf(stderr, "Couldn't read file \"%s\": %s.  Skipping...\n", ent->d_name, e.get_message().c_str());
				continue;
			}
			fprintf(output, "\"%s\",%s,\"%s\",\"%s\"\n", sr.get_title(), sr.get_ccli_num(), sr.get_author(), sr.get_keys());
		}
		fclose(output);
		closedir (dir);
	}
	else {
		cerr << "Couldn't open directory \"" << dir_name << "\"\n";
		return 1;
	}
	
	return 0;
}