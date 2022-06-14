all: preload.cc
	g++ -O3 -shared -fPIC preload.cc -o preload.so
