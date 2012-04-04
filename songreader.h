#ifndef _songreader_h_included_
#define _songreader_h_included_
#include <fstream>

using namespace std;

class SongReader {
private:
	char* title;
	char* author;
	char* copyright;
	char* ccli_num;
	char* keys;
	
	void store_field(char** field, ifstream* file, int field_size);
public:
	void read(char* filename);

	char* getTitle();
	char* getAuthor();
	char* getCopyright();
	char* getCcliNum();
	char* getKeys();
};
#endif