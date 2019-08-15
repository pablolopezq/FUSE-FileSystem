#include "filesystem.h"
#include "structs.h"
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>

char bitmap[BLOCK_SIZE];
File_Entry * directory;

void load_directory(){
	int num_entries = BLOCK_SIZE * 8;
	int directory_size = (num_entries * 16) / BLOCK_SIZE;

	for(int i = 0; i < directory_size; i++){
		read_block(reinterpret_cast<char*>(&directory[i * 4096]), i + 1);
	}
}

struct File_Entry * get_entry(const char * path){
	int num_entries = BLOCK_SIZE * 8;
	int directory_size = (num_entries * 16) / BLOCK_SIZE;
	int entries_per_block = 4096 / 16;

	for(int i = 0; i < directory_size; i++){
		for(int x = 0; x < entries_per_block; x++){
			if(strcmp(path, directory[i * 4096 + x * 16].name) == 0){
				return &directory[i * 4096 + x * 16];
			}
		}
	}
}

void get_file_size(struct File_Entry * entry, int * size, int * blocks){
	char index_block_buffer[4096];
	read_block(index_block_buffer, entry->index_block);

	int pointers_per_block = BLOCK_SIZE / 4;

	for(int i = 0; i < pointers_per_block; i++){
		if(index_block_buffer[i * 4] == -1){
			*size = i * BLOCK_SIZE;
			*blocks = i;
			break;
		}
	}
}

int get_free_block(){

	int occupied_blocks;
	int first_set_bit;

	for(int i = 0; i < 512; i++){
		if(bitmap[i] != 0){
			occupied_blocks = i;
			break;
		}
	}

	for(int i = 0; i < 8; i++){
		if(((bitmap[occupied_blocks] >> i) & 1) == 1){
			first_set_bit = i;
			break;
		}
	}

	return (8 * occupied_blocks + first_set_bit);

}

void set_busy_block(int block){

	int byte = floor(block / 8);
	int block_num = block % 8;

	bitmap[byte] &= ~(1 << block_num);

}

void set_free_block(int block){

	int byte = floor(block / 8);
	int block_num = block % 8;

	bitmap[byte] |= 1 << block_num;
	
}

void pl_init (struct fuse_conn_info *conn){
	read_block(bitmap, 0);
	load_directory();
}

int pl_getattr (const char * path, struct stat * stat_buff){

	if (strcmp(path, "/")==0) {
        stat_buff->st_mode = S_IFDIR|0777;
		stat_buff->st_uid = 0;
        stat_buff->st_gid = 0;
        stat_buff->st_nlink = 1;
        stat_buff->st_ino = 0;
        stat_buff->st_size = BLOCK_SIZE;
        stat_buff->st_blksize = BLOCK_SIZE;
        stat_buff->st_blocks = 1;
    }
    else{
        struct File_Entry* entry = get_entry(path);

        if(entry == NULL) {
            return -1;
        }
        
        if(entry->is_directory) {
            stat_buff->st_mode = S_IFDIR|0777;
            stat_buff->st_uid = 0;
            stat_buff->st_gid = 0;
            stat_buff->st_nlink = 1;
            stat_buff->st_ino = 0;
            stat_buff->st_size = BLOCK_SIZE;
            stat_buff->st_blksize = BLOCK_SIZE;
            stat_buff->st_blocks = 1;
        }
        else{
            int size, blocks;

            get_file_size(entry, &size, &blocks);

            stat_buff->st_mode = S_IFREG|0777;
            stat_buff->st_nlink = 1;
            stat_buff->st_ino = 0;
            stat_buff->st_uid = 0;
            stat_buff->st_gid= 0 ;
            stat_buff->st_size = size; 
            stat_buff->st_blksize = BLOCK_SIZE;
            stat_buff->st_blocks = blocks;
        }
    }
    return 0;
}

int pl_rmdir (const char * path){

	struct File_Entry * entry = get_entry(path);
	entry->name[0] = '\0';

	if(entry == NULL)
		return -1;

	char index_block_buffer[4096];
	read_block(index_block_buffer, entry->index_block);
	int pointers_per_block = BLOCK_SIZE / 4;

	char block_num_char[4];

	for(int i = 0; i < pointers_per_block; i++){
		memcpy(block_num_char, index_block_buffer + (16 * i), 4);
		int block_num = atoi(block_num_char);
		set_free_block(block_num);
	}

	return 0;
}

int pl_rename(const char * path, const char * new_name){

	if(strlen(new_name) > 11)
		return -1;

	struct File_Entry * entry = get_entry(path);
	strcpy(entry->name, new_name);

	return 0;
}
