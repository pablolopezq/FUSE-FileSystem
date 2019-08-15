#Proyecto Sistemas Operativos 2
#FUSE File System

##Estructura del file system
	
	Tamano del bloque en disco: 4096 bytes
	Tamano del sector en file system: 4096 bytes

	Tamano del bitmap en bytes: (TAMANO_PARTICION_BYTES / TAMANO_DE_BLOQUE) / 8

	FCB: char[FILE_NAME_SIZE] name
		 bool is_directory
		 int index_block

	Cantidad de entradas : TAMANO_PARTICION_BYTES / TAMANO_DE_BLOQUE