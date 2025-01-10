useHeap.exe: useHeap.o heap.o hash.o
	g++ -o useHeap.exe useHeap.o heap.o hash.o

useHeap.o: useHeap.cpp
	g++ -std=c++17 -O2 -c useHeap.cpp

heap.o: heap.cpp heap.h
	g++ -std=c++17 -O2 -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -std=c++17 -O2 -c hash.cpp

debug:
	g++ -g -std=c++17 -o useHeapDebug.exe useHeap.cpp heap.cpp hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups