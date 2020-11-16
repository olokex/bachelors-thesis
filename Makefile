build:
	[ -d ./build ] || mkdir build
	#cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

clean:
	rm -rf build

.PHONY: clean build