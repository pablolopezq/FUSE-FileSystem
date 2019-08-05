#include "disk.h"

#include <cstdio>
#include <cstring>

FILE * disk;

void create_disk(const char * path, int size){

	//Open disk with update mode
	disk = fopen(path, "w+");

	//Set and write empty bitmap
	uint8_t bitmap[BLOCK_SIZE];

	for(int i = 0; i < BLOCK_SIZE; i++){
		bitmap[i] = 0xFF;
	}

	write_block(bitmap, 0);

	//Set and write empty directory
	uint8_t directory[BLOCK_SIZE];

	for(int i = 0; i < BLOCK_SIZE; i++){
		bitmap[i] = 0x00;
	}

	write_block(directory, 1);

	//Calculate num_blocks
	int kb_size = size * 1024;
	int num_blocks = kb_size / 512;

	//Write empty blocks to file
	char block[BLOCK_SIZE];

	for(int i = 0; i < num_blocks; i++){

		for(int y = 0; y < BLOCK_SIZE; y++){
			block[i] = 0;
		}
		write_block(block, i + 2);
	}
}

void close_disk(){
	fflush(disk);
	fclose(disk);
}

bool write_block(char * buffer, int block){
	
	int offset = block * BLOCK_SIZE;
	
	fseek(disk, offset, SEEK_RET);

	return (fwrite(buffer, 1, BLOCK_SIZE, disk) == BLOCK_SIZE);
}

bool read_block(char * buffer, int block){

	int offset = block * BLOCK_SIZE;

	fseek(disk, offset SEEK_RET);

	return (fread(buffer, 1, BLOCK_SIZE, disk) == BLOCK_SIZE);
}

