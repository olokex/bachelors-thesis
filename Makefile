build:
	[ -d ./build ] || mkdir build
	cd build && cmake .. && make

clean:
	rm -rf build

.PHONY: clean build