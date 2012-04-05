#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ioexception.h"
#include "songreader.h"

// private

// This method needs to take a field_size_length parameter
void SongReader::store_field(char** field, ifstream* file, int field_length_type) {
	int field_length = 0;
	unsigned char* buffer = new unsigned char[9];
	if (field_length_type == 6 || field_length_type == 2) { // one byte for verse length
		file->read((char*) buffer, 1);
		field_length = (int) buffer[0];
	}
	// four bytes for verse length
	else if (  field_length_type == 9
			|| field_length_type == 12
			|| field_length_type == 20) {
		file->read((char*) buffer, 4);
		// The four-byte number is stored little-endian, so we
		// do some shifting and ORing to put things together.
		for (int i = 0; i <= 3; i++) {
			field_length = field_length | (buffer[i] << (i * 8));
		}
	}
	
	delete *field;
	*field = new char[field_length];
	file->read(*field, field_length);
	delete buffer;
}

// public
SongReader::SongReader() {
	title = new char[1];
	author = new char[1];
	copyright = new char[1];;
	ccli_num = new char[1];;
	keys = new char[1];
	black_hole = new char[1]; // use to read unknown fields
}

void SongReader::read(char* filename) {
	ifstream::pos_type size;
	ifstream file;
	unsigned char* buffer;
	
	file.open (filename, ios::in | ios::binary | ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		//cout << "Size: " << size << endl;
		file.seekg(0, ios::beg);
		buffer = new unsigned char[9];
		while (!file.eof()) {
			file.read((char*) buffer, 9);
			//printf("Field ID: %d\n", buffer[0]);
			if (buffer[0] == 1)
				this->store_field(&title, &file, (int) buffer[8]);
			
			else if (buffer[0] == 2)
				this->store_field(&author, &file, (int) buffer[8]);
			
			else if (buffer[0] == 3)
				this->store_field(&copyright, &file, (int) buffer[8]);
			
			else if (buffer[0] == 5)
				this->store_field(&ccli_num, &file, (int) buffer[8]);
			
			else if (buffer[0] == 11)
				this->store_field(&keys, &file, (int) buffer[8]);
			
			/* Some hymn/demo format that doesn't label verses/choruses */
			else if (  buffer[0] == 12 /* verse? */
					|| buffer[0] == 20 /* chorus? */) {
				char* verse = new char[1];
				file.read((char*) buffer, 2);
				this->store_field(&verse, &file, (int) buffer[1]);

				// Don't make use of this yet
				delete verse;
			}
			
			/* Some insane presentation/metadata after this. EOF for all I care */
			else if (buffer[0] == 34)
				break;

			/* Labeled verses */
			else if (buffer[0] == 37) {
				int verse_length = 0;
				char* label = new char[1];
				char* verse = new char[1];
				
				this->store_field(&label, &file, (int) buffer[8]);
				// Verse length type
				file.read((char*) buffer, 1);
				this->store_field(&verse, &file, (int) buffer[0]);
				
				// Don't make use of these yet
				delete label;
				delete verse;
			}
			
			// Unknown fields with no content/length
			else if (  buffer[0] == 6
					|| buffer[0] == 9) {
				// do nothing
			}
			// Unknown fields with length
			else if (  buffer[0] == 0
					|| buffer[0] == 4 // Source?  Compliation?
					|| buffer[0] == 7
					|| buffer[0] == 8
					|| buffer[0] == 29 // Themes
					|| buffer[0] == 30
					|| buffer[0] == 31 // Verse order?
					|| buffer[0] == 32
					|| buffer[0] == 35
					|| buffer[0] == 36
					|| buffer[0] == 38
					|| buffer[0] == 39) {
						
				this->store_field(&black_hole, &file, (int) buffer[8]);
			}
						
			/**
			 * The 9th byte of these field headers seems to indicate
			 * whether or not content will follow.  6 means that the next
			 * byte will indicate the length of the field.  2 is typically
			 * followed by 0 and an empty field.
			 */
			// else if (buffer[8] == 0x06) { /* Unknown field with some content */ 
			// 	int field_size = (int) buffer[9];
			// 	// printf("Seeking %d bytes...", field_size);
			// 	file.seekg(field_size, ios::cur);
			// }
			// else if (buffer[8] == 2) { /* Unknown, empty field */ }
			
			else {
				file.close();
				delete buffer;
				stringstream ss;
				ss << "Error reading file \"" << filename << "\"";
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

void SongReader::read(string filename) {
	char* c_str_filename = (char*) filename.c_str();
	this->read(c_str_filename);
}

/* Getters */
char* SongReader::getTitle() { return title; }
char* SongReader::getAuthor() { return author; }
char* SongReader::getCopyright() { return copyright; }
char* SongReader::getCcliNum() { return ccli_num; }
char* SongReader::getKeys() { return keys; }