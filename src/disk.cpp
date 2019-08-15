#include "disk.h"
#include "structs.h"

#include <cstring>

FILE * disk;

void create_disk(const char * path){

	//Open disk with update mode
	disk = open_disk(path);

	//Write empty blocks to file
	char block[BLOCK_SIZE];
	int num_blocks = BLOCK_SIZE * 8;

	for(int i = 0; i < num_blocks; i++){

		for(int y = 0; y < BLOCK_SIZE; y++){
			block[i] = 0;
		}
		write_block(block, i);
	}

	char bitmap[BLOCK_SIZE];

	for(int i = 0; i < BLOCK_SIZE; i++){
		bitmap[i] = 0xFF;
	}

	write_block(bitmap, 0);

	//Set and write empty directory
	File_Entry file_entry;
	file_entry.name[0] = '\0';
	file_entry.is_directory = 0;
	file_entry.index_block = -1;

	int num_entries = BLOCK_SIZE * 8;
	int directory_size = (num_entries * 16) / BLOCK_SIZE;
	int entries_per_block = 4096 / 16;

	char dir_block_buffer[4096];

	for(int i = 0; i < entries_per_block; i++){
		memcpy(&dir_block_buffer[i * 32], &file_entry, sizeof(File_Entry));
	}

	for(int i = 0; i < directory_size; i++){
		write_block(dir_block_buffer, i + 1);
	}
	
}

void close_disk(){
	fflush(disk);
	fclose(disk);
}

FILE * open_disk(const char * path){
	return (fopen(path, "+w"));
}

bool write_block(char * buffer, int block){
	
	int offset = block * BLOCK_SIZE;
	
	fseek(disk, offset, SEEK_SET);

	return (fwrite(buffer, 1, BLOCK_SIZE, disk) == BLOCK_SIZE);
}

bool read_block(char * buffer, int block){

	int offset = block * BLOCK_SIZE;

	fseek(disk, offset, SEEK_SET);

	return (fread(buffer, 1, BLOCK_SIZE, disk) == BLOCK_SIZE);
}