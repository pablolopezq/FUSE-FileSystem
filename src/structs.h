#ifndef STRUCTS_H
#define STRUCTS_H

struct File_Entry{
	char name[11];
	bool is_directory;
	int index_block;
};

#endif