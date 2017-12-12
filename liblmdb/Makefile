OPT = -O2 -g
W	= -W -Wall -Wno-unused-parameter -Wbad-function-cast -Wuninitialized
THREADS = -pthread
CFLAGS = -pthread $(OPT) $(W) $(XCFLAGS)

.PHONY: all natives webassembly clean

all: natives webassembly

natives: build/linux64/liblmdb.a build/windows64/liblmdb.a

webassembly: build/asmjs/liblmdb.bc build/binaryen/liblmdb.bc

clean:
	rm -rf build

build/asmjs/mdb.bc: mdb.c lmdb.h midl.h
	mkdir -p build/asmjs
	emcc -s USE_PTHREADS=1 $(CFLAGS) -o $@ mdb.c

build/asmjs/midl.bc: midl.c midl.h
	mkdir -p build/asmjs
	emcc -s USE_PTHREADS=1 $(CFLAGS) -o $@ midl.c

build/asmjs/liblmdb.bc: build/asmjs/mdb.bc build/asmjs/midl.bc
	emcc -s USE_PTHREADS=1 -o $@ build/asmjs/mdb.bc build/asmjs/midl.bc

build/binaryen/mdb.bc: mdb.c lmdb.h midl.h
	mkdir -p build/binaryen
	emcc -s USE_PTHREADS=1 -s BINARYEN=1 $(CFLAGS) -o $@ mdb.c

build/binaryen/midl.bc: midl.c midl.h
	mkdir -p build/asmjs
	emcc -s USE_PTHREADS=1 -s BINARYEN=1 $(CFLAGS) -o $@ midl.c

build/binaryen/liblmdb.bc: build/binaryen/mdb.bc build/binaryen/midl.bc
	emcc -s USE_PTHREADS=1 -s BINARYEN=1 -o $@ build/binaryen/mdb.bc build/binaryen/midl.bc

build/linux64/mdb.o: mdb.c lmdb.h midl.h
	mkdir -p build/linux64
	x86_64-linux-gnu-gcc $(CFLAGS) -o $@ mdb.c

build/linux64/midl.o: midl.c midl.h
	mkdir -p build/linux64
	x86_64-linux-gnu-gcc $(CFLAGS) -o $@ midl.c

build/linux64/liblmdb.a: build/linux64/mdb.o build/linux64/midl.o
	x86_64-linux-gnu-ar rs $@ build/linux64/mdb.o build/linux64/midl.o

build/windows64/mdb.o: mdb.c lmdb.h midl.h
	mkdir -p build/windows64
	x86_64-w64-mingw32-gcc $(CFLAGS) -o $@ mdb.c

build/windows64/midl.o: midl.c midl.h
	mkdir -p build/windows64
	x86_64-w64-mingw32-gcc $(CFLAGS) -o $@ midl.c

build/windows64/liblmdb.a: build/windows64/mdb.o build/windows64/midl.o
	x86_64-w64-mingw32-ar rs $@ build/windows64/mdb.o build/windows64/midl.o
