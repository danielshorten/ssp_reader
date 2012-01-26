#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class IOException {
private:
	string message;
public:
	IOException() { message = ""; }
	IOException(string s) { message = s; }
	string getMessage() { return message; }
};

class SongReader {
private:
	char* title;
	char* author;
	char* copyright;
	char* ccli_num;
	char* keys;
	
	/**
	 * Utility method to store standard fields
	 */
	void store_field(char** field, ifstream* file, int field_size) {
		*field = new char[field_size];
		file->read(*field, field_size);
	}
public:
	void read(char* filename) {
		ifstream::pos_type size;
		ifstream file;
		unsigned char* buffer;
		
		file.open (filename, ios::in | ios::binary | ios::ate);
		if (file.is_open()) {
			size = file.tellg();
			//cout << "Size: " << size << endl;
			file.seekg(0, ios::beg);
			buffer = new unsigned char[10];
			while (!file.eof()) {
				file.read((char*) buffer, 10);
				// printf("Field ID: %d\n", buffer[0]);
				if (buffer[0] == 1)
					this->store_field(&title, &file, (int) buffer[9]);
				
				else if (buffer[0] == 2)
					this->store_field(&author, &file, (int) buffer[9]);
				
				else if (buffer[0] == 3)
					this->store_field(&copyright, &file, (int) buffer[9]);
				
				else if (buffer[0] == 5)
					this->store_field(&ccli_num, &file, (int) buffer[9]);

 				/* Some weird, empty field with 9-byte header? */
				else if (buffer[0] == 6)
					file.seekg(-1, ios::cur);
				
				else if (buffer[0] == 11)
					this->store_field(&keys, &file, (int) buffer[9]);
				
				/* Some hymn/demo format that doesn't label verses/choruses */
				else if (buffer[0] == 12) {
					
				}
				
				/* Labeled verses */
				else if (buffer[0] == 37) {
					int verse_length = 0;
					int label_length = (int) buffer[9];
					char* label = new char[label_length];
					char* verse;
					
					file.read(label, label_length);
					file.read((char*) buffer, 1);
					if (buffer[0] == 6) { // one byte for verse length
						file.read((char*) buffer, 1);
						verse_length = (int) buffer[0];
					}
					else if (buffer[0] == 12) { // four bytes for verse length
						file.read((char*) buffer, 4);
						// The four-byte number is stored little-endian, so we
						// do some shifting and ORing to put things together.
						for (int i = 0; i < 3; i++) {
							verse_length = verse_length | ((int) buffer[i] << (i * 8));
						}
					}
					verse = new char[verse_length];
					file.read(verse, verse_length);
					
					// Don't make use of these yet
					delete label;
					delete verse;
				}
				
				/**
				 * The 9th byte of these field headers seems to indicate
				 * whether or not content will follow.  6 means that the next
				 * byte will indicate the length of the field.  2 is typically
				 * followed by 0 and an empty field.
				 */
				else if (buffer[8] == 0x06) { /* Unknown field with some content */ 
					int field_size = (int) buffer[9];
					// printf("Seeking %d bytes...", field_size);
					file.seekg(field_size, ios::cur);
				}
				else if (buffer[8] == 2) { /* Unknown, empty field */ }
				
				else {
					stringstream ss;
					ss << "Error reading file";
					throw IOException(ss.str());
				}
			}
			// cout << "\nEnd of file." << endl;
			file.close();
			delete buffer;
		}
		else {
			stringstream ss;
			ss << "Couldn't open file \"" << filename << "\"";
			throw IOException(ss.str());
		}
	}
	
	/* Getters */
	char* getTitle() { return title; }
	char* getAuthor() { return author; }
	char* getCopyright() { return copyright; }
	char* getCcliNum() { return ccli_num; }
	char* getKeys() { return keys; }
};

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