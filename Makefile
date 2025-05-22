compile:
	cmake -B build
	cmake --build build -j 12
run:
	./build/src/kpass
