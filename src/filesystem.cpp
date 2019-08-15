#include "filesystem.h"
#include "structs.h"
#include <cstdio>
#include <cmath>

char bitmap[BLOCK_SIZE];
File_Entry * directory;

void load_directory(){
	int num_entries = BLOCK_SIZE * 8;
	int directory_size = (num_entries * 16) / BLOCK_SIZE;

	for(int i = 0; i < directory_size; i++){
		read_block(&directory[i * 4096], i + 1);
	}
}

struct File_Entry * pl_get_entry(char * path){
	int num_entries = BLOCK_SIZE * 8;
	int directory_size = (num_entries * 16) / BLOCK_SIZE;
	int entries_per_block = 4096 / 16;

	for(int i = 0; i < directory_size; i++){
		for(int x = o; x < entries_per_block; x++){
			if(strcmp(path, directory[i * 4096 + x * 16]) == 0){
				return directory[i * 4096 + x * 16];
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
        statbuf->st_mode = S_IFDIR|0777;
		statbuf->st_uid = 0;
        statbuf->st_gid = 0;
        statbuf->st_nlink = 1;
        statbuf->st_ino = 0;
        statbuf->st_size = BLOCK_SIZE;
        statbuf->st_blksize = BLOCK_SIZE;
        statbuf->st_blocks = 1;
    }
    else{
        struct File_Entry* entry = pl_get_entry(path);

        if(entry==NULL) {
            return -ENOENT;
        }
        
        if(entry->is_dir) {
            statbuf->st_mode = S_IFDIR|0777;
            statbuf->st_uid = 0;
            statbuf->st_gid = 0;
            statbuf->st_nlink = 1;
            statbuf->st_in o =0;
            statbuf->st_size = BLOCK_SIZE;
            statbuf->st_blksize = BLOCK_SIZE;
            statbuf->st_blocks = 1;
        }
        else{
            int size, blocks;

            pl_get_file_size(entry, &size, &blocks);

            statbuf->st_mode = S_IFREG|0777;
            statbuf->st_nlink = 1;
            statbuf->st_ino = 0;
            statbuf->st_uid = 0;
            statbuf->st_gid= 0 ;
            statbuf->st_size = size; 
            statbuf->st_blksize = BLOCK_SIZE;
            statbuf->st_blocks = blocks;
        }
    }
    return 0;
}