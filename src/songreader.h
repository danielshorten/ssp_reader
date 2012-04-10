/**
 * songreader.h
 *
 * Header for SongReader class (songreader.cpp)
 */

#ifndef _songreader_h_included_
#define _songreader_h_included_
#include <fstream>
#include <string>

using namespace std;

class SongReader {
private:
	char* title;
	char* author;
	char* copyright;
	char* ccli_num;
	char* keys;
	char* black_hole;
	
	void store_field(char** field, ifstream* file, int field_size);
public:
	SongReader();
	void read(char* filename);
	void read(string filename);

	char* get_title();
	char* get_author();
	char* get_copyright();
	char* get_ccli_num();
	char* get_keys();
};
#endif