

.PHONY:format

format:
	bash script/format.sh

build:
	cmake -S . -B build
	cmake --build build

test: build
	cd build && ctest

clean:
	rm -rf build