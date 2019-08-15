#include <fuse.h>
#include <cstdio>
#include "filesystem.h"

struct fuse_operations operations = {
	.init = pl_init,
	.getattr = pl_getattr,
	.rmdir = pl_rmdir,
	.read = pl_read,
	.write = pl_write,
	.readdir = pl_readdir,
	.mkdir = pl_mkdir,
	.rename = pl_rename
};

int main(int argc, char *argv[]){
	
	create_disk(argv[1]);

	int fuse_stat;

	fuse_stat = fuse_main(argc, argv, &fuse_operations, NULL);

	close_disk();

	return fuse_stat;
}