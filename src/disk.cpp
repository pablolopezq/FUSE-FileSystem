#include "disk.h"

#include <cstdio>
//#include <cmath>
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