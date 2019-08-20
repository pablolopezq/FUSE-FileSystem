run:
	rm -r build
	mkdir build
	cd build
	g++ -o plfs src/*.cpp -D_FILE_OFFSET_BITS=64
	./plfs